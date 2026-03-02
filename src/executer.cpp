#include "executer.hpp"
#include "builtins.hpp"

#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <iostream>
#include <vector>
#include <string>
#include <cerrno>

void Executer::execute(const std::vector<std::string> &tokens)
{
    if (Builtins::handle(tokens))
        return;

    bool redirect = false;
    std::string outFile;
    bool background = false;

    std::vector<std::string> cleanedTokens;
    cleanedTokens.reserve(tokens.size());

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "&" && i == tokens.size() - 1) {
            background = true;
            continue;
        }

        if (tokens[i] == ">") {
            if (i + 1 < tokens.size()) {
                redirect = true;
                outFile = tokens[i + 1];
                ++i;
                continue;
            } else {
                std::cerr << "nsh: syntax error near unexpected token `>'\n";
                return;
            }
        }

        cleanedTokens.push_back(tokens[i]);
    }

    if (cleanedTokens.empty())
        return;

    std::vector<const char *> argv;
    argv.reserve(cleanedTokens.size() + 1);

    for (const std::string &t : cleanedTokens)
        argv.push_back(t.c_str());
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid == 0)
    {
        if (redirect) {
            int fd = open(outFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("open");
                _exit(1);
            }

            if (dup2(fd, STDOUT_FILENO) < 0) {
                perror("dup2");
                close(fd);
                _exit(1);
            }

            close(fd);
        }

        int status = execvp(argv[0], const_cast<char *const *>(argv.data()));

        if (status != 0)
        {
            std::string msg = "failed to execute command";
            if (errno == ENOENT)
                msg = "command not found";

            std::cerr << cleanedTokens[0] << ": " << msg << std::endl;
        }

        _exit(1);
    }
    else if (pid == -1)
    {
        std::cerr << cleanedTokens[0] << ": failed to execute command" << std::endl;
    }
    else
    {
        if (!background)
            waitpid(pid, nullptr, 0);
    }
}

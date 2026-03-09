#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include "fcntl.h"
#include <cerrno>
#include <iostream>
#include <vector>

void Executer::execute(const std::vector<std::string> &tokens)
{
    if (tokens.empty())
        return;

    if (Builtins::handle(tokens))
        return;

    std::vector<std::string> commandTokens = tokens;
    bool runInBackground = false;
    std::string outputFile;

    if (!commandTokens.empty() && commandTokens.back() == "&")
    {
        runInBackground = true;
        commandTokens.pop_back();
    }

    for (size_t i = 0; i < commandTokens.size(); ++i)
    {
        if (commandTokens[i] == ">")
        {
            if (i + 1 >= commandTokens.size())
            {
                std::cerr << "nsh: missing output file for redirection" << std::endl;
                return;
            }

            outputFile = commandTokens[i + 1];
            commandTokens.erase(commandTokens.begin() + i, commandTokens.begin() + i + 2);
            break;
        }
    }

    if (commandTokens.empty())
    {
        std::cerr << "nsh: missing command" << std::endl;
        return;
    }

    std::vector<const char *> argv;

    for (const std::string &token : commandTokens)
        argv.push_back(token.c_str());
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) // fork failed
        std::cerr << commandTokens[0] << ": failed to execute command" << std::endl;
    else if (pid == 0) // child process
    {
        if (!outputFile.empty())
        {
            int fd = open(outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);

            if (fd < 0)
            {
                std::cerr << outputFile << ": failed to open output file" << std::endl;
                _exit(EXIT_FAILURE);
            }

            if (dup2(fd, STDOUT_FILENO) < 0)
            {
                std::cerr << outputFile << ": failed to redirect output" << std::endl;
                close(fd);
                _exit(EXIT_FAILURE);
            }

            close(fd);
        }

        int status = execvp(argv[0], const_cast<char *const *>(argv.data()));

        if (status != 0)
        {
            std::string msg = "failed to execute command";

            if (errno == ENOENT)
                msg = "command not found";

            std::cerr << commandTokens[0] << ": " << msg << std::endl;
            _exit(EXIT_FAILURE);
        }
    }
    else // parent process (pid > 0)
    {
        if (!runInBackground)
            waitpid(pid, nullptr, 0);
    }
}

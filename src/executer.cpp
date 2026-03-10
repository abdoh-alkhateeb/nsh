#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>
#include <fcntl.h>

void Executer::execute(const std::vector<std::string> &tokens)
{
    if (Builtins::handle(tokens))
        return;

    std::vector<std::string> commandTokens = tokens;
    bool background = false;
    bool redirectOutput = false;
    std::string outputFile;

    if (!commandTokens.empty() && commandTokens.back() == "&")
    {
        background = true;
        commandTokens.pop_back();
    }

    std::vector<std::string> finalTokens;

    for (size_t i = 0; i < commandTokens.size(); ++i)
    {
        if (commandTokens[i] == ">")
        {
            if (i + 1 < commandTokens.size())
            {
                redirectOutput = true;
                outputFile = commandTokens[i + 1];
            }
            break;
        }

        finalTokens.push_back(commandTokens[i]);
    }

    std::vector<const char *> argv;

    for (const std::string &token : finalTokens)
        argv.push_back(token.c_str());
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) // fork failed
       std::cerr << finalTokens[0] << ": failed to execute command" << std::endl;
    else if (pid == 0)
    {
        if (redirectOutput)
        {
            int fd = open(outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);

            if (fd < 0)
            {
                std::cerr << "failed to open output file" << std::endl;
                exit(EXIT_FAILURE);
            }

            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        int status = execvp(argv[0], const_cast<char *const *>(argv.data()));

        if (status != 0)
        {
            std::string msg = "failed to execute command";

            if (errno == ENOENT)
                msg = "command not found";

           std::cerr << finalTokens[0] << ": " << msg << std::endl;
        }
    }
    else // parent process (pid > 0)
    {
        if (!background)
            waitpid(pid, nullptr, 0);
    }
}

#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>
#include <fcntl.h>
#include <algorithm>

void Executer::execute(const std::vector<std::string> &tokens)
{
    if (Builtins::handle(tokens))
        return;
    std::vector<std::string> newTokens = tokens;
    std::string outputFile;
    bool redirect = false;

    for (size_t i = 0; i < newTokens.size(); ++i)
    {
       if (newTokens[i] == ">")
       {
          if (i + 1 < newTokens.size())
          {
            redirect = true;
            outputFile = newTokens[i + 1];

            newTokens.erase(newTokens.begin() + i,
                            newTokens.begin() + i + 2);
          }
          break;
       }
    }
    std::vector<const char *> argv;

    for (const std::string &token : newTokens)
        argv.push_back(token.c_str());
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid == 0)
    {
        if (redirect)
        {
           int fd = open(outputFile.c_str(),
                         O_CREAT | O_WRONLY | O_TRUNC,
                         0644);

         if (fd < 0)
         {
            std::cerr << "Failed to open file" << std::endl;
            exit(1);
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

            std::cerr << tokens[0] << ": " << msg << std::endl;
        }
    }
    else if (pid == -1)
        std::cerr << tokens[0] << ": failed to execute command" << std::endl;
    else
        waitpid(pid, nullptr, 0);
}

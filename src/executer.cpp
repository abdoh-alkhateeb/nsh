#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>
#include <fcntl.h> // for open()
void Executer::execute(const std::vector<std::string> &tokens)
{
    if (Builtins::handle(tokens))
        return;

    std::vector<const char *> argv;
    std::string outputFile = "";
    bool runInBackground = false; // 1. Flag for background execution

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        if (tokens[i] == ">")
        {
            if (i + 1 < tokens.size()) {
                outputFile = tokens[i + 1]; 
            }
            break; 
        }
        else if (tokens[i] == "&")
        {
            // 2. Set the flag to true if we see '&'
            runInBackground = true;
            continue; // Skip adding '&' to argv
        }
        
        argv.push_back(tokens[i].c_str());
    }
    argv.push_back(nullptr);

    // If argv is empty (e.g., user just typed "&"), safely return
    if (argv.size() == 1) return;

    pid_t pid = fork();

    if (pid < 0) // fork failed
        std::cerr << tokens[0] << ": failed to execute command" << std::endl;
    else if (pid == 0) // child process
    {
        if (!outputFile.empty())
        {
            int fd = open(outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd >= 0) {
                dup2(fd, STDOUT_FILENO);
                close(fd);
            } else {
                std::cerr << "nsh: failed to open file" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        int status = execvp(argv[0], const_cast<char *const *>(argv.data()));

        if (status != 0)
        {
            std::string msg = "failed to execute command";
            if (errno == ENOENT)
                msg = "command not found";

            std::cerr << tokens[0] << ": " << msg << std::endl;
            exit(EXIT_FAILURE); 
        }
    }
    else // parent process (pid > 0)
    {
        // 3. Only wait for the child if runInBackground is false!
        if (!runInBackground) 
        {
            waitpid(pid, nullptr, 0);
        }
    }
}
#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>
#include "fcntl.h"
#include <cstring>

void Executer::execute(const std::vector<std::string> &tokens)
{
    // If the command is valid, you may leave :) (the command is excecuted by the builtins.cpp file)
    if (Builtins::handle(tokens))
        return;

    std::vector<const char *> argv;

    
    int stdo = dup(STDOUT_FILENO);
    int fd;
    bool file_opened = false;
    // Copying the contents of the token vector to argv vector
    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens.at(i) == ">") {
            // Opening a file with the inputted file name, file can be created if it doesn't exist
            // 0644 means that the owner can read/write, group can read only, and other users can read only
            fd = open(tokens.at(i + 1).c_str(), O_WRONLY | O_CREAT, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
            file_opened = true;
            break;
        }
        else argv.push_back((tokens.at(i)).c_str());
    } 

    argv.push_back(nullptr);
    // pid_t is used to store process IDs, fork is used to return an id to the process
    pid_t pid = fork();

    if (pid < 0) // fork failed
        std::cerr << tokens[0] << ": failed to execute command" << std::endl;
    else if (pid == 0) // child process
    {
        // executing the command.
        int status = execvp(argv[0], const_cast<char *const *>(argv.data()));

        if (status != 0)
        {
            std::string msg = "failed to execute command";

            if (errno == ENOENT)
                msg = "command not found";

            std::cerr << tokens[0] << ": " << msg << std::endl;
        }
        if (file_opened) {
            dup2(stdo, STDOUT_FILENO);
        }

    }
    else // parent process (pid > 0) 
    {
        waitpid(pid, nullptr, 0);

        // We need to redirect the output stream to stdout for the parent process as well, so that nsh> does not get printed in the text file!!!! (took me hours to learn this)
        // If I had opened the file in the child process, this wouldn't have happened
        if (file_opened) {
            dup2(stdo, STDOUT_FILENO);
        }
    }
    
    
}

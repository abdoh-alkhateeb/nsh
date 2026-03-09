#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>
#include <fcntl.h>

void Executer::execute(const std::vector<std::string> &tokens) {
    if (Builtins::handle(tokens))
        return;

    std::vector<const char *> argv;

    for (const std::string &token : tokens)
        argv.push_back(token.c_str());
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) // fork failed
        std::cerr << tokens[0] << ": failed to execute command" << std::endl;
    else if (pid == 0) // child process
    {   
        bool redirect = false;
        int index;
        for(int i=0; i<argv.size() && !redirect; i++){
            if(argv[i][0] == '>') {
                redirect = true;
                index = i;
            }
        }
        int status;
        if(!redirect)
            status = execvp(argv[0], const_cast<char *const *>(argv.data()));
        else{
            std::vector<const char *> argv_copy;
            for (int i=0; i<index; i++)
                argv_copy.push_back(argv[i]);
            argv_copy.push_back(nullptr);
            status = execvp(argv[0], const_cast<char *const *>(argv_copy.data()));
            int fd = open(argv[index+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(fd, STDOUT_FILENO);
        }

        if (status != 0)
        {
            std::string msg = "failed to execute command";

            if (errno == ENOENT)
                msg = "command not found";

            std::cerr << tokens[0] << ": " << msg << std::endl;
        }
    }
    else // parent process (pid > 0)
        waitpid(pid, nullptr, 0);
}

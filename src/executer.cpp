#include "executer.hpp"
#include "builtins.hpp"
#include <fcntl.h>
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>

void Executer::execute(std::vector<std::string> tokens)
{
    if (Builtins::handle(tokens))
        return;

    std::vector<const char *> argv;

    for (const std::string &token : tokens)
        argv.push_back(token.c_str());
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0){ // fork failed
        std::cerr << tokens[0] << ": failed to execute command" << std::endl;}
    else if (pid == 0) // child process
    {
for (int i=0; i < tokens.size();i++){
     if(tokens[i] == ">"){
          int fd = open(tokens[i+1].c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
          dup2(fd, STDOUT_FILENO);
          close(fd);
          tokens.resize(i);
          break;
}}
char* argv[tokens.size() + 1];
for (int i=0; i <tokens.size();i++)
{
argv[i]= &tokens[i][0];
}
argv[tokens.size()] = nullptr;

   execvp(argv[0],argv);}
        int status;
        if (status != 0)
        {
            std::string msg = "failed to execute command";

            if (errno == ENOENT)
                msg = "command not found";

            std::cerr << tokens[0] << ": " << msg << std::endl;
        }
     else{ // parent process (pid > 0)
        waitpid(pid, nullptr, 0);
}
}

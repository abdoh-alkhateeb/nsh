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

    std::vector<const char *> argv;
bool gt_exists=false;
bool amp_exists=false;
    for (const std::string &token : tokens)
{if(token=="&"){
amp_exists=true;
break;}
if(token==">"){
gt_exists=true;
break;}
  argv.push_back(token.c_str());

}

    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) // fork failed
        std::cerr << tokens[0] << ": failed to execute command" << std::endl;
    else if (pid == 0) // child process
    {
if(gt_exists){	
int fd = open(tokens[2].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644); 
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
    else { // parent process (pid > 0)
      if(!amp_exists)  waitpid(pid, nullptr, 0);
}}

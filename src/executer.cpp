#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>

void Executer::execute(const std::vector<std::string> &tokens)
{

int redirectIndex = -1;

for(int i = 0; args[i] != NULL; i++){
	 if(string(args[i]) == ">"){
	 redirectIndex = i;
	 break;
	 }
}

if(redirectIndex != -1){
	 int fd = open(args[redirectIndex + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);

	 dup2(fd, STDOUT_FILENO);
	 close(fd);

	 args[redirectIndex] = NULL;
}

bool background = false;

for(int i = 0; args[i] != NULL; i++){
	 if(args[i+1] == NULL && string(args[i]) == "&"){
	 background = true;
	 args[i] = NULL;
	 }
}
    if (Builtins::handle(tokens))
        return;

    std::vector<const char *> argv;

    for (const std::string &token : tokens)
        argv.push_back(token.c_str());
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid == 0)
    {
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

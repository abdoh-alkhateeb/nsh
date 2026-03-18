#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>
#include <fstream>

void Executer::execute(const std::vector<std::string> &tokens, std::string* his, int size, int x)
{
    if (Builtins::handle(tokens, his, size))
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
        int status = execvp(argv[0], const_cast<char *const *>(argv.data()));

	for(int i=0; i< x; i++)
	{
		if(*argv[i]=='>')
		{	std::cout << "found";
			std::ofstream file(argv[i+1]);
			//dup2(1, file);
		}
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
	for(int i=0; i<x; i++){
		 if(!*argv[i]=='&')
                {
			waitpid(pid, nullptr, 0);
                }
	}
}

#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <iostream>
#include <fcntl.h>


void Executer::execute (std::vector<std::string> &tokens)
{
    if (Builtins::handle(tokens))
        return;

    std::vector<const char *> argv;

    for (const std::string &token : tokens)
        argv.push_back(token.c_str());
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid == 0)
    { 
	for(size_t i = 0; i < tokens.size(); i++) {

		if (tokens[i] == ">") {

		int fd = open(tokens[i+1].c_str(), O_WRONLY | O_CREAT | O_TRUNC , 0644);

			dup2(fd , STDOUT_FILENO);

			close(fd);

			tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
 
			break; 

		}

	}



	std::vector<const char*> argv;

	for (const std::string &token : tokens) {

		argv.push_back(token.c_str());  }

		argv.push_back(nullptr);



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

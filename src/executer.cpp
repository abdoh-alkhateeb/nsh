#include <fcntl.h>
#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>

void Executer::execute(const std::vector<std::string> &tokens)
{
    if (Builtins::handle(tokens)) {return;}

	std::vector<std::string> args = tokens;

	bool background = false;

	if (!args.empty() && args.back() == "&")
	{
    		background = true;
    		args.pop_back();
	}

	std::string outputFile;

	for (size_t i = 0; i < args.size(); i++)
	{
    		if (args[i] == ">")
    		{
       	 		outputFile = args[i + 1];
       	 		args.resize(i);
       	 		break;
   	 	}
	}

    std::vector<const char *> argv;

    for (const std::string &token : args)
        argv.push_back(token.c_str());
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) // fork failed
        std::cerr << tokens[0] << ": failed to execute command" << std::endl;
    else if (pid == 0) // child process
    {
	if (!outputFile.empty())
	{
    		int fd = open(outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
    else // parent process (pid > 0)
    	{
    		if (!background)
        	waitpid(pid, nullptr, 0);
	}
}

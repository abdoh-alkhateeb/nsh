#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

void Executer::execute(const std::vector<std::string> &tokens)
{
    if (tokens.empty())
        return;
    if (Builtins::handle(tokens)) return;
    int out_fd = -1;
    std::vector<const char *> argv;

	for (size_t i = 0; i < tokens.size(); i++)
	{
		if(tokens[i] == ">")
		{
			out_fd = open(tokens[i+1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
			break;
		}
		argv.push_back(tokens[i].c_str());
	}
    argv.push_back(nullptr);
    pid_t pid = fork();

    if (pid < 0) // fork failed
        perror("fork");
    else if (pid == 0) // child process
    {
	if (out_fd != -1)
	{
		dup2(out_fd, 1);
		close(out_fd);
	}
	execvp(argv[0], (char**)argv.data());
	perror("execvp");
	exit(1);
     }
    else // parent process (pid > 0)
    {
        waitpid(pid, nullptr, 0);
	if(out_fd != -1) close(out_fd);
    }
}



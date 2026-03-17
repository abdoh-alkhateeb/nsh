#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>

void Executer::execute(const std::vector<std::string> &tokens)
{
    if (Builtins::handle(tokens))
        return;

    // We will build a new argv that doesn't include the redirection symbols
    std::vector<const char *> argv;
    std::string outputFile = "";
    bool redirecting = false;

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == ">") {
            if (i + 1 < tokens.size()) {
                outputFile = tokens[i + 1];
                redirecting = true;
                break; // Stop adding to argv; everything after ">" is for redirection
            }
        }
        argv.push_back(tokens[i].c_str());
    }
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << tokens[0] << ": failed to execute command" << std::endl;
    } 
    else if (pid == 0) { // child process
        if (redirecting) {
            // O_WRONLY: Write only
            // O_CREAT: Create file if it doesn't exist
            // O_TRUNC: Clear the file if it already exists
            // 0644: Standard permissions (Read/Write for owner, Read for others)
            int fd = open(outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            
            if (fd < 0) {
                perror("open");
                exit(1);
            }

            // Redirect STDOUT (1) to our file descriptor (fd)
            if (dup2(fd, STDOUT_FILENO) < 0) {
                perror("dup2");
                exit(1);
            }
            close(fd); // Close the original descriptor as it's no longer needed
        }

        execvp(argv[0], const_cast<char *const *>(argv.data()));
        perror("execvp failed");
        exit(1);
    } 
    else { // parent process
        waitpid(pid, nullptr, 0);
    }
}

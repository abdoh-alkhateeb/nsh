#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>

void Executer::execute(const std::vector<std::string> &tokens)
{
    if (tokens.empty()) return;
    if (Builtins::handle(tokens)) return;

    std::vector<const char *> argv;
    std::string outputFile = "";
    bool redirecting = false;
    bool background = false; // Add this flag

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == ">") {
            if (i + 1 < tokens.size()) {
                outputFile = tokens[i + 1];
                redirecting = true;
                break; 
            }
        }
        // Check for & at the end
        if (tokens[i] == "&") {
            background = true;
            break; // Stop adding to argv
        }
        argv.push_back(tokens[i].c_str());
    }
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed" << std::endl;
    } 
    else if (pid == 0) { // Child
        if (redirecting) {
            int fd = open(outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd >= 0) {
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
        }
        execvp(argv[0], const_cast<char *const *>(argv.data()));
        exit(1);
    } 
    else { // Parent
        if (background) {
            // Do NOT wait. Just print the PID and move on.
            std::cout << "[Process running in background: " << pid << "]" << std::endl;
        } else {
            // Wait normally for foreground processes
            waitpid(pid, nullptr, 0);
        }
    }
}

#include "executer.hpp"
#include "builtins.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <fcntl.h>      // for open flags
#include <errno.h>      // for errno
#include <string.h>     // for strerror (optional)

void Executer::execute(const std::vector<std::string> &tokens)
{
    // If it's a built‑in, handle it and return
    if (Builtins::handle(tokens))
        return;

    // Prepare to build the argument list for execvp,
    // and detect redirection and background operators.
    std::vector<const char*> argv;   // for execvp (null‑terminated)
    std::string outFile;              // file for redirection, if any
    bool background = false;

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == ">") {
            // Next token must be the output file
            if (i + 1 < tokens.size()) {
                outFile = tokens[i + 1];
                i++;  // skip the filename
            } else {
                std::cerr << "syntax error: expected filename after '>'" << std::endl;
                return;
            }
        }
        else if (tokens[i] == "&") {
            // Background operator – usually at the end, but we just set a flag
            background = true;
            // Do not add '&' to argv
        }
        else {
            // Normal argument
            argv.push_back(tokens[i].c_str());
        }
    }
    argv.push_back(nullptr);   // execvp expects a null‑terminated array

    // Fork a child process
    pid_t pid = fork();

    if (pid < 0) {
        // Fork failed
        std::cerr << tokens[0] << ": failed to fork" << std::endl;
        return;
    }
    else if (pid == 0) {
        // --- Child process ---
        // If output redirection is requested, set it up
        if (!outFile.empty()) {
            // Open the file: write only, create if not exists, truncate if exists
            int fd = open(outFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                std::cerr << tokens[0] << ": failed to open output file " << outFile
                          << " (" << strerror(errno) << ")" << std::endl;
                _exit(1);
            }
            // Redirect stdout to the file
            if (dup2(fd, STDOUT_FILENO) < 0) {
                std::cerr << tokens[0] << ": dup2 failed" << std::endl;
                _exit(1);
            }
            close(fd);
        }

        // Execute the command
        int status = execvp(argv[0], const_cast<char* const*>(argv.data()));
        // If we reach here, execvp failed
        std::string msg = "failed to execute command";
        if (errno == ENOENT)
            msg = "command not found";
        std::cerr << tokens[0] << ": " << msg << std::endl;
        _exit(1);
    }
    else {
        // --- Parent process ---
        if (background) {
            // Background job: do not wait, just print the PID (optional)
            std::cout << "[" << pid << "] " << tokens[0] << " (background)" << std::endl;
            // Note: The child will become a zombie when it terminates.
            // A real shell would set up a SIGCHLD handler to reap it.
        } else {
            // Foreground job: wait for it to finish
            waitpid(pid, nullptr, 0);
        }
    }
}

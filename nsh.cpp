#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>

int main() {
    std::string line;
    while (std::cout << "nsh> " && std::getline(std::cin, line)) {
        if (line == "exit") break;
        
        std::vector<char*> args;
        std::stringstream ss(line);
        std::string temp;
        bool background = false;

        while (ss >> temp) {
            if (temp == "&") {
                background = true;
            } else {
                args.push_back(strdup(temp.c_str()));
            }
        }
        args.push_back(NULL);

        if (args[0] == NULL) continue;

        pid_t pid = fork();
        if (pid == 0) { // Child process
            for (int i = 0; args[i] != NULL; i++) {
                if (strcmp(args[i], ">") == 0) {
                    int fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                    args[i] = NULL;
                    break;
                }
            }
            execvp(args[0], args.data());
            perror("execvp failed");
            exit(1);
        } else { // Parent process
            if (!background) waitpid(pid, NULL, 0);
        }
    }
    return 0;
}https://github.com/juju0817/nsh.git

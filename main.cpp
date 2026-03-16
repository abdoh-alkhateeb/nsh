#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

using namespace std;

int main() {
    char command[1024];

    while (true) {
        cout << "shell> ";
        cin.getline(command, 1024);
        if (strlen(command) == 0) continue;

        char* args[100];
        int i = 0;
        char* token = strtok(command, " ");
        while (token) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        pid_t pid = fork();
        if (pid == 0) {
            for (int j = 0; args[j]; j++) {
                if (strcmp(args[j], ">") == 0) {
                    int fd = open(args[j + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                    args[j] = NULL;
                    break;
                }
            }
            execvp(args[0], args);
            perror("execvp failed");
            exit(1);
        } else {
            wait(NULL);
        }
    }
    return 0;
}

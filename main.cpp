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

        bool background = false;
        for (int j = 0; args[j]; j++) {
            if (strcmp(args[j], "&") == 0) {
                background = true;
                args[j] = NULL;
                break;
            }
        }

        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args);
            perror("execvp failed");
            exit(1);
        } else {
            if (!background) wait(NULL);
        }
    }
    return 0;
}

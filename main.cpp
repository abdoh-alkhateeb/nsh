#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

using namespace std;

int main() {

    char command[100];

    while (true) {

        cout << "shell> ";
        cin.getline(command, 100);

        char* args[10];
        int i = 0;

        char* token = strtok(command, " ");

        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }

        args[i] = NULL;

        bool background = false;

        for (int j = 0; args[j] != NULL; j++) {
            if (strcmp(args[j], "&") == 0) {
                background = true;
                args[j] = NULL;
                break;
            }
        }

        pid_t pid = fork();

        if (pid == 0) {
            execvp(args[0], args);
        }
        else {
            if (!background) {
                wait(NULL);
            }
        }
    }

    return 0;
}

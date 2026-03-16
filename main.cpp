#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>
#include <string>

using namespace std;

int main() {
    vector<string> history;
    char command[1024];

    while (true) {
        cout << "shell> ";
        cin.getline(command, 1024);
        if (strlen(command) == 0) continue;

        history.push_back(command);

        char* args[100];
        int i = 0;
        char* token = strtok(command, " ");
        while (token) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (strcmp(args[0], "history") == 0) {
            for (int j = 0; j < history.size(); j++)
                cout << j + 1 << " " << history[j] << endl;
            continue;
        }

        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args);
            perror("execvp failed");
            exit(1);
        } else {
            wait(NULL);
        }
    }
    return 0;
}

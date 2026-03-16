#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int main() {
    string input;

    while (true) {
        cout << "shell> ";
        getline(cin, input);

        if (input == "exit")
            break;

        string command;
        string outfile;

        size_t pos = input.find(">");

        if (pos != string::npos) {
            command = input.substr(0, pos);
            outfile = input.substr(pos + 1);
            while (!outfile.empty() && outfile[0] == ' ')
                outfile.erase(0,1);
        } else {
            command = input;
        }

        stringstream ss(command);
        vector<string> args;
        string temp;

        while (ss >> temp)
            args.push_back(temp);

        vector<char*> argv;

        for (auto &arg : args)
            argv.push_back(&arg[0]);

        argv.push_back(nullptr);

        pid_t pid = fork();

        if (pid == 0) {

            if (!outfile.empty()) {
                int fd = open(outfile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            execvp(argv[0], argv.data());
            exit(1);

        } else {
            wait(NULL);
        }
    }

    return 0;
}

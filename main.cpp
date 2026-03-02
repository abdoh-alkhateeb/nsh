#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    std::string line;
    std::vector<std::string> history;

    while (true) {
        std::cout << "$ ";
        std::getline(std::cin, line);
        if (line.empty()) continue;
        history.push_back(line);

        // Check for exit
        if (line == "exit") break;

        // Output redirection
        std::string cmd = line;
        std::string outfile;
        size_t pos = line.find(">");
        if (pos != std::string::npos) {
            cmd = line.substr(0, pos);
            outfile = line.substr(pos + 1);
            outfile.erase(0, outfile.find_first_not_of(" "));
        }

        // Tokenize command
        std::istringstream iss(cmd);
        std::vector<char*> args;
        std::string token;
        while (iss >> token) {
            char* arg = new char[token.size() + 1];
            std::strcpy(arg, token.c_str());
            args.push_back(arg);
        }
        args.push_back(nullptr);

        pid_t pid = fork();
        if (pid == 0) {
            if (!outfile.empty()) {
                freopen(outfile.c_str(), "w", stdout);
            }
            execvp(args[0], args.data());
            perror("execvp failed");
            exit(1);
        } else {
            wait(nullptr);
        }

        // Free memory
        for (auto arg : args) delete[] arg;
    }

    return 0;
}


bool background = false;
if (!cmd.empty() && cmd.back() == '&') {
    background = true;
    cmd.pop_back(); // Remove &
}
pid_t pid = fork();
if (pid == 0) {
    if (!outfile.empty()) {
        freopen(outfile.c_str(), "w", stdout);
    }
    execvp(args[0], args.data());
    perror("execvp failed");
    exit(1);
} else {
    if (!background) wait(nullptr);
}
if (line == "history") {
    for (size_t i = 0; i < history.size(); ++i) {
        std::cout << i + 1 << " " << history[i] << std::endl;
    }
    continue;
}

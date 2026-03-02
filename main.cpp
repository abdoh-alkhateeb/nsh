#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

int main() {
    std::string line;
    std::vector<std::string> history;

    while (true) {
        std::cout << "$ ";
        std::getline(std::cin, line);
        if (line.empty()) continue;
        history.push_back(line);

        if (line == "exit") break;
        if (line == "history") {
            for (size_t i = 0; i < history.size(); ++i)
                std::cout << i + 1 << " " << history[i] << std::endl;
            continue;
        }

        bool background = false;
        size_t amp = line.find('&');
        if (amp != std::string::npos) {
            background = true;
            line.erase(amp);
        }

        std::string cmd = line;
        std::string outfile;
        size_t pos = line.find('>');
        if (pos != std::string::npos) {
            cmd = line.substr(0, pos);
            outfile = line.substr(pos + 1);
            outfile.erase(0, outfile.find_first_not_of(" "));
        }

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
            if (!background) wait(nullptr);
        }

        for (auto arg : args) delete[] arg;
    }
    return 0;
}

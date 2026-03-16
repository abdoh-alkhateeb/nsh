#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

int main() {
    std::vector<std::string> history;
    std::string line;

    while (true) {
        std::cout << "myshell> ";
        std::getline(std::cin, line);
        if (line.empty()) continue;
        history.push_back(line);

        if (line == "exit") break;

        if (line == "history") {
            for (size_t i = 0; i < history.size(); i++)
                std::cout << i+1 << " " << history[i] << std::endl;
            continue;
        }

        bool background = false;
        if (line.back() == '&') {
            background = true;
            line.pop_back();
        }

        std::string cmd = line;
        std::string filename;
        bool redirect = false;
        size_t pos = line.find('>');
        if (pos != std::string::npos) {
            redirect = true;
            cmd = line.substr(0, pos);
            filename = line.substr(pos + 1);
            filename.erase(0, filename.find_first_not_of(" "));
        }

        pid_t pid = fork();
        if (pid == 0) {
            if (redirect) {
                std::ofstream out(filename);
                std::cout.rdbuf(out.rdbuf());
            }
            system(cmd.c_str());
            exit(0);
        } else {
            if (!background) waitpid(pid, nullptr, 0);
        }
    }

    return 0;
}

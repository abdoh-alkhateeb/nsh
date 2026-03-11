#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>
#include <fcntl.h>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <cctype>

Executer::Executer() {}
Executer::~Executer() {}

Command Executer::parseCommand(const std::string& cmd_line) {
    Command cmd;
    std::string current_token;
    bool in_quotes = false;
    bool reading_filename = false;

    std::string tokens = cmd_line; // Work with a copy

    for (size_t i = 0; i < tokens.size(); ++i) {
        char c = tokens[i];

        if (c == '"') {
            in_quotes = !in_quotes;
            continue;
        }

        if (reading_filename) {
            if (!in_quotes && isspace(c)) {
                reading_filename = false;
            } else {
                cmd.output_file += c;
            }
            continue;
        }
        if (!in_quotes && c == '>') {
            if (!current_token.empty()) {
                cmd.args.push_back(current_token);
                current_token.clear();
            }
            reading_filename = true;
            continue;
        }
        if (!in_quotes && isspace(c)) {
            if (!current_token.empty()) {
                cmd.args.push_back(current_token);
                current_token.clear();
            }
        } else {
            current_token += c;
        }
        if (!in_quotes && c == '&') {
            if (!current_token.empty()) {
                cmd.args.push_back(current_token);
                current_token.clear();
            }
            cmd.background = true;
            continue;
        }
    }
    if (!current_token.empty()) {
        cmd.args.push_back(current_token);
    }
    if (!cmd.args.empty()) {
        cmd.name = cmd.args[0];
    }
    return cmd;
}

void Executer::executeCommand(const Command& cmd) {
    if (cmd.name.empty()) return;

    // Convert args to vector of strings for Builtins
    std::vector<std::string> tokens = cmd.args;

    // Handle builtins
    if (Builtins::handle(tokens))
        return;

    // Prepare argv for execvp
    std::vector<const char *> argv;
    for (const std::string &arg : cmd.args)
        argv.push_back(arg.c_str());
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) { // fork failed
        std::cerr << cmd.name << ": failed to execute command" << std::endl;
    } else if (pid == 0) { // child process
        // Handle output redirection
        if (!cmd.output_file.empty()) {
            int fd = open(cmd.output_file.c_str(),
                     O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
                 std::cerr << cmd.name << ": cannot create file "
                         << cmd.output_file << std::endl;
                 exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        int status = execvp(argv[0], const_cast<char *const *>(argv.data()));

        if (status != 0) {
            std::string msg = "failed to execute command";
            if (errno == ENOENT)
                msg = "command not found";
            std::cerr << cmd.name << ": " << msg << std::endl;
            exit(1);
        } else { // Parent process
            if (cmd.background) {
                std::cout << "[" << pid << "] " << cmd.name << std::endl;
            } else {
                int status;
                waitpid(pid, &status, 0);
            }
        }
    }
}

void Executer::execute(const std::string& cmd_line) {
    if (cmd_line.empty()) return;

    Command cmd = parseCommand(cmd_line);
    executeCommand(cmd);
}

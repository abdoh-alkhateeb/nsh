#include "sys/wait.h"
#include <iostream>
#include <vector>
#include <string>
#include <fcntl.h>
#include <unistd.h>

static std::vector<std::string> command_history;

void Executer::execute(const std::vector<std::string> &tokens)
{
    if (tokens.empty()) return;

    std::string full_cmd = "";
    for (const std::string& t : tokens) {
        full_cmd += t + " ";
    }
    command_history.push_back(full_cmd);

    if (tokens[0] == "history") {
        for (size_t i = 0; i < command_history.size(); ++i) {
            std::cout << i + 1 << "  " << command_history[i] << std::endl;
        }
        return;
    }

    if (Builtins::handle(tokens))
        return;

    std::vector<const char *> argv;
    std::string outfile = "";
    
    bool run_in_bg = (tokens.back() == "&");
    size_t token_limit = run_in_bg ? tokens.size() - 1 : tokens.size();

    for (size_t i = 0; i < token_limit; ++i) {
        if (tokens[i] == ">") {
            if (i + 1 < tokens.size()) {
                outfile = tokens[i + 1];
            }
            break;
        }
        argv.push_back(tokens[i].c_str());
    }
    argv.push_back(nullptr);

    if (argv.empty() || argv[0] == nullptr) return; 

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << tokens[0] << ": failed to execute command" << std::endl;
    }
    else if (pid == 0) 
    { 
        if (!outfile.empty()) {
            int fd = open(outfile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd >= 0) {
                dup2(fd, STDOUT_FILENO);
                close(fd);
            } else

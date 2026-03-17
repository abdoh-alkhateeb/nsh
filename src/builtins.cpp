#include "builtins.hpp"
#include "unistd.h"
#include "stdlib.h"
#include <iostream>

static std::vector<std::string> history_list;

bool Builtins::handle(const std::vector<std::string> &tokens) {
    if (tokens.empty()) return false;

    std::string current_cmd = "";
    for (const auto& t : tokens) current_cmd += t + " ";
    history_list.push_back(current_cmd);

    if (tokens[0] == "history") {
        for (size_t i = 0; i < history_list.size(); ++i) {
            std::cout << i + 1 << " " << history_list[i] << std::endl;
        }
        return true; 
    }

    return false;
}

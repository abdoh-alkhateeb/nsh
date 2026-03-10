#pragma once
#include <string>
#include <vector>

class Builtins
{
public:
    static void add_to_history(const std::string& cmd);
    static bool handle(const std::vector<std::string> &tokens);
};

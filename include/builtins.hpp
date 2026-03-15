#pragma once
#include <string>
#include <vector>

class Builtins
{
public:
    static std::string history[10];
    static std::vector<std::string> historyy;
    static bool handle(const std::vector<std::string> &tokens);
};

#pragma once
#include <string>
#include <vector>

class Builtins
{
public:
static std::vector<std::string> history;
    static bool handle(const std::vector<std::string> &tokens);
};

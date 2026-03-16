#pragma once
#include <string>
#include <vector>

class Builtins
{
public:
    static bool handle(const std::vector<std::string> &tokens);
    static void add_history(const std::vector<std::string> &tokens);

private:
    static std::vector<std::string> history;
};

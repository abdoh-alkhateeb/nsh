#pragma once
#include <string>
#include <vector>

class Builtins
{
public:
    static bool handle(const std::vector<std::string> &tokens);
    static void addToHistory(const std::string &cmd);   // new
    static void printHistory();                          // new

private:
    static std::vector<std::string> history;             // new
};

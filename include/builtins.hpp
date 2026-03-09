#pragma once
#include <string>
#include <vector>

class Builtins
{
public:
    static bool handle(const std::vector<std::string> &tokens);
    static void addHistory(const std::string &command);

private:
    static std::vector<std::string> history;
};

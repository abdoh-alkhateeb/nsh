#pragma once
#include <string>
#include <vector>

class Builtins
{
private:
    static std::vector<std::string> history;
public:
    static bool handle(const std::vector<std::string> &tokens);
    static void log(std::string);

};

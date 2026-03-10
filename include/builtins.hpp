#ifndef BUILTINS_HPP
#define BUILTINS_HPP
#include <string>
#include <vector>

class Builtins
{
public:
    static bool handle(const std::vector<std::string> &tokens);
    static void addHistory(const std::string &command);
};

#endif
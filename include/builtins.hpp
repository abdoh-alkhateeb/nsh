#pragma once
#include <string>
#include <vector>

class Builtins
{
public:
    static bool handleCd(const std::vector<std::string> &tokens);

    static bool handle(const std::vector<std::string> &tokens);
};

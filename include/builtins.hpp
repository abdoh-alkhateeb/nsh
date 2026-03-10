#pragma once
#include <string>
#include <vector>
#include "shellMessage.hpp"

class Builtins
{
public:
    static shellMessage handleCd(const std::vector<std::string> &tokens);

    static shellMessage handle(const std::vector<std::string> &tokens);
};

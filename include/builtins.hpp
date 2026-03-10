#pragma once
#include <string>
#include <vector>
#include "shellMessage.hpp"

class Builtins
{
public:
    static std::vector<std::string> history;
    static shellMessage handleCd(const std::vector<std::string> &tokens);

    static shellMessage handleHistory();

    static shellMessage handle(const std::vector<std::string> &tokens);
};

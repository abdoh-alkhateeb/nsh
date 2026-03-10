#pragma once
#include <string>
#include <vector>
#include "shellMessage.hpp"

class Executer
{
public:
    static void execute(const std::vector<std::string> &tokens);
    static shellMessage runCommand(std::vector<const char *> argv);

    static std::vector<std::string> splitVectorBefore(const std::vector<std::string> &vec, int idx);

    static int getOutputRedirectionIdx(const std::vector<std::string> &tokens);

    static std::vector<const char *> stringVectorToCString(const std::vector<std::string> &strVec);
};

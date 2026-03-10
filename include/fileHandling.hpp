#pragma once
#include <string>

class fileHandling {
public:
    static bool writeToFile(const std::string &msg,const std::string fileName,const bool append);
};

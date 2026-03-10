#pragma once
#include <string>
#include <vector>

class History {
public:
    static void add(const std::string &cmd);
    static void show();

private:
    static std::vector<std::string> entries;
};

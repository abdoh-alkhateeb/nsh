#include "history.hpp"
#include <iostream>

std::vector<std::string> History::entries;

void History::add(const std::string &cmd)
{
    entries.push_back(cmd);
}

void History::show()
{
    for (size_t i = 0; i < entries.size(); ++i)
    {
        std::cout << (i + 1) << " " << entries[i] << std::endl;
    }
}

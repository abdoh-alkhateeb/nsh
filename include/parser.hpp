
#pragma once
#include <string>
#include <vector>

class Parser
{
public:
    static std::vector<std::string> parseInput(const std::string &input);
	static std::vector<std::string> getHistory();
};

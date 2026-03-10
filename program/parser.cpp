#include "parser.hpp"
#include <sstream>

std::vector<std::string> Parser::parseInput(const std::string &input)
{
    std::stringstream ss(input);
    std::string token;
    std::vector<std::string> tokens;

    while (ss >> token)
        tokens.push_back(token);

    return tokens;
}

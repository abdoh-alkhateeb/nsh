#include "parser.hpp"
#include <sstream>


// The program splits the input string into several space-separated words and adds them to a vector data structure.
std::vector<std::string> Parser::parseInput(const std::string &input)
{
    std::stringstream ss(input);
    std::string token;
    std::vector<std::string> tokens;

    while (ss >> token)
        tokens.push_back(token);

    return tokens;
}

#include "parser.hpp"
#include <sstream>

std::vector<std::string> input_saved;
std::vector<std::string> Parser::parseInput(const std::string &input)
{
    std::stringstream ss(input);
    std::string token;
    std::vector<std::string> tokens;
    while (ss >> token)
    {
        tokens.push_back(token);
        input_saved.push_back(token);
    }
    input_saved.push_back("\n");
    return tokens;
}


std::vector<std::string> Parser::getHistory()
{
    return input_saved;
}

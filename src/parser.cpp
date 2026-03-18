#include "parser.hpp"
#include <sstream>

std::vector<std::string> Parser::parseInput(const std::string &input, int &i)
{
    std::stringstream ss(input);
    std::string token;
    std::vector<std::string> tokens;

    while (ss >> token){
        tokens.push_back(token);
	i++;}

    return tokens;
}

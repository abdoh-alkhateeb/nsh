#include "parser.hpp"
#include <sstream>

// takes a strings and splits it a vector of strings (tokens) seperated by spaces
std::vector<std::string> Parser::parseInput(const std::string &input) {
    std::stringstream ss(input);
    std::string token;
    std::vector<std::string> tokens;

    while (ss >> token)
        tokens.push_back(token);

    return tokens;
}

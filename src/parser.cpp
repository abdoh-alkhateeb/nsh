#include "parser.hpp"
#include <cctype>

std::vector<std::string> Parser::parseInput(const std::string &input)
{
    std::vector<std::string> tokens;
    std::string token;

    for (char ch : input)
    {
        if (std::isspace(static_cast<unsigned char>(ch)))
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
        }
        else if (ch == '>')
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }

            tokens.push_back(std::string(1, ch));
        }
        else
            token.push_back(ch);
    }

    if (!token.empty())
        tokens.push_back(token);

    return tokens;
}

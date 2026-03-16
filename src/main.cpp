#include "parser.hpp"
#include "executer.hpp"
#include <iostream>

std::vector<std::string> history;

int main()
{
    while (true)
    {
        std::cout << "nsh> ";
        std::string input;
        std::getline(std::cin, input);

        if (input.empty())
            continue;

	history.push_back(input);

        std::vector<std::string> tokens = Parser::parseInput(input);
        Executer::execute(tokens);
    }
}

#include "parser.hpp"
#include "executer.hpp"
#include "builtins.hpp"   // need this for addToHistory
#include <iostream>

int main()
{
    while (true)
    {
        std::cout << "nsh> ";
        std::string input;
        std::getline(std::cin, input);

        if (input.empty())
            continue;

        // Add the raw input to history
        Builtins::addToHistory(input);

        std::vector<std::string> tokens = Parser::parseInput(input);
        Executer::execute(tokens);
    }
}

#include "parser.hpp"
#include "executer.hpp"
#include "history.hpp"
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

        // record history before tokenizing, preserve original string
        History::add(input);

        std::vector<std::string> tokens = Parser::parseInput(input);
        Executer::execute(tokens);
    }
}

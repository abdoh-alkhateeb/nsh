#include "parser.hpp"
#include "executer.hpp"
#include "builtins.hpp"
#include <iostream>

int main()
{
    std::vector<std::string> history;
    while (true)
    {
        std::cout << "nsh> ";
        std::string input;
        std::getline(std::cin, input);
	history.push_back(input);
        if (input.empty())
            continue;

        std::vector<std::string> tokens = Parser::parseInput(input);
	if (Builtins::handle(tokens, history))
		continue;
        Executer::execute(tokens);
    }
}

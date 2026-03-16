#include "parser.hpp"
#include "executer.hpp"
#include <iostream>
#include <vector>
#include <string>

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
        Executer::execute(tokens);
    }
}

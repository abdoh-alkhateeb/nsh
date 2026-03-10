#include "parser.hpp"
#include "executer.hpp"
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

        std::vector<std::string> tokens = Parser::parseInput(input);
        Executer::execute(tokens);
    }
	std::vector<std::string> history;
	history.push_back(input);
	for(int i = 0; i < history.size(); i++)
	std::out << history[i] <<std::endl;

}

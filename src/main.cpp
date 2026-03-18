#include "parser.hpp"
#include "executer.hpp"
#include <iostream>

int main()
{
    std::string his[100];
    int i =0;
    while (true)
    {
        std::cout << "nsh> ";
	int x=0;
        std::string input;
        std::getline(std::cin, input);
	his[i]= input;
	i++;
        if (input.empty())
            continue;

        std::vector<std::string> tokens = Parser::parseInput(input, x);
        Executer::execute(tokens, his, i,x);
    }
}

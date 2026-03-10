#include "parser.hpp"
#include "executer.hpp"
#include <iostream>

// Main program which takes input and calls the functions of the library we made.
// My long, long, long, long convo with ChatGPT https://chatgpt.com/share/69b0099a-a4c0-8006-b898-349ace69e823
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
}

#include "parser.hpp"
#include "executer.hpp"
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

    if (input == "history")
    {
        for (int i = 0; i < history.size(); ++i)
            std::cout << i + 1 << " " << history[i] << std::endl;
        continue;
    }

        std::vector<std::string> tokens = Parser::parseInput(input);
        Executer::execute(tokens);
    }
}

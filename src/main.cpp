#include "parser.hpp"
#include "executer.hpp"
#include <iostream>
#include <vector>

int main()
{
    std::vector<std::string> history;

    while (true)
    {
        std::cout << "nsh> ";
        std::string input;
        std::getline(std::cin, input);

        if (input.empty())
            continue;

        std::vector<std::string> tokens = Parser::parseInput(input);

        if (tokens[0] == "history")
        {
            for (size_t i = 0; i < history.size(); i++)
            {
                std::cout << i + 1 << " " << history[i] << std::endl;
            }
            history.push_back(input);
            continue;
        }

        history.push_back(input);
        Executer::execute(tokens);
    }
}

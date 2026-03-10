#include "parser.hpp"
#include "executer.hpp"
#include <iostream>
#include <vector>
#include <string>

int main()
{
    std::vector<std::string> historyList;

    while (true)
    {
        std::cout << "nsh> ";
        std::string input;
        std::getline(std::cin, input);

        if (input.empty())
            continue;

        historyList.push_back(input);
        if (input == "history")
        {
            for (size_t i = 0; i < historyList.size(); ++i)
                std::cout << i + 1 << " " << historyList[i] << std::endl;

            continue;
        }

        std::vector<std::string> tokens = Parser::parseInput(input);
        Executer::execute(tokens);
    }
}

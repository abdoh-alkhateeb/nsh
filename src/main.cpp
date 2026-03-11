#include "parser.hpp"
#include "executer.hpp"
#include <iostream>
#include "builtins.hpp"

int main()
{
    Executer shell;
    std::string cmd_line;
    while (true)
    {
        std::cout << "nsh> ";
        std::getline(std::cin, cmd_line);

        if (cmd_line.empty())
            continue;

        Builtins::addToHistory(cmd_line);
        shell.execute(cmd_line);
    }
    return 0;
}

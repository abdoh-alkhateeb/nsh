#pragma once
#include <string>
#include <vector>

struct Command {
    std::string name;
    std::vector<std::string> args;
    std::string output_file;
    bool background;
    Command() : background(false) {}
};


class Executer
{
public:
	Executer();
	~Executer();
	void execute(const std::string& cmd_line);

private:
	Command parseCommand(const std::string& tokens);
	void executeCommand(const Command& cmd);
};

#pragma once
#include <vector>
#include <string>
#include <fstream>

//made to help with output redirection, now i store each message with a type to differentiate the streams, this way i can print it back out correctly
// also this helps so i dont lose the ordering of the messages if i were to just use  a string of two strings for each streams if that makes sense
class shellMessage {
public:
    enum Type { Stdout, Stderr };

    struct Msg {
        Type type;
        std::string text;
    };

private:
    std::vector<Msg> messages;

public:
    void addStdout(const std::string &text);
    void addStderr(const std::string &text);

    void print() const;

    bool outputToFile(const std::string &fileName, bool append = false) const;

    void clear();

    std::string getCombined() const;

    bool isEmpty() const;
};
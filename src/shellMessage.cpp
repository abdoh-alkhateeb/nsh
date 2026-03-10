#include "shellMessage.hpp"
#include "fileHandling.hpp"
#include <iostream>

void shellMessage::addStdout(const std::string &text) {
    messages.push_back(Msg{Stdout, text});
}

void shellMessage::addStderr(const std::string &text) {
    messages.push_back(Msg{Stderr, text});
}

void shellMessage::print() const {
    for (const auto &m : messages) {
        if (m.type == Stdout) std::cout << m.text;
        else std::cerr << m.text;
    }
}

// wtrites messages to a file
bool shellMessage::outputToFile(const std::string &fileName, bool append) const {
    std::string combined;
    for (const auto &m : messages) combined += m.text;
    return fileHandling::writeToFile(combined, fileName, append);
}

void shellMessage::clear() {
    messages.clear();
}

std::string shellMessage::getCombined() const {
    std::string combined;
    for (const auto &m : messages) combined += m.text;
    return combined;
}

bool shellMessage::isEmpty() const {
    return messages.empty();
}
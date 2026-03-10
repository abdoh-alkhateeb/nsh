#include "fileHandling.hpp"
#include <fstream>
#include <iostream>

bool fileHandling::writeToFile(const std::string &msg, const std::string fileName,const bool append) {
    std::ofstream outFile;
    outFile.open(fileName, append ? std::ios::app : std::ios::out);

    if (!outFile.is_open()) {
        std::cerr << "failed to open the file\n";
        return false;
    }

    outFile << msg;
    outFile.close();
    return true;
}

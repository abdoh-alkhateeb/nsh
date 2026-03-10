#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>

std::vector<const char *> Executer::stringVectorToCString(const std::vector<std::string> &strVec) {
    std::vector<const char *> cStrVec;
    for (const std::string &str : strVec)
        cStrVec.push_back(str.c_str()); // lifetime of new vector must be same as old vector to avoid nullptr reference as c_str() returns a pointer to original strings internal c string.
    cStrVec.push_back(nullptr);
    return cStrVec;
}

shellMessage Executer::runCommand(std::vector<const char *> argv, bool isBackgroundTask) {
    shellMessage msg; // will store both stdout and stderr

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        msg.addStderr(std::string(argv[0]) + ": failed to create pipe\n");
        return msg;
    }

    pid_t pid = fork();
    if (pid == -1) {
        msg.addStderr(std::string(argv[0]) + ": failed to fork\n");
        return msg;
    }

    if (pid == 0) { // child procccess
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO); // this redirect the stderr and stdout streams
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[1]);

        int status = execvp(argv[0], const_cast<char *const *>(argv.data()));
        if (status != 0) {
            std::string errMsg = "failed to execute command";
            if (errno == ENOENT)
                errMsg = "command not found ";
            std::cerr << argv[0] << ": " << errMsg << std::endl; // child still writes to stderr as text is redirected now
        }
        _exit(1);
    }

    if (isBackgroundTask) { // return early if its a background task and close the pipes so they dont make us wait.
        close(pipefd[0]);
        close(pipefd[1]);
        return msg;
    }

    close(pipefd[1]);
    char buffer[128];
    ssize_t count;

    while ((count = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        msg.addStdout(std::string(buffer, count));
    }

    close(pipefd[0]);

    int status;
    waitpid(pid, &status, 0); // wait for child to finish
    return msg; // return combined stdout and stderr as shellMessage
}

std::vector<std::string> Executer::splitVectorBefore(const std::vector<std::string>& vec, int idx) {
    std::vector<std::string> result;
    if (idx <= 0) return result;
    if (idx > vec.size()) idx = vec.size();

    for (int i = 0; i < idx; i++)
        result.push_back(vec[i]);
    return result;
}


bool Executer::removeBackgroundToken(std::vector<std::string> &tokens) {
    if (!tokens.empty() && tokens.back() == "&") {
        tokens.pop_back();
        return true;
    }
    return false;
}

int Executer::getOutputRedirectionIdx(const std::vector<std::string> &tokens) {
    for (int i=0; i < tokens.size(); ++i)
        if (tokens[i] == ">" || tokens[i] == ">>") return i;
    return -1;
}

void Executer::execute(std::vector<std::string> tokens) {
    int idx = getOutputRedirectionIdx(tokens);
    shellMessage msg;

    // No output redirection
    if (idx <= 0) {
        msg = Builtins::handle(tokens);
        if (!Builtins::isBuiltin(tokens[0])) {
            bool isbackgroundTocken = removeBackgroundToken(tokens);
            auto argv = stringVectorToCString(tokens);
            msg = runCommand(argv, isbackgroundTocken);
        }
        msg.print();
        return;
    }

    // Ensure there is a file after the redirection symbol
    if (idx + 1 >= tokens.size()) {
        shellMessage err;
        err.addStderr("syntax error: no file specified for redirection\n");
        err.print();
        return;
    }

    // auto is equivalent of java var keyword for nicer readability.
    auto splitMessage = splitVectorBefore(tokens, idx);

    msg = Builtins::handle(splitMessage);
    if (!Builtins::isBuiltin(tokens[0])) {
        auto argv = stringVectorToCString(splitMessage);
        msg = runCommand(argv, false);
    }

    // Write output to file
    bool append = tokens[idx] == ">>";
    if (!msg.outputToFile(tokens[idx + 1], append)) std::cout << "Error writing to file\n";
}

#include <array>
#include <memory>
#include <stdexcept>
#include <fstream>
#include <cstdio>
#include "CommandCaller.h"

#if defined __WIN32__
    #define popen _popen
    #define pclose _pclose
#endif

CommandCaller& CommandCaller::getInstance() {
    static CommandCaller instance;

    return instance;
}

std::string CommandCaller::takeValue(const std::string& command) {
    std::array<char, 128> buffer;
    std::string result;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

    if (!pipe) {
        throw std::runtime_error("CommandCaller::takeValue: popen() failed.");
    }
    
    while(fgets(buffer.data(), buffer.size(), pipe.get())) {
        result += buffer.data();
    }
    
    return result;
}

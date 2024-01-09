#include "commandCaller.h"

CommandCaller& CommandCaller::getInstance() {
    static CommandCaller cmd_caller;
    return cmd_caller;
}

std::string CommandCaller::takeValue(std::string command) {
    std::FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return "";
    
    char buffer[8];
    fgets(buffer, 8, pipe);
    pclose(pipe);        
    
    return std::string(buffer);
}

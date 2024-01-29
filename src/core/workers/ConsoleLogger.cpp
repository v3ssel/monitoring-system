#include <sstream>
#include <iostream>

#include "ConsoleLogger.h"

namespace s21 {
    void ConsoleLogger::Log(const std::string &message, LogLevel lvl) {
        std::string slvl = parseLevel(lvl);
        std::stringstream ss;
        ss << slvl << ": " << message << "\n";

        std::cout << ss.str();
    }
    
    std::string ConsoleLogger::parseLevel(LogLevel lvl) {
        switch (lvl) {
            case LogLevel::INFO:
                return "[INFO]";
                break;
            case LogLevel::WARNING:
                return "[WARNING]";
                break;
            case LogLevel::ERROR:
                return "[ERROR]";
                break;
        }

        return std::string();
    }
}

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>

namespace s21 {
    enum class LogLevel {
        INFO,
        WARNING,
        ERROR
    };

    class Logger {
       public:
        virtual ~Logger() = default;
        virtual void setEnabled(bool) = 0;
        virtual void Log(const std::string& message, LogLevel lvl) = 0;
    };
}

#endif  // _LOGGER_H_

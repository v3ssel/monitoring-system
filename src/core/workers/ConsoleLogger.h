#ifndef _CONSOLELOGGER_H_
#define _CONSOLELOGGER_H_

#include "../interfaces/Logger.h"

namespace s21 {
    class ConsoleLogger : public Logger {
       public:
        void Log(const std::string& message, LogLevel lvl) override;

       private:
        std::string parseLevel(LogLevel lvl);
    };
} // namespace s21


#endif  // _CONSOLELOGGER_H_

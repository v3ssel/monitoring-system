#ifndef _CONSOLELOGGER_H_
#define _CONSOLELOGGER_H_

#include "../interfaces/Logger.h"

namespace s21 {
    class ConsoleLogger : public Logger {
       public:
        void setEnabled(bool enabled) override;
        void Log(const std::string& message, LogLevel lvl) override;

       private:
        std::string parseLevel(LogLevel lvl);

        bool enabled_;
    };
} // namespace s21


#endif  // _CONSOLELOGGER_H_

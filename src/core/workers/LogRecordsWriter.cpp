#include <ctime>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "LogRecordsWriter.h"

namespace s21 {
    LogRecordsWriter::LogRecordsWriter(const std::string &logs_directory) {
        logs_directory_ = logs_directory;
    }

    void LogRecordsWriter::write(std::queue<std::string> &records) {
        std::time_t t = std::time(nullptr);
        std::tm* lt = std::localtime(&t);

        char date_buf[10];
        strftime(date_buf, 10, "%y-%m-%d", lt);
        
        std::string log_name = logs_directory_ + "/" + std::string(date_buf) + ".txt";
        std::ofstream log(log_name, std::ios::app);

        if (log.is_open()) {
            while (!records.empty()) {
                std::stringstream ss;
                
                ss << std::put_time(lt, "[%y-%m-%d %H:%M:%S] | ")
                   << records.front()
                   << std::endl;

                log << ss.str();
                records.pop();
            }
        }
    }
    
    void LogRecordsWriter::setLogsDirectory(const std::string &logs_directory) {
        logs_directory_ = logs_directory;
    }
}

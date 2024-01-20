#ifndef _LOGRECORDSWRITER_H_
#define _LOGRECORDSWRITER_H_

#include "RecordsWriter.h"

namespace s21 {
    class LogRecordsWriter : public RecordsWriter {
       public:
        LogRecordsWriter() = default;
        LogRecordsWriter(const std::string& logs_directory);

        void write(std::queue<std::string>& records) override;
        void setLogsDirectory(const std::string& logs_directory) override;

       private:
        std::string logs_directory_;
    };
}

#endif  // _LOGRECORDSWRITER_H_

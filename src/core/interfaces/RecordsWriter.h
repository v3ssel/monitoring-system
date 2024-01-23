#ifndef _RECORDSWRITER_H_
#define _RECORDSWRITER_H_

#include <queue>
#include <string>

namespace s21 {
    class RecordsWriter {
       public:
        virtual ~RecordsWriter() = default;
        virtual void write(std::queue<std::string>&) = 0;
        virtual void setLogsDirectory(const std::string&) = 0;
    };
}

#endif  // _RECORDSWRITER_H_

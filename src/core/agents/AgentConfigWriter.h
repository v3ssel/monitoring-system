#ifndef _AGENTCONFIGWRITER_H_
#define _AGENTCONFIGWRITER_H_

#include <string>

namespace s21 {
    class AgentConfigWriter {
       public:
        static void write(const std::string& filename,
                          const std::string& metric_name,
                          const std::string& compare_sign,
                          const std::string& metric_value);
    };
}

#endif  // _AGENTCONFIGWRITER_H_

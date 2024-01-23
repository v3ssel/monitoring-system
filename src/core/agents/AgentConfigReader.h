#ifndef _AGENTCONFIGREADER_H_
#define _AGENTCONFIGREADER_H_

#include <string>
#include "../interfaces/ConfigReader.h"

namespace s21 {
    class AgentConfigReader : public ConfigReader {
       public:
        AgentConfigReader(Agent* agent);
        void read(const std::string &path) override;
        void setComparisonsAndCriticals(size_t op_index, const std::string &type, const std::string& line);
    };
    
} // namespace s21


#endif  // _AGENTCONFIGREADER_H_

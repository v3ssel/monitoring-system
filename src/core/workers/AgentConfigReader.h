#ifndef _AGENTCONFIGREADER_H_
#define _AGENTCONFIGREADER_H_

#include <string>

namespace s21 {
    class Agent;

    class AgentConfigReader {
       public:
        AgentConfigReader();
        AgentConfigReader(Agent* agent);
        void read(const std::string &path);
        void setComparisonsAndCriticals(size_t op_index, const std::string &type, const std::string& line);

       private:
        Agent* agent_;
    };
    
} // namespace s21


#endif  // _AGENTCONFIGREADER_H_

#ifndef __AGENT_H__
#define __AGENT_H_

#include <string>

namespace s21 {    
    class Agent {
    public:
        bool is_active;
        int update_time;
        
        virtual void readConfig(std::string file_name) = 0;
        virtual void analyzeSystem() = 0;
        virtual std::string toString() = 0;
    };
}

#endif

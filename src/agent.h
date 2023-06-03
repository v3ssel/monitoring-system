#ifndef __AGENT_H__
#define __AGENT_H_

#include <string>

namespace s21 {    
    class Agent {
    public:
        virtual void analyzeSystem() = 0;
        virtual std::string toString() = 0;
    };
}

#endif

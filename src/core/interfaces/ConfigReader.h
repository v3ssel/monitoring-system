#ifndef _CONFIGREADER_H_
#define _CONFIGREADER_H_

#include <string>

namespace s21 {
    class Agent;

    class ConfigReader {
       public:
        ConfigReader(Agent* agent) { agent_ = agent; }
        virtual ~ConfigReader() {}
        
        virtual void read(const std::string&) = 0;
        
       protected:
        Agent* agent_;
    };
} // namespace s21


#endif  // _CONFIGREADER_H_

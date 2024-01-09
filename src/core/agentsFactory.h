#ifndef __AGENTS_FACTORY_H__
#define __AGENTS_FACTORY_H__

#if defined __APPLE__
    #include <dlfcn.h>
#elif defined __WIN32__
    #include <windows.h>
#endif

#include <functional>
#include <memory>
#include <stdexcept>

#include "agent.h"

namespace s21 {
    class AgentsFactory {
       public:
        static AgentsFactory& GetInstance();
        AgentsFactory(const AgentsFactory& af) = delete;   
        AgentsFactory& operator=(const AgentsFactory& af) = delete;     

        std::shared_ptr<Agent> GetAgent(const std::string& name);

       private:
        AgentsFactory() = default;
        ~AgentsFactory() = default;
    };
}

#endif  // __AGENTS_FACTORY_H__

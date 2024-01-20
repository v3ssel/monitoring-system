#ifndef _AGENTS_FACTORY_H_
#define _AGENTS_FACTORY_H_

#if defined __APPLE__
    #include <dlfcn.h>
#elif defined __WIN32__
    #include <windows.h>
#endif

#include <functional>
#include <memory>
#include <stdexcept>

#include "../Agent.h"

namespace s21 {
    class AgentsFactory {
       public:
        static AgentsFactory& GetInstance();
        AgentsFactory(const AgentsFactory& af) = delete;   
        AgentsFactory& operator=(const AgentsFactory& af) = delete;     

        std::shared_ptr<Agent> LoadAgent(const std::string& name);

       private:
        AgentsFactory() = default;
        ~AgentsFactory() = default;
    };
}

#endif  // _AGENTS_FACTORY_H_

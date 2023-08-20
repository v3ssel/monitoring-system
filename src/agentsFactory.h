#ifndef __AGENTS_FACTORY_H__
#define __AGENTS_FACTORY_H__

#include <dlfcn.h>
#include <functional>
#include <memory>
#include <stdexcept>

#include "agent.h"

namespace s21 {
    class AgentsFactory {
       public:
        AgentsFactory() = delete;
        AgentsFactory(std::string library);
        AgentsFactory(const AgentsFactory& af) = delete;
        ~AgentsFactory() = default;

        std::unique_ptr<Agent>& GetAgent();

       private:
        std::unique_ptr<Agent> instance_;
    };
}

#endif  // __AGENTS_FACTORY_H__

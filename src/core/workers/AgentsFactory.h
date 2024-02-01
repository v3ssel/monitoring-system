#ifndef _AGENTS_FACTORY_H_
#define _AGENTS_FACTORY_H_

#include <memory>
#include "../agents/Agent.h"

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

#ifndef _AGENTSSEARCHER_H_
#define _AGENTSSEARCHER_H_

#include <string>
#include <unordered_map>
#include "../interfaces/SearcherObserver.h"
#include "../agents/Agent.h"

namespace s21 {
    class AgentsSearcher {
        public:
         AgentsSearcher(SearcherObserver* observer,
                        const std::string& search_directory,
                        const std::string& configs_directory);

         void search();
         std::shared_ptr<Agent>& getAgent(const std::string& agent_name);
         void clearAgents();

         void setSearchDirectory(const std::string& directory);
         void setConfigDirectory(const std::string& directory);

        private:
         std::unordered_map<std::string, std::shared_ptr<Agent>> agents_;
  
         std::string agents_directory_;
         std::string configs_directory_;
         SearcherObserver* observer_;
    };
}

#endif  // _AGENTSSEARCHER_H_

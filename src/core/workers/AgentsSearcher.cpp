#include <filesystem>
#include "AgentsFactory.h"
#include "AgentsSearcher.h"

namespace s21 {
AgentsSearcher::AgentsSearcher(SearcherObserver* observer,
                               const std::string &search_directory,
                               const std::string &configs_directory) {
    observer_ = observer;
    agents_directory_ = search_directory;
    configs_directory_ = configs_directory;
}

void AgentsSearcher::search() {
    for (auto &agent : std::filesystem::directory_iterator(agents_directory_)) { 
        if (agent.path().extension() != ".so") continue;

        std::string new_agent_name = agent.path().filename().string();

        if (agents_.find(new_agent_name) != agents_.end()) {
            agents_[new_agent_name]->readConfig(configs_directory_);

            continue;
        }
        
        auto created = agents_.emplace(new_agent_name, AgentsFactory::GetInstance().LoadAgent(agent.path().string()));
        if (!created.second) continue;

        observer_->NotifyNewAgentLoaded(new_agent_name);
    }
}

std::shared_ptr<Agent> &AgentsSearcher::getAgent(const std::string &agent_name) {
    return agents_[agent_name];
}
}

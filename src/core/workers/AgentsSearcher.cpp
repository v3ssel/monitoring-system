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
    if (!std::filesystem::exists(agents_directory_)) {
        observer_->NotifyError("ERROR: AgentsSearcher::search(): Search Directory \"" + agents_directory_ + "\" not found.");
        return;
    }

    try {
        for (auto &agent : std::filesystem::directory_iterator(agents_directory_)) { 
            if (agent.path().extension() != ".so" &&
                agent.path().extension() != ".dll")
                continue;

            std::string new_agent_name = agent.path().filename().string();

            auto readCfg = [&](std::shared_ptr<Agent>& ag) {
                try {
                    ag->readConfig(configs_directory_);
                } catch (std::exception &e) {
                    observer_->NotifyError("ERROR: AgentsSearcher::search(): " + new_agent_name + "::readConfig(): " + std::string(e.what()));
                }
            };

            if (agents_.find(new_agent_name) != agents_.end()) {
                readCfg(agents_[new_agent_name]);
                continue;
            }
            
            auto created = agents_.emplace(new_agent_name, AgentsFactory::GetInstance().LoadAgent(agent.path().string()));
            if (!created.second)
                continue;

            readCfg(created.first->second);
            observer_->NotifyNewAgentLoaded(new_agent_name);
        }
    } catch (std::exception &e) {
        observer_->NotifyError("ERROR: AgentsSearcher::search(): " + std::string(e.what()));
    }
}

std::shared_ptr<Agent> &AgentsSearcher::getAgent(const std::string &agent_name) {
    return agents_[agent_name];
}

void AgentsSearcher::clearAgents() {
    agents_.clear();
}

void AgentsSearcher::setSearchDirectory(const std::string &directory) {
    agents_directory_ = directory;
}

void AgentsSearcher::setConfigDirectory(const std::string &directory) {
    configs_directory_ = directory;
}
}

#include <fstream>
#include "Agent.h"

namespace s21 {
Agent::Agent() {
    observer_ = nullptr;
    config_reader_ = std::make_unique<AgentConfigReader>(this);

    is_active = true;
    update_time_changed_ = false; 
}

void Agent::readConfig(const std::string &directory) {
    config_reader_->read(directory);
}

void Agent::setObserver(AgentsObserver *observer) {
    this->observer_ = observer;
}
}

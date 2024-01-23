#include <fstream>
#include "Agent.h"

namespace s21 {
Agent::Agent() {
    observer_ = nullptr;
    config_reader_ = nullptr;

    is_active = true;
    update_time_changed_ = false; 

    update_time_ = 3;
}

void Agent::readConfig(const std::string &directory) {
    if (config_reader_)
        config_reader_->read(directory);
}

void Agent::setObserver(AgentsObserver *observer) {
    observer_ = observer;
}

AgentsObserver *Agent::getObserver() {
    return observer_;
}

void Agent::addCriticalValue(const std::string &name, double value) {
    critical_values_[name] = value;
}

void Agent::addCriticalComparison(const std::string &name, CmpFunc cmp) {
    comparisons_[name] = cmp;
}
}

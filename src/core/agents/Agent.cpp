#include <fstream>
#include "Agent.h"

namespace s21 {
Agent::Agent() {
    observer_ = nullptr;
    config_reader_ = nullptr;

    is_active = true;
    is_update_time_changed = false; 

    update_time_ = 3;
}

void Agent::readConfig(const std::string &directory) {
    if (config_reader_)
        config_reader_->read(directory);
}

void Agent::setUpdateTime(int new_time) {
    if (update_time_ != new_time) {
        is_update_time_changed = true;
        update_time_ = new_time;
    }
}

int Agent::getUpdateTime() {
    return update_time_;
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

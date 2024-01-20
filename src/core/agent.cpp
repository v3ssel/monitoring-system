#include <fstream>
#include "Agent.h"

namespace s21 {
void Agent::readConfig(const std::string &directory) {
    std::ifstream conf(directory + '/' + this->config_name);
    if (!conf.is_open() && observer_) {
        observer_->NotifyError("ERROR: " + this->name + ": Could not open the configuration file: \"" + directory + '/' + this->config_name + "\".");
        return;
    } 

    std::string line;
    while (std::getline(conf, line)) {
        if (line.find("name") == 0) {
            this->name = line.substr(line.find(":") + 1);
        }
        if (line.find("type") == 0) {
            this->type = line.substr(line.find(":") + 1);
        }
        if (line.find("update_time") == 0) {
            int next_update = std::stoi(line.substr(line.find(":") + 1));

            if (this->update_time != next_update) {
                this->update_time_changed = true;
                this->update_time = next_update;
            }
        }
        
        for (auto& metric : metrics_names_) {
            if (line.find(metric) == 0) {
                setComparisonsAndCriticals(metric.length(), metric, line);
            }
        }
    }
}

void Agent::setComparisonsAndCriticals(size_t op_index, const std::string &type, const std::string& line){
        if (observer_ && line.size() <= op_index + 1) {
            observer_->NotifyError("ERROR: " + this->name + ": Could not read critical value for \"" + type + "\" metric.");
            return;
        }

        size_t value_index = line[op_index + 1] == '=' ? op_index + 2 : op_index + 1;

        if (line[op_index] == '>') {
            if (line[op_index + 1] == '=') {
                this->comparisons_[type] = Comparisons<double>::is_greater_or_equal;
            } else {
                this->comparisons_[type] = Comparisons<double>::is_greater;
            }
        }

        if (line[op_index] == '<') {
            if (line[op_index + 1] == '=') {
                this->comparisons_[type] = Comparisons<double>::is_less_or_equal;
            } else {
                this->comparisons_[type] = Comparisons<double>::is_less;
            }
        }

        if (line[op_index] == '=' && line[op_index + 1] == '=') {
            this->comparisons_[type] = Comparisons<double>::is_equal;
        }

        if (line[op_index] == '!' && line[op_index + 1] == '=') {
            this->comparisons_[type] = Comparisons<double>::is_not_equal;
        }

        critical_values_[type] = std::stod(line.substr(value_index));
}

void Agent::setObserver(AgentsObserver *observer) {
    this->observer_ = observer;
}
}

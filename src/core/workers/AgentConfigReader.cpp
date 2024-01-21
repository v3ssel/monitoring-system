#include "AgentConfigReader.h"
#include "../Agent.h"
#include <fstream>

namespace s21 {
AgentConfigReader::AgentConfigReader() {
    agent_ = nullptr;
}

AgentConfigReader::AgentConfigReader(Agent *agent) {
    agent_ = agent;
}
    
void AgentConfigReader::read(const std::string &path) {
    std::ifstream conf(path + '/' + agent_->config_name_);
    if (!conf.is_open() && agent_->observer_) {
        agent_->observer_->NotifyError("ERROR: " + agent_->name +
                                       ": Could not open the configuration file: \"" +
                                       path + '/' + agent_->config_name_ + "\".");
        return;
    }

    std::string line;
    while (std::getline(conf, line)) {
        if (line.find("name") == 0) {
            agent_->name = line.substr(line.find(":") + 1);
        }
        if (line.find("type") == 0) {
            agent_->type = line.substr(line.find(":") + 1);
        }
        if (line.find("update_time") == 0) {
            int next_update = std::stoi(line.substr(line.find(":") + 1));

            if (agent_->update_time != next_update) {
                agent_->update_time_changed_ = true;
                agent_->update_time = next_update;
            }
        }
        
        for (auto& metric : agent_->metrics_names_) {
            if (line.find(metric) == 0) {
                setComparisonsAndCriticals(metric.length(), metric, line);
            }
        }
    }
}

void AgentConfigReader::setComparisonsAndCriticals(size_t op_index, const std::string &type, const std::string &line) {
    if (agent_->observer_ && line.size() <= op_index + 1) {
        agent_->observer_->NotifyError("ERROR: " + agent_->name + ": Could not read critical value for \"" + type + "\" metric.");
        return;
    }

    size_t value_index = line[op_index + 1] == '=' ? op_index + 2 : op_index + 1;

    if (line[op_index] == '>') {
        if (line[op_index + 1] == '=') {
            agent_->comparisons_[type] = Comparisons<double>::is_greater_or_equal;
        } else {
            agent_->comparisons_[type] = Comparisons<double>::is_greater;
        }
    }

    if (line[op_index] == '<') {
        if (line[op_index + 1] == '=') {
            agent_->comparisons_[type] = Comparisons<double>::is_less_or_equal;
        } else {
            agent_->comparisons_[type] = Comparisons<double>::is_less;
        }
    }

    if (line[op_index] == '=' && line[op_index + 1] == '=') {
        agent_->comparisons_[type] = Comparisons<double>::is_equal;
    }

    if (line[op_index] == '!' && line[op_index + 1] == '=') {
        agent_->comparisons_[type] = Comparisons<double>::is_not_equal;
    }

    agent_->critical_values_[type] = std::stod(line.substr(value_index));
}

} // namespace s21
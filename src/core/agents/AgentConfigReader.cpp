#include <fstream>
#include "AgentConfigReader.h"
#include "Agent.h"
#include "utils/Comparisons.h"
#include "utils/CompareType.h"

namespace s21 {

AgentConfigReader::AgentConfigReader(Agent *agent) 
    : ConfigReader(agent) {}
    
void AgentConfigReader::read(const std::string &path) {
    std::ifstream conf(path + '/' + agent_->config_name);
    if (!conf.is_open() && agent_->getObserver()) {
        agent_->getObserver()->NotifyError("ERROR: " + agent_->name +
                                       ": Could not open the configuration file: \"" +
                                       path + '/' + agent_->config_name + "\".");
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
            agent_->setUpdateTime(std::stoi(line.substr(line.find(":") + 1)));
        }
        
        for (auto& metric : agent_->metrics_names_) {
            if (line.find(metric) == 0) {
                setComparisonsAndCriticals(metric.length(), metric, line);
            }
        }
    }
}

void AgentConfigReader::setComparisonsAndCriticals(size_t op_index, const std::string &type, const std::string &line) {
    if (agent_->getObserver() && line.size() <= op_index + 1) {
        agent_->getObserver()->NotifyError("ERROR: " + agent_->name + ": Could not read critical value for \"" + type + "\" metric.");
        return;
    }

    size_t value_index = line[op_index + 1] == '=' ? op_index + 2 : op_index + 1;

    if (line[op_index] == '>') {
        if (line[op_index + 1] == '=') {
            agent_->addCriticalComparison(type, Comparisons<double>::is_greater_or_equal, CompareType::IS_GREATER_OR_EQ);
        } else {
            agent_->addCriticalComparison(type, Comparisons<double>::is_greater, CompareType::IS_GREATER);
        }
    }

    if (line[op_index] == '<') {
        if (line[op_index + 1] == '=') {
            agent_->addCriticalComparison(type, Comparisons<double>::is_less_or_equal, CompareType::IS_LESS_OR_EQ);
        } else {
            agent_->addCriticalComparison(type, Comparisons<double>::is_less, CompareType::IS_LESS);
        }
    }

    if (line[op_index] == '=' && line[op_index + 1] == '=') {
        agent_->addCriticalComparison(type, Comparisons<double>::is_equal, CompareType::IS_EQ);
    }

    if (line[op_index] == '!' && line[op_index + 1] == '=') {
        agent_->addCriticalComparison(type, Comparisons<double>::is_not_equal, CompareType::IS_NOT_EQ);
    }

    agent_->addCriticalValue(type, std::stod(line.substr(value_index)));
}

} // namespace s21

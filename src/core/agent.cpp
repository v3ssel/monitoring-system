#include "Agent.h"

namespace s21 {
Agent::~Agent() {}

void Agent::SetObserver(AgentsObserver* observer) {
    this->observer_ = observer;
}

void Agent::SetComparisonsAndCriticals(size_t op_index, const std::string &type, const std::string& line){
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
}

#include "agent.h"

namespace s21 {
    void Agent::SetErrorsReceiver(std::function<void(std::string)> errors_receiver) {
        this->send_error_ = errors_receiver;
    }

    void Agent::SetCriticalValuesCallback(std::function<void(std::string)> critical_values_callback) {
        this->send_critical_value_ = critical_values_callback;
    }

    void Agent::SetComparisonsAndCriticals(size_t op_index, const std::string &type, const std::string& line){
        if (line.size() <= op_index + 1) {
            this->send_error_(this->name + " error: Could not read critical value for \"" + type + "\" metric.");
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

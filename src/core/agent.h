#ifndef __AGENT_H__
#define __AGENT_H_

#include <string>
#include <functional>
#include "comparisons.h"

namespace s21 {    
    class Agent {
       public:
        bool is_active;
        
        std::string name;
        std::string type;
        int update_time;
        
        virtual void readConfig(const std::string& file_name) = 0;
        virtual void updateMetrics() = 0;
        virtual std::string toString() = 0;

        void SetErrorsReceiver(std::function<void(std::string)> errors_receiver);
        void SetCriticalValuesCallback(std::function<void(std::string)> critical_values_callback);
        void SetComparisonsAndCriticals(size_t op_index, const std::string& type, const std::string& line);
        
       protected:
        std::unordered_map<std::string, std::function<bool(double, double)>> comparisons_;
        std::unordered_map<std::string, double> critical_values_;

        std::function<void(std::string)> send_error_;
        std::function<void(std::string)> send_critical_value_;
    };
}

#endif

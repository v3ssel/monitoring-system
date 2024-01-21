#ifndef _AGENT_H_
#define _AGENT_H_

#include <string>
#include <functional>
#include <memory>
#include "interfaces/AgentsObserver.h"
#include "utils/Comparisons.h"

namespace s21 {    
    class Agent {
       public:
        bool is_active;
        std::string name;
        std::string type;
        std::vector<std::string> metrics_names_;
        int update_time;

        virtual ~Agent() = default;

        virtual void updateMetrics() = 0;
        virtual std::string toString() = 0;

        void readConfig(const std::string& directory);
        void setComparisonsAndCriticals(size_t op_index, const std::string& type, const std::string& line);
        void setObserver(AgentsObserver* observer);
        
       protected:
        bool update_time_changed;
        std::string config_name;

        AgentsObserver* observer_;
        std::unordered_map<std::string, std::function<bool(double, double)>> comparisons_;
        std::unordered_map<std::string, double> critical_values_;
    };
}

#endif  // _AGENT_H_

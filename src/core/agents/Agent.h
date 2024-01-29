#ifndef _AGENT_H_
#define _AGENT_H_

#include <string>
#include <functional>
#include <memory>

#include "../interfaces/AgentsObserver.h"
#include "../interfaces/ConfigReader.h"
#include "../utils/CompareType.h"

namespace s21 {
    using CmpFunc = std::function<bool(double, double)>;

    struct AgentData {
        CompareType compare_type;
        CmpFunc compare_func;
        double critical_val;
    };

    class Agent {
       public:

        bool is_active;
        bool is_update_time_changed;
        std::string name;
        std::string type;
        std::string config_name;
        std::unordered_map<std::string, std::string> additional_params_;
        std::vector<std::string> metrics_names_;

        Agent();
        virtual ~Agent() = default;

        virtual void updateMetrics() = 0;
        virtual std::string toString() = 0;

        virtual void readConfig(const std::string& directory);
        
        void setUpdateTime(int new_time);
        int getUpdateTime();

        void setObserver(AgentsObserver* observer);
        AgentsObserver* getObserver();

        void addCriticalValue(const std::string& name, double value);
        void addCriticalComparison(const std::string& name, CmpFunc cmp, CompareType type);

        CompareType getCompareFncType(const std::string& metric_name);
        double getCriticalValue(const std::string& metric_name);

       protected:
        int update_time_;

        AgentsObserver* observer_;
        std::unique_ptr<ConfigReader> config_reader_;

        // std::unordered_map<std::string, CmpFunc> comparisons_;
        // std::unordered_map<std::string, double> critical_values_;

        std::unordered_map<std::string, AgentData> compare_data_;
    };
}

#endif  // _AGENT_H_

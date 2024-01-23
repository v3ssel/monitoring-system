#ifndef _AGENT_H_
#define _AGENT_H_

#include <string>
#include <functional>
#include <memory>
#include "../interfaces/AgentsObserver.h"
#include "../interfaces/ConfigReader.h"
#include "../utils/Comparisons.h"

namespace s21 {    
    class Agent {
       public:
        using CmpFunc = std::function<bool(double, double)>;

        bool is_active;
        std::string name;
        std::string type;
        std::string config_name;
        std::vector<std::string> metrics_names_;

        Agent();
        virtual ~Agent() = default;

        virtual void updateMetrics() = 0;
        virtual void setUpdateTime(int) = 0;
        virtual std::string toString() = 0;

        virtual void readConfig(const std::string& directory);

        void setObserver(AgentsObserver* observer);
        AgentsObserver* getObserver();

        void addCriticalValue(const std::string& name, double value);
        void addCriticalComparison(const std::string& name, CmpFunc cmp);

       protected:
        bool update_time_changed_;
        int update_time_;

        AgentsObserver* observer_;
        std::unique_ptr<ConfigReader> config_reader_;

        std::unordered_map<std::string, CmpFunc> comparisons_;
        std::unordered_map<std::string, double> critical_values_;
    };
}

#endif  // _AGENT_H_

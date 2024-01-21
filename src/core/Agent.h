#ifndef _AGENT_H_
#define _AGENT_H_

#include <string>
#include <functional>
#include <memory>
#include "interfaces/AgentsObserver.h"
#include "utils/Comparisons.h"
#include "workers/AgentConfigReader.h"

namespace s21 {    
    class Agent {
        friend class AgentConfigReader;
       public:
        bool is_active;
        std::string name;
        std::string type;
        std::vector<std::string> metrics_names_;
        int update_time;

        Agent();
        virtual ~Agent() = default;

        virtual void updateMetrics() = 0;
        virtual std::string toString() = 0;

        virtual void readConfig(const std::string& directory);
        void setObserver(AgentsObserver* observer);
        
       protected:
        bool update_time_changed_;
        std::string config_name_;

        AgentsObserver* observer_;
        std::unique_ptr<AgentConfigReader> config_reader_;

        std::unordered_map<std::string, std::function<bool(double, double)>> comparisons_;
        std::unordered_map<std::string, double> critical_values_;
    };
}

#endif  // _AGENT_H_

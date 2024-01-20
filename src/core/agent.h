#ifndef _AGENT_H_
#define _AGENT_H_

#include <string>
#include <functional>
#include <memory>
#include "AgentsObserver.h"
#include "Comparisons.h"

namespace s21 {    
    class Agent {
       public:
        bool is_active;
        
        std::string name;
        std::string type;
        int update_time;

        virtual ~Agent();

        virtual void readConfig(const std::string& file_name) = 0;
        virtual void updateMetrics() = 0;
        virtual std::string toString() = 0;

        void SetObserver(AgentsObserver* observer);
        void SetComparisonsAndCriticals(size_t op_index, const std::string& type, const std::string& line);
        
    //    protected:
        bool update_time_changed;
        std::string config_name;

        std::unordered_map<std::string, std::function<bool(double, double)>> comparisons_;
        std::unordered_map<std::string, double> critical_values_;

        AgentsObserver* observer_;
    };
}

#endif  // _AGENT_H_

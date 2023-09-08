#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <unordered_map>
#include <vector>
#include <map>
#include <chrono>
#include <thread>
#include <fstream>
#include <filesystem>
#include "agentsFactory.h"

namespace s21 {  
    class Kernel {
    public:
        std::vector<std::string> agent_names;
        std::map<std::string, s21::AgentsFactory> agents;
        std::map<std::string, std::thread> threads;
        int record_time;
        // std::vector<std::thread> threads;

        Kernel();
        void searchNewAgents();

        // void analyzeSystem(Agent& agent);
        // void analyzeSystem(std::unique_ptr<s21::Agent>& agent);
        void analyzeSystem(AgentsFactory& agent);
        void makeRecords();
        void updateActiveAgents();
        
        // void launchAgents();
    };
}

#endif

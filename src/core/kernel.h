#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <unordered_map>
#include <vector>
#include <queue>
#include <chrono>
#include <thread>
#include <fstream>
#include <filesystem>
#include "agentsFactory.h"

namespace s21 {  
    class Kernel {
       public:
        std::vector<std::string> agents_names;
        std::unordered_map<std::string, std::shared_ptr<s21::Agent>> agents;
        std::unordered_map<std::string, std::thread> threads;
        int record_time;

        Kernel(const std::string& agents_directory = "./agents");
        void searchNewAgents();

        void analyzeSystem(std::shared_ptr<s21::Agent>& agent);
        void makeRecords();
        void updateActiveAgents();
        
       private:
        std::string agents_directory_;
        std::queue<std::string> errors_q_;
    };
}

#endif

#ifndef __KERNEL_H__
#define __KERNEL_H__
#include <unordered_map>
#include <vector>
#include <map>
#include <thread>
#include <filesystem>
#include "agentsFactory.h"

class Kernel {
    std::vector<std::string> agent_names;
    std::vector<s21::Agent*> ags;
    std::map<std::string, s21::AgentsFactory> agents;
    std::vector<std::thread> threads;


    Kernel();
    void makeRecord();
    void searchNewAgents();
    void updateActiveAgents();
    // void launchAgents();
};

#endif

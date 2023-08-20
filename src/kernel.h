#ifndef __KERNEL_H__
#define __KERNEL_H__
#include <unordered_map>
#include <vector>
#include <map>
#include "agent.h"

class Kernel {
    std::vector<std::string> agent_names;
    std::vector<s21::Agents*> ags;
    std::map<string, s21::AgentsFactory> agents;
    std::vector<std::thread> threads;


    Kernel();
    void makeRecord();
    void searchNewAgents();
    void updateActiveAgents();
    void launchAgents();
};

#endif

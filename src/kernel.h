#ifndef __KERNEL_H__
#define __KERNEL_H__
#include <vector>
#include "agent.h"

class Kernel {
    std::vector<s21::Agent*> agents;
    Kernel();
    void makeRecord();
    void searchNewAgents();
    void updateActiveAgents();
};

#endif

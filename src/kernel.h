#ifndef __KERNEL_H__
#define __KERNEL_H__

class Kernel {
    std::vector<Agent> agents;
    Kernel();
    void makeRecord();
    void searchNewAgents();
    void updateActiveAgents();
};

#endif

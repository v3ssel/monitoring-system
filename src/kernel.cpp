#include "kernel.h"
// #include <ctime>

// namespace s21 {
// }  // namespace s21

Kernel::Kernel() {
    updateActiveAgents();
}

void Kernel::makeRecord() {
    std::ofstream log("yyyy-MM-dd.txt", std::ios::app);
    if (log.is_open()) {
        std::cout << "\n yy-MM-dd HH:mm:ss | ";
        for(Agent agent: agents) {
            std::cout << agent.toString << " | ":
        }
    }
    log.close();
}

void Kernel::searchNewAgents() {

}

void Kernel::updateActiveAgents() {
    //look for
    agents.push_back();
}

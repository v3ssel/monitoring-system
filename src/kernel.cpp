#include "kernel.h"
#include <ctime>
#include <iomanip>  // std::put_time
#include <iostream>

namespace s21 {

Kernel::Kernel() {
    std::cout << "start" << std::endl;
    record_time = 4;
    std::thread(&Kernel::searchNewAgents, this).detach();
    std::thread(&Kernel::makeRecords, this).detach();
}

void Kernel::makeRecords() {
    while (true) {
        std::cout << "rec" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(record_time));
        
        std::time_t t = std::time(nullptr);
        std::tm lt = *std::localtime(&t);
        char date_buf[10];
        strftime(date_buf, 10, "%y-%m-%d", &lt);
        
        std::string log_name = "./logs/" + std::string(date_buf) + ".txt";
        std::ofstream log(log_name, std::ios::app);

        if (log.is_open()) {
            log << std::put_time(&lt, "[%y-%m-%d %H:%M:%S] | ");
            for (auto& agent: agents) {
                log << agent.second.GetAgent()->toString() << " | ";
            }
            log << std::endl;
        }

        log.close();
    }
}

void Kernel::updateActiveAgents() {
    for (auto &agent : std::filesystem::directory_iterator("./agents")) { 
        if (agent.path().extension() != ".so") continue;
        // std::string new_agent = agent.path().filename().string();
        std::string new_agent = agent.path().string();
        if (agents.find(new_agent) != agents.end()) continue;
        
        bool is_create = agents.emplace(new_agent, AgentsFactory(new_agent)).second;
        if (!is_create) continue;

        bool is_launch = threads.emplace(new_agent, std::thread(&Kernel::analyzeSystem, this, std::ref(agents[new_agent]))).second; 
        if (is_launch) agent_names.push_back(new_agent);
    }
}

void Kernel::searchNewAgents() {
    while (true) {
        updateActiveAgents();
    }
}

// void Kernel::analyzeSystem(Agent& agent) {
//     agent.updateMetrics();
// }

// void Kernel::analyzeSystem(std::unique_ptr<s21::Agent>& agent) {
//     agent->updateMetrics();
// }

void Kernel::analyzeSystem(AgentsFactory& agent) {
    agent.GetAgent()->updateMetrics();
}


}  // namespace s21

// void Kernel::searchNewAgents() {
//     for (auto &agent : std::filesystem::directory_iterator("./agent")) {    //поиск агентов
//         if (agent.path().extension() == ".so") {
//             // std::cout << agent.path().filename().string() << std::endl;
//             // agent_names.push_back(agent.path().filename().string());
//             agent_names.push_back(agent.path().string());
//             std::cout << agent.path().string() << std::endl;
//         }
//     }
// }

// void Kernel::createAgents() {
//     for (auto file_name : agent_names) {    //создание агентов
//         agents.insert({file_name, AgentsFactory ag(file_name)});
//     }
// }

// void launchAgents() {
//     for (auto agent : agents) {     //запуск агентов
//         std::thread thread(agent.second.GetAgent()->analyzeSystem());
//         threads.push_back();
//         // agent.second.GetAgent()->analyzeSystem();
//     }
// }

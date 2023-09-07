#include "kernel.h"
#include <ctime>
#include <fstream>
// #include <dlfcn.h>
#include <iomanip>  // std::put_time
#include <thread>
#include <iostream>

namespace s21 {

Kernel::Kernel() {
    std::cout << "start" << std::endl;
    // record_time = 4;
    std::thread search(this, &Kernel::searchNewAgents);
    while (true) {
        makeRecord();
        std::this_thread::sleep_for(std::chrono::seconds(record_time));
    }
}

void Kernel::makeRecord() {
    std::time_t t = std::time(nullptr);
    std::tm lt = *std::localtime(&t);
    char date_buf[10];
    strftime(date_buf, 10, "%y-%m-%d", &lt);
    std::string date_str(date_buf);
    std::string log_name = "./logs/" + date_str + ".txt";
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

void Kernel::updateActiveAgents() {
    for (auto &agent : std::filesystem::directory_iterator("./agents")) { 
        if (agent.path().extension() != ".so") continue;
        std::string new_agent = agent.path().filename().string();
        if (agents.find(new_agent) != agents.end()) continue;
        bool is_create = agents.emplace(new_agent, AgentsFactory(new_agent)).second;
        if (!is_create) continue;
        bool is_launch = threads.emplace(new_agent, std::thread(agents[new_agent].GetAgent(), &analyzeSystem)).second; 
        if (is_launch) agent_names.push_back(new_agent);            
    }
}

void Kernel::searchNewAgents() {
    while (true) {
        std::cout << "in" << std::endl;
        updateActiveAgents();

    }
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

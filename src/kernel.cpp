#include "kernel.h"
#include <ctime>
#include <fstream>
#include <dlfcn.h>
#include <iomanip>  // std::put_time
#include <thread>
// #include <pthread.h>    // Скомпилируй используя -lpthread 

// namespace s21 {
// }  // namespace s21

Kernel::Kernel() {
    updateActiveAgents();
    std::thread thread_1(searchNewAgents);
    // pthread_create(&thread_01, NULL, searchNewAgents, NULL);
}

void Kernel::makeRecord() {
    std::time_t t = std::time(nullptr);
    std::tm lt = *std::localtime(&t);
    // std::cout << std::put_time(&lt, "[%Y-%m-%d %H:%M:%S]") << std::endl;
    char date_buf[10];
    // char time_buf[10];
    strftime(date_buf, 10, "%y-%m-%d", &lt);
    // strftime(time_buf, 10, "%H:%M:%S", &lt);
    // std::cout << date_buf << std::endl;
    // std::cout << time_buf << std::endl;
    std::string date_str(date_buf);
    // std::string time_str(time_buf);
    std::string log_name = date_str + ".txt";
    // std::cout << log_name << std::endl;
    std::ofstream log(log_name, std::ios::app);
    if (log.is_open()) {
        log << std::put_time(&lt, "[%y-%m-%d %H:%M:%S] | ");
        // log << "[" + date_str + " " + time_str + "] | ";
        for(Agent agent: agents) {
            log << agent.second.GetAgent()->toString << " | ":
        }
        log << std::endl;
    }
    log.close();
}

void Kernel::updateActiveAgents() {
    // std::vector<std::string> agent_names;
    for (auto &agent : std::filesystem::directory_iterator("./agent")) {    //поиск агентов
        if (agent.path().extension() == ".so") {
            // std::cout << agent.path().filename().string() << std::endl;
            agent_names.push_back(agent.path().filename().string());
        }
    }

    // std::vector<s21::Agent*> agents;
    for (auto file_name : agent_names) {    //создание агентов
        // void *op = dlopen(file_name.c_str(), RTLD_LAZY);
        // s21::Agent* (*create)();
        // create = (s21::Agent*(*)())dlsym(op, "create_obj");
        // s21::Agent* ag = (s21::Agent*)create();
        agents.insert({file_name, AgentsFactory ag(file_name)});
    }

    // for (auto agent : agents) {     //запуск агентов
    //     agent.GetAgent()->analyzeSystem();
    // }

}

void Kernel::searchNewAgents() {
    for (auto &agent : std::filesystem::directory_iterator("./agent")) {    //поиск агентов
        if (agent.path().extension() == ".so") {
            // std::cout << agent.path().filename().string() << std::endl;
            // agent_names.push_back(agent.path().filename().string());
            agent_names.push_back(agent.path().string());
            std::cout << agent.path().string() << std::endl;
        }
    }
}

void Kernel::createAgents() {
    for (auto file_name : agent_names) {    //создание агентов
        agents.insert({file_name, AgentsFactory ag(file_name)});
    }
}

void launchAgents() {
    for (auto agent : agents) {     //запуск агентов
        std::thread thread(agent.second.GetAgent()->analyzeSystem());
        threads.push_back();
        // agent.second.GetAgent()->analyzeSystem();
    }
}

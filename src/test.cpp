// #include <iostream>
// #include <fstream>
// #include <string>
// #include "commandCaller.h"
// // #include <string.h>
// #include <atomic>
// #include <vector>
// #include <sstream>
// #include <thread>
// #include <chrono>

// using namespace std::chrono_literals;

// std::string takeValue(std::string command) {
//     FILE* pipe = popen(command.c_str(), "r");
//     if (!pipe) {return "";}
//     char buffer[8];
//     fgets(buffer, 8, pipe);
//     pclose(pipe);
//     return std::string (buffer);
// }

// std::chrono::milliseconds ms = 8000ms;

// std::atomic<int> ab = {0};

// void func() {
//     while (true) {
//         auto tmp = ms.count();
//         std::this_thread::sleep_for(ms);

//         if (tmp != ms.count())
//             break;

//         std::stringstream ss;

//         ss << "THREAD: " << std::this_thread::get_id() << " " << ab << " " << tmp << " " << ms.count() << "\n";
        
//         std::cout << ss.str();
//     }
// }

// int main(int argc, const char * argv[]) {
//             // url = "translate.yandex.ru";    // считывание с config
//     // std::string command = "ping -c 1 translate.yandex.ru | awk ' /^1 packets transmitted/{print $7}'";
//     // std::string loss = takeValue(command);
//     // std::cout << loss << std::endl;
//     // int availability = loss.find("0.0%") ? 0 : 1;
//     // std::cout << availability << std::endl;
//     // if (loss.compare("0.0%"))
//     //     std::cout << 0 << std::endl;
//     //     // availability = 0;
//     // else
//     //     std::cout << 1 << std::endl;
//     //     // availability = 1;

//     std::vector<std::thread> tt;

//     for (auto i = 0; i < 4; i++) {
//         tt.emplace_back(func);
//     }

//     std::this_thread::sleep_for(std::chrono::seconds(3));

//     ms = 1000ms;
    
//     ab = {1};

//     tt[2].detach();
//     tt[2] = std::thread(func);
    
//     for (auto i = 0; i < 10; i++) {
//         tt[i].join();
//     }

    
//     std::cout << "MAIN T\n";
//     return 0;
// }


#include <filesystem>
#include <iostream>
#include <vector>
#include <dlfcn.h>
#include "agent.h"


void updateActiveAgents() {
    std::vector<std::string> agent_libs;
    for (auto &agent : std::filesystem::directory_iterator("./agents")) {    //поиск агентов
        if (agent.path().extension() == ".so") {
            // std::cout << agent.path().filename().string() << std::endl;
            agent_libs.push_back(agent.path().filename().string());
        }
    }

    std::vector<s21::Agent*> agents;
    for (auto file_name : agent_libs) {    //создание агентов
        void *op = dlopen(file_name.c_str(), RTLD_LAZY);
        s21::Agent* (*create)();
        create = (s21::Agent*(*)())dlsym(op, "create_obj");
        s21::Agent* ag = (s21::Agent*)create();
        agents.push_back(ag);
        dlclose(op);
    }

    for (auto agent : agents) {     //запуск агентов
        agent->analyzeSystem();
    }
}

// void updateActiveAgents() {
//     std::vector<std::string> agents;
//     for (auto &agent : std::filesystem::directory_iterator("./agent")) {        //поиск агентов
//         if (agent.path().extension() == ".so") {
//             // void *op = dlopen(agent.path().filename().c_str(), RTLD_LAZY);
//             void *op = dlopen(agent.path().filename().string(), RTLD_LAZY);    //создание агентов
//             s21::Agent* (*create)();
//             create = (s21::Agent*(*)())dlsym(op, "create_obj");
//             s21::Agent* ag = (s21::Agent*)create();
//             agents.push_back(ag);
//             dlclose(op);
//             ag->analyzeSystem();    //запуск агентов
//         }
//     }
// }

int main() {
    updateActiveAgents();
    return 0;
}

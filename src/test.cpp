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

// void Kernel::makeRecord() {
//     std::time_t t = std::time(nullptr);
//     std::tm lt = *std::localtime(&t);
//     // std::cout << std::put_time(&lt, "[%Y-%m-%d %H:%M:%S]") << std::endl;
//     char date_buf[10];
//     // char time_buf[10];
//     strftime(date_buf, 10, "%y-%m-%d", &lt);
//     // strftime(time_buf, 10, "%H:%M:%S", &lt);
//     // std::cout << date_buf << std::endl;
//     // std::cout << time_buf << std::endl;
//     std::string date_str(date_buf);
//     // std::string time_str(time_buf);
//     std::string log_name = date_str + ".txt";
//     // std::cout << log_name << std::endl;
//     std::ofstream log(log_name, std::ios::app);
//     if (log.is_open()) {
//         log << std::put_time(&lt, "[%y-%m-%d %H:%M:%S] | ");
//         // log << "[" + date_str + " " + time_str + "] | ";
//         for(Agent agent: agents) {
//             log << agent.second.GetAgent()->toString << " | ":
//         }
//         log << std::endl;
//     }
//     log.close();
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


// #include <filesystem>
// #include <iostream>
// #include <vector>
// #include <thread>
// #include <dlfcn.h>
// #include <filesystem>
// #include "kernel.h"

//     std::vector<std::string> agent_names;
//     std::vector<s21::AgentsFactory> ags;
//     std::map<std::string, s21::AgentsFactory> agents;
//     std::vector<std::thread> threads;

// void searchNewAgents() {
//     for (auto &agent : std::filesystem::directory_iterator("./agents")) {    //поиск агентов
//         if (agent.path().extension() == ".so") {
//             // std::cout << agent.path().filename().string() << std::endl;
//             // agent_names.push_back(agent.path().filename().string());
//             agent_names.push_back(agent.path().string());
//             std::cout << agent.path().string() << std::endl;
//         }
//     }
// }

// void createAgents() {
//     for (auto file_name : agent_names) {    //создание агентов
//         // agents.insert({file_name, s21::AgentsFactory(file_name)});
//         // agents.insert(std::pair<std::string, s21::AgentsFactory>(file_name, s21::AgentsFactory(file_name)));
//         std::cout << file_name << std::endl;
//         // s21::AgentsFactory af(file_name);
//         // af.GetAgent()->analyzeSystem();
//         // std::cout << af.GetAgent()->toString() << std::endl;
//         ags.emplace_back(std::move(s21::AgentsFactory(file_name)));
//         // auto ag = std::pair<std::string, s21::AgentsFactory*> (file_name, s21::AgentsFactory(file_name));
//         // agents.insert(ag);

//         // ags.push_back(s21::AgentsFactory(file_name));
//     }
// }

// void launchAgents() {
//     for (auto agent : ags) {     //запуск агентов
//         // std::thread thread(agent.second.GetAgent()->analyzeSystem());
//         // threads.push_back();
//         std::cout << agent.GetAgent()->toString() << std::endl;
//         agent.GetAgent()->analyzeSystem();
//     }
// }
#include <iostream>
#include <csignal>
#include <unordered_map>
#include <thread>
// #include "core/agents_src/agentCPU.h"
#include "core/Kernel.h"
// #include "core/agentsFactory.h"

int main() {
    std::signal(SIGSEGV, [](int) {
        std::cout << "SEGA\n";
    });
    std::atexit([]() {
        std::cout << "success exit\n";
    });

    std::cout << "MAIN" << "\n";

    s21::Kernel k("C:\\Coding\\Projects\\CPP9_MonitoringSystem-1\\src\\agents");

    // std::this_thread::sleep_for(std::chrono::seconds(5));
    // std::cout << "DISABLING" << "\n";
    // k.disableAgent("libagentCPU.so");
    // std::cout << "DISABLED" << "\n";

    // std::this_thread::sleep_for(std::chrono::seconds(5));
    // std::cout << "ENABLING" << "\n";
    // k.enableAgent ("libagentCPU.so");
    // std::cout << "ENABLED" << "\n";

    // std::this_thread::sleep_for(std::chrono::seconds(600));
    // std::cout << "MAIN WOKEN UP" << "\n";

    while (true) {
        while (k.qcritical_values_.size() > 0) {
            std::cout << k.qcritical_values_.front() << "\n";
            k.qcritical_values_.pop();
        }
        while (k.qerrors_.size() > 0) {
            std::cout << "ERROR: " << k.qerrors_.front() << "\n";
            k.qerrors_.pop();
        }
    }

    return 0;
}

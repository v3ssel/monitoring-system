#include "agentNetwork.h"
#include "commandCaller.h"
#include <sys/sysctl.h>
#include <iostream>
#include <thread>

extern "C" s21::AgentNetwork* create_obj() {
    return new s21::AgentNetwork;
}

s21::AgentNetwork::AgentNetwork() {
    is_active = true;
    availability = 0;
    inet_throughput = 100;
    readConfig(".confNet");
}

void s21::AgentNetwork::readConfig(std::string file_name) {
    std::ifstream conf(file_name);
    if (conf.is_open()) {
        std::string line;
        while (std::getline(conf, line)) {
            if (!line.find("update_time") == std::string::npos) {
                std::cout << "dfv" << std::endl;
                update_time_ = atoi(line.substr(line.find("update_time")).c_str());
                std::cout << update_time_ << std::endl;
            }
        }
    }
    conf.close();
}

void s21::AgentNetwork::updateMetrics() {
    while (is_active) {
        url = "translate.yandex.ru";    // считывание с config
        std::string command = "ping -c 1 " + url + " | awk ' /^1 packets transmitted/{print $7}'";
        std::string loss = CommandCaller::getInstance().takeValue(command);
        availability = loss.find("0.0%") ? 0 : 1;
        std::cout << availability << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(update_time_));
        std::cout << is_active << std::endl;
    };
};

std::string s21::AgentNetwork::toString() {
    return "<url> : " + std::to_string(this->availability) + " | inet_throughput : " + std::to_string(this->inet_throughput);
};

// netstat -I en0 -b 5 | head -n 3
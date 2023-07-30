#include "agentNetwork.h"
#include "commandCaller.h"
#include <sys/sysctl.h>

extern "C" s21::AgentNetwork* create_obj() {
    return new s21::AgentNetwork;
}

extern "C" void destroy_obj(s21::AgentNetwork* agent) {
    delete agent;
}

void s21::AgentNetwork::analyzeSystem() {
    url = "translate.yandex.ru";    // считывание с config
    std::string command = "ping -c 1 " + url + " | awk ' /^1 packets transmitted/{print $7}'";
    std::string loss = takeValue(command);
    availability = loss.find("0.0%") ? 0 : 1;
    // std::cout << availability << std::endl;

};

std::string s21::AgentNetwork::toString() {
    return "<url> : "s + std::to_string(this->availability) + " | inet_throughput : "s + std::to_string(this->inet_throughput);
};

// netstat -I en0 -b 5 | head -n 3
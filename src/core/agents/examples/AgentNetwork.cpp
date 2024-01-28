#include <iostream>
#include <thread>
#include <chrono>

#include "AgentNetwork.h"
// #include "CommandCaller.h"
// #include <sys/sysctl.h>

namespace s21 {
    extern "C" AgentNetwork* create_obj() {
        return new AgentNetwork;
    }

    AgentNetwork::AgentNetwork() : Agent() {
        is_active = true;
        availability = 0;
        inet_throughput = 100;

        Agent::name = "AgentNetwork";
        Agent::type = "NET";
        Agent::config_name = ".conf" + Agent::type;
    }

    void AgentNetwork::updateMetrics() {
        observer_->NotifyResult(this->toString());
        // url = "translate.yandex.ru";    // считывание с config
        // std::string command = "ping -c 1 " + url + " | awk ' /^1 packets transmitted/{print $7}'";
        // std::string loss = CommandCaller::getInstance().takeValue(command);
        // availability = loss.find("0.0%") ? 0 : 1;
        // std::cout << availability << std::endl;
        // std::this_thread::sleep_for(std::chrono::seconds(update_time_));
        // std::cout << is_active << std::endl;
    };

    std::string AgentNetwork::toString() {
        return "<url> : " + std::to_string(this->availability) + " | inet_throughput : " + std::to_string(this->inet_throughput);
    };
}
// netstat -I en0 -b 5 | head -n 3
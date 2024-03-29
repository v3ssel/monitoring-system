#include <iostream>
#include <thread>
#include <chrono>
#include <limits>

#include "AgentNetwork.h"
#include "../AgentConfigReader.h"
#include "../../utils/Comparisons.h"
// #include "CommandCaller.h"
// #include <sys/sysctl.h>

namespace s21 {
    extern "C" AgentNetwork* create_obj() {
        return new AgentNetwork;
    }

    AgentNetwork::AgentNetwork() : Agent() {
        availability_ = 0;
        inet_throughput_ = 100;
        additional_params["url"] = "1.1.1.1";

        Agent::config_reader_ = std::make_unique<AgentConfigReader>(this);
        Agent::name = "AgentNetwork";
        Agent::type = "NET";
        Agent::config_name = ".conf" + Agent::type;

        Agent::addMetric("availability");
        Agent::addMetric("inet_throughput");
    }

    void AgentNetwork::updateMetrics() {
        // url = "translate.yandex.ru";    // считывание с config
        // std::string command = "ping -c 1 " + url + " | awk ' /^1 packets transmitted/{print $7}'";
        // std::string loss = CommandCaller::getInstance().takeValue(command);
        // availability = loss.find("0.0%") ? 0 : 1;
        
        // availability_ = ...
        if (compare_data_["availability"].compare_func(availability_, compare_data_["availability"].critical_val)) {
            Agent::observer_->NotifyCritical(this->name, "availability", std::to_string(availability_));
        }

        // inet_throughput_ = ...
        if (compare_data_["inet_throughput"].compare_func(inet_throughput_, compare_data_["inet_throughput"].critical_val)) {
            Agent::observer_->NotifyCritical(this->name, "inet_throughput", std::to_string(inet_throughput_));
        }

        observer_->NotifyResult(this->toString());
    };

    std::string AgentNetwork::toString() {
        return "<" + additional_params["url"] + "> availability : " +std::to_string(this->availability_) +
               " | inet_throughput : " + std::to_string(this->inet_throughput_);
    };
}
// netstat -I en0 -b 5 | head -n 3
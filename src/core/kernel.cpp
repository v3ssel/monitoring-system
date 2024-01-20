#include "kernel.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

namespace s21 {

Kernel::Kernel(const std::string& agents_directory,
               const std::string& logs_directory,
               const std::string& configs_directory) {
    std::cout << "Kernel start" << std::endl;

    searcher_ = std::make_unique<AgentsSearcher>(this, agents_directory, configs_directory);
    writer_ = std::make_unique<LogRecordsWriter>(logs_directory);

    record_time = 4;
    update_agents_time = 2;

    std::thread(&Kernel::searchAgents, this).detach();
    std::thread(&Kernel::makeRecords, this).detach();
}

Kernel::~Kernel() {
    for (auto& thread : threads_) {
        searcher_->getAgent(thread.first)->is_active = false;
        thread.second.join();
    }
}

void Kernel::searchAgents() {
    while (true) {
        searcher_->search();
        std::this_thread::sleep_for(std::chrono::seconds(update_agents_time));
    }
}

void Kernel::analyzeSystem(std::shared_ptr<s21::Agent> &agent) {
    while (agent->is_active) {
        agent->updateMetrics();
    }
}

void Kernel::makeRecords() {
    while (true) {
        if (qmetrics_.empty())
            continue;
        
        writer_->write(qmetrics_);
        std::this_thread::sleep_for(std::chrono::seconds(record_time));   
    }
}

void Kernel::NotifyNewAgentLoaded(const std::string &agent_name) {
    auto& agent = searcher_->getAgent(agent_name);

    agent->SetObserver(this);
    std::thread th(&Kernel::analyzeSystem, this, std::ref(agent)); 
    threads_.emplace(agent_name, (std::move(th)));
}

void Kernel::disableAgent(const std::string &agent_name) {
    if (!searcher_->getAgent(agent_name)->is_active)
        return;

    searcher_->getAgent(agent_name)->is_active = false;
    threads_[agent_name].join();
    threads_.erase(agent_name);
}

void Kernel::enableAgent(const std::string &agent_name) {
    if (searcher_->getAgent(agent_name)->is_active)
        return;
    
    searcher_->getAgent(agent_name)->is_active = true;
    threads_.emplace(agent_name, std::thread(&Kernel::analyzeSystem, this, std::ref(searcher_->getAgent(agent_name))));
}

void Kernel::NotifyResult(const std::string& result) {
    qmetrics_.push(result);
}

void Kernel::NotifyCritical(const std::string& text) {
    qcritical_values_.push(text);
}

void Kernel::NotifyError(const std::string& error) {
    qerrors_.push(error);
}

}  // namespace s21

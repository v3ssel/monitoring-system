#include "kernel.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

namespace s21 {

Kernel::Kernel(const std::string& agents_directory,
               const std::string& logs_directory,
               const std::string& configs_directory,
               const int update_agents_time) {
    logger_ = std::make_unique<ConsoleLogger>();
    searcher_ = std::make_unique<AgentsSearcher>(this, agents_directory, configs_directory);
    writer_ = std::make_unique<LogRecordsWriter>(logs_directory);

    update_agents_time_ = update_agents_time;
}

Kernel::~Kernel() {
    logger_->Log("Kernel Shutdown.", LogLevel::INFO);

    for (auto& thread : threads_) {
        searcher_->getAgent(thread.first)->is_active = false;
        thread.second.join();
    }
}

void Kernel::start() {
    logger_->Log("Kernel Start.", LogLevel::INFO);

    std::thread(&Kernel::searchAgents, this).detach();
    std::thread(&Kernel::makeRecords, this).detach();
}

void Kernel::searchAgents() {
    logger_->Log("Searcher Start.", LogLevel::INFO);

    while (true) {
        searcher_->search();

        auto awake_time = std::chrono::high_resolution_clock::now();
        while (std::chrono::high_resolution_clock::now() < (awake_time + std::chrono::seconds(update_agents_time_))) {
            continue;
        }
    }
}

void Kernel::analyzeSystem(std::shared_ptr<s21::Agent> &agent) {
    logger_->Log("Agent \"" + agent->name + "\" started.", LogLevel::INFO);

    while (agent->is_active) {
        agent->updateMetrics();
    }

    logger_->Log("Agent \"" + agent->name + "\" stopped.", LogLevel::INFO);
}

void Kernel::makeRecords() {
    logger_->Log("Recorder Start.", LogLevel::INFO);
    while (true) {
        if (qmetrics_.empty())
            continue;
        
        std::lock_guard<std::mutex> lock(qmetrics_mtx_);
        writer_->write(qmetrics_);
    }
}

void Kernel::NotifyNewAgentLoaded(const std::string &agent_name) {
    logger_->Log("Agent \"" + agent_name + "\" loaded.", LogLevel::INFO);
    auto& agent = searcher_->getAgent(agent_name);

    agent->setObserver(this);
    std::thread th(&Kernel::analyzeSystem, this, std::ref(agent)); 
    threads_.emplace(agent_name, (std::move(th)));
}

void Kernel::disableAgent(const std::string &agent_name) {
    logger_->Log("Disabling agent: \"" + agent_name + "\".", LogLevel::INFO);

    if (!searcher_->getAgent(agent_name)->is_active) {
        logger_->Log("Agent \"" + agent_name + "\" is already disabled.", LogLevel::WARNING);
        return;
    }

    searcher_->getAgent(agent_name)->is_active = false;
    threads_[agent_name].join();
    threads_.erase(agent_name);
}

void Kernel::enableAgent(const std::string &agent_name) {
    logger_->Log("Enabling agent: \"" + agent_name + "\".", LogLevel::INFO);

    if (searcher_->getAgent(agent_name)->is_active) {
        logger_->Log("Agent \"" + agent_name + "\" is already enabled.", LogLevel::WARNING);
        return;
    }
    
    searcher_->getAgent(agent_name)->is_active = true;
    threads_.emplace(agent_name, std::thread(&Kernel::analyzeSystem, this, std::ref(searcher_->getAgent(agent_name))));
}

void Kernel::changeUpdateAgentTime(int new_time) {
    update_agents_time_ = new_time;
}

void Kernel::changeSearchDirectory(const std::string &new_directory) {
    searcher_->setSearchDirectory(new_directory);
}

void Kernel::changeConfigsDirectory(const std::string &new_directory) {
    searcher_->setConfigDirectory(new_directory);
}

void Kernel::changeLogsDirectory(const std::string &new_directory) {
    writer_->setLogsDirectory(new_directory);
}

void Kernel::NotifyResult(const std::string& result) {
    std::lock_guard<std::mutex> lock(qmetrics_mtx_);
    qmetrics_.push(result);
}

void Kernel::NotifyCritical(const std::string& text) {
    logger_->Log("Critical value added: \"" + text + "\".", LogLevel::WARNING);

    std::lock_guard<std::mutex> lock(qcritical_values_mtx_);
    qcritical_values_.push(text);
}

void Kernel::NotifyError(const std::string& error) {
    logger_->Log("Error appeared: \"" + error + "\".", LogLevel::ERROR);
    
    std::lock_guard<std::mutex> lock(qerrors_mtx_);
    qerrors_.push(error);
}

std::queue<std::string> Kernel::takeCriticals() {
    std::lock_guard<std::mutex> lock(qcritical_values_mtx_);
    logger_->Log("Sending " + std::to_string(qcritical_values_.size()) + " critical messages.", LogLevel::INFO);

    std::queue<std::string> tmp = qcritical_values_;
    qcritical_values_ = std::queue<std::string>();

    return tmp;
}

std::queue<std::string> Kernel::takeErrors() {
    std::lock_guard<std::mutex> lock(qerrors_mtx_);
    logger_->Log("Sending " + std::to_string(qerrors_.size()) + " error messages.", LogLevel::INFO);

    std::queue<std::string> tmp = qerrors_;
    qerrors_ = std::queue<std::string>();

    return tmp;
}

}  // namespace s21

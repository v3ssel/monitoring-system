#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

#include "workers/ConsoleLogger.h"
#include "kernel.h"

namespace s21 {

Kernel::Kernel(const std::string& agents_directory,
               const std::string& logs_directory,
               const std::string& configs_directory,
               const int update_agents_time) {
    logger_ = std::make_unique<ConsoleLogger>();
    logger_->setEnabled(true);
    searcher_ = std::make_unique<AgentsSearcher>(this, agents_directory, configs_directory);
    writer_ = std::make_unique<LogRecordsWriter>(logs_directory);

    update_agents_time_ = update_agents_time;
    stop_searcher_ = false;
    stop_writer_ = false;
}

Kernel::~Kernel() {
    logger_->Log("Kernel Shutdown.", LogLevel::INFO);

    this->stop();
}

void Kernel::start() {
    logger_->Log("Kernel Start.", LogLevel::INFO);

    this->startSearcher();
    this->startWriter();
}

void Kernel::stop() {
    logger_->Log("Kernel Stop.", LogLevel::INFO);

    this->stopSearcher();
    this->stopWriter();    

    for (auto& thread : agents_threads_) {
        getAgentByName(thread.first)->is_active = false;
        thread.second.join();
    }

    agents_threads_.clear();
    searcher_->clearAgents();
    logger_->Log("Kernel Stop Successfuly.", LogLevel::INFO);
}

void Kernel::searchAgents() {
    while (!stop_searcher_) {
        searcher_->search();

        auto awake_time = std::chrono::high_resolution_clock::now();
        while (std::chrono::high_resolution_clock::now() < (awake_time + std::chrono::seconds(update_agents_time_))) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

void Kernel::analyzeSystem(std::shared_ptr<s21::Agent> &agent) {
    logger_->Log("Agent \"" + agent->name + "\" started.", LogLevel::INFO);

    while (agent->is_active) {
        agent->updateMetrics();

        auto awake_time = std::chrono::high_resolution_clock::now() + std::chrono::seconds(agent->getUpdateTime());
        while (std::chrono::high_resolution_clock::now() <= awake_time) {
            if (agent->is_update_time_changed) {
                agent->is_update_time_changed = false;
                break;
            }
        }
    }

    logger_->Log("Agent \"" + agent->name + "\" stopped.", LogLevel::INFO);
}

void Kernel::makeRecords() {
    while (!stop_writer_) {
        if (qmetrics_.empty())
            continue;
        
        std::lock_guard<std::mutex> lock(qmetrics_mtx_);
        writer_->write(qmetrics_);
    }
}

void Kernel::startSearcher() {
    logger_->Log("Searcher Start.", LogLevel::INFO);

    stop_searcher_ = false;
    searching_thread_ = std::thread(&Kernel::searchAgents, this);
}

void Kernel::startWriter() {
    logger_->Log("Writer Start.", LogLevel::INFO);

    stop_writer_ = false;
    records_thread_ = std::thread(&Kernel::makeRecords, this);
}

void Kernel::stopSearcher() {
    if (stop_searcher_)
        return;

    stop_searcher_ = true;
    searching_thread_.join();

    logger_->Log("Searcher Stopped.", LogLevel::INFO);
}

void Kernel::stopWriter() {
    if (stop_writer_)
        return;

    stop_writer_ = true;
    records_thread_.join();

    logger_->Log("Writer Stopped.", LogLevel::INFO);
}

void Kernel::NotifyNewAgentLoaded(const std::string &agent_name) {
    logger_->Log("Agent \"" + agent_name + "\" loaded.", LogLevel::INFO);
    auto& agent = getAgentByName(agent_name);

    agent->setObserver(this);
    std::thread th(&Kernel::analyzeSystem, this, std::ref(agent)); 
    agents_threads_.emplace(agent_name, (std::move(th)));

    agent_start_time_.emplace(agent_name, std::chrono::high_resolution_clock::now());
    active_agents_.insert(agent_name);
}

std::shared_ptr<Agent> &Kernel::getAgentByName(const std::string &agent_name) {
    return searcher_->getAgent(agent_name);
}

std::set<std::string> &Kernel::getActiveAgents() {
    return active_agents_;
}

std::chrono::milliseconds Kernel::getAgentActiveTime(const std::string &agent_name) {
    if (agent_start_time_.count(agent_name) == 0)
        return std::chrono::milliseconds(0);
    
    auto timens = std::chrono::high_resolution_clock::now() - agent_start_time_[agent_name];
    return std::chrono::duration_cast<std::chrono::milliseconds>(timens);
}

void Kernel::disableAgent(const std::string &agent_name) {
    logger_->Log("Disabling agent: \"" + agent_name + "\".", LogLevel::INFO);

    if (!getAgentByName(agent_name)->is_active) {
        logger_->Log("Agent \"" + agent_name + "\" is already disabled.", LogLevel::WARNING);
        return;
    }

    getAgentByName(agent_name)->is_active = false;
    agents_threads_[agent_name].join();
    agents_threads_.erase(agent_name);

    agent_start_time_.erase(agent_name);
    active_agents_.erase(agent_name);
}

void Kernel::enableAgent(const std::string &agent_name) {
    logger_->Log("Enabling agent: \"" + agent_name + "\".", LogLevel::INFO);

    if (getAgentByName(agent_name)->is_active) {
        logger_->Log("Agent \"" + agent_name + "\" is already enabled.", LogLevel::WARNING);
        return;
    }
    
    getAgentByName(agent_name)->is_active = true;
    agents_threads_.emplace(agent_name, std::thread(&Kernel::analyzeSystem, this, std::ref(getAgentByName(agent_name))));

    agent_start_time_.emplace(agent_name, std::chrono::high_resolution_clock::now());
    active_agents_.insert(agent_name);
}

void Kernel::changeUpdateAgentTime(int new_time) {
    update_agents_time_ = new_time;
}

void Kernel::changeSearchDirectory(const std::string &new_directory) {
    logger_->Log("Search directory changed to \"" + new_directory + "\".", LogLevel::INFO);
    searcher_->setSearchDirectory(new_directory);
}

void Kernel::changeConfigsDirectory(const std::string &new_directory) {
    logger_->Log("Configs directory changed to \"" + new_directory + "\".", LogLevel::INFO);
    searcher_->setConfigDirectory(new_directory);
}

void Kernel::changeLogsDirectory(const std::string &new_directory) {
    logger_->Log("Logs directory changed to \"" + new_directory + "\".", LogLevel::INFO);
    writer_->setLogsDirectory(new_directory);
}

void Kernel::NotifyResult(const std::string& result) {
    std::lock_guard<std::mutex> lock(qmetrics_mtx_);
    qmetrics_.push(result);
}

void Kernel::NotifyCritical(const std::string& agent_name, const std::string& metric_name, const std::string& metric_value) {
    std::string crit = "CRITICAL | " + agent_name + ": " + metric_name + " == " + metric_value;
    logger_->Log("Critical value added: \"" + crit + "\".", LogLevel::WARNING);

    std::lock_guard<std::mutex> lock(qcritical_values_mtx_);
    qcritical_values_.push(crit);
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

void Kernel::setLoggerEnabled(bool enabled) {
    logger_->setEnabled(enabled);
}

}  // namespace s21

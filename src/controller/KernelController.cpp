#include "KernelController.h"
#include "../core/agents/AgentConfigWriter.h"

namespace s21 {
    void KernelController::setKernel(Kernel *kernel) {
        kernel_ = kernel;
    }

    void KernelController::deleteKernel() {
        if (kernel_) delete kernel_;
    }

    std::shared_ptr<Agent> &KernelController::getAgentByName(const std::string &agent_name) {
        return kernel_->getAgentByName(agent_name);
    }

    std::set<std::string> &KernelController::getActiveAgents() {
        return kernel_->getActiveAgents();
    }

    std::chrono::milliseconds KernelController::getAgentActiveTime(const std::string &agent_name) {
        return kernel_->getAgentActiveTime(agent_name);
    }

    void KernelController::startKernel() {
        kernel_->start();
    }

    void KernelController::stopKernel() {
        kernel_->stop();
    }

    void KernelController::startSearcher() {
        kernel_->startSearcher();
    }

    void KernelController::startWriter() {
        kernel_->startWriter();
    }

    void KernelController::stopSearcher() {
        kernel_->stopSearcher();
    }

    void KernelController::stopWriter() {
        kernel_->stopWriter();
    }

    void KernelController::disableAgent(const std::string &agent_name) {
        kernel_->disableAgent(agent_name);
    }
    
    void KernelController::enableAgent(const std::string &agent_name) {
        kernel_->enableAgent(agent_name);
    }
    
    void KernelController::changeSearchDirectory(const std::string &new_directory) {
        kernel_->changeSearchDirectory(new_directory);
    }
    
    void KernelController::changeConfigsDirectory(const std::string &new_directory) {
        kernel_->changeConfigsDirectory(new_directory);
    }
    
    void KernelController::changeLogsDirectory(const std::string &new_directory) {
        kernel_->changeLogsDirectory(new_directory);
    }
    
    std::queue<std::string> KernelController::takeCriticals() {
        return kernel_->takeCriticals();
    }
    
    std::queue<std::string> KernelController::takeErrors() {
        return kernel_->takeErrors();
    }
    
    void KernelController::writeParamToConfig(const std::string &filename, const std::string &param_name, const std::string &param_value) {
        AgentConfigWriter::write(filename, param_name, ":", param_value);
    }
    
    void KernelController::writeCompareSignToConfig(const std::string &filename, const std::string &metric_name, const std::string &compare_sign) {
        AgentConfigWriter::write(filename, metric_name, compare_sign, "");
    }
    
    void KernelController::writeCriticalToConfig(const std::string &filename, const std::string &metric_name, const std::string &metric_value) {
        AgentConfigWriter::write(filename, metric_name, "", metric_value);
    }
}

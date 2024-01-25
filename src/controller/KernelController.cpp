#include "KernelController.h"

namespace s21 {
    void KernelController::setKernel(Kernel *kernel) {
        kernel_ = kernel;
    }

    Kernel *KernelController::getKernel() {
        return kernel_;
    }

    std::shared_ptr<Agent> &KernelController::getAgentByName(const std::string &agent_name) {
        return kernel_->getAgentByName(agent_name);
    }

    std::set<std::string> &KernelController::getActiveAgents() {
        return kernel_->getActiveAgents();
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
}

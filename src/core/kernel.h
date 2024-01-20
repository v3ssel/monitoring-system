#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <unordered_map>
#include <queue>
#include <chrono>
#include <thread>
#include <mutex>

#include "SearcherObserver.h"
#include "AgentsSearcher.h"
#include "AgentsObserver.h"
#include "AgentsFactory.h"
#include "LogRecordsWriter.h"

namespace s21 {  
    class Kernel : public SearcherObserver, public AgentsObserver {
       public:
        int update_agents_time;

        Kernel(const std::string& agents_directory = "./agents",
               const std::string& logs_directory = "./logs",
               const std::string& configs_directory = "./config");
        ~Kernel();

        void searchAgents();
        void analyzeSystem(std::shared_ptr<s21::Agent>& agent);
        void makeRecords();

        void NotifyNewAgentLoaded(const std::string& agent_name) override;

        void disableAgent(const std::string& agent_name);
        void enableAgent(const std::string& agent_name);

        void NotifyResult(const std::string& result) override;
        void NotifyCritical(const std::string& text) override;
        void NotifyError(const std::string& error) override;

        std::queue<std::string> takeCriticals();
        std::queue<std::string> takeErrors();
        
    //    private:
        std::unique_ptr<AgentsSearcher> searcher_;
        std::unique_ptr<LogRecordsWriter> writer_;

        std::unordered_map<std::string, std::thread> threads_;

        std::queue<std::string> qmetrics_, qcritical_values_, qerrors_;
        std::mutex qmetrics_mtx_, qcritical_values_mtx_, qerrors_mtx_;
    };
}

#endif  // _KERNEL_H_

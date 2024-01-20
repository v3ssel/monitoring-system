#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <unordered_map>
#include <vector>
#include <queue>
#include <chrono>
#include <thread>
#include <fstream>
#include <filesystem>
#include "SearcherObserver.h"
#include "AgentsSearcher.h"
#include "AgentsObserver.h"
#include "AgentsFactory.h"
#include "LogRecordsWriter.h"

namespace s21 {  
    class Kernel : public SearcherObserver, public AgentsObserver {
       public:
        int record_time;
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
        void NotifyError(const std::string& error) override;
        void NotifyCritical(const std::string& text) override;
        
       private:
        std::unique_ptr<AgentsSearcher> searcher_;
        std::unique_ptr<LogRecordsWriter> writer_;

        std::unordered_map<std::string, std::thread> threads_;

        std::queue<std::string> qmetrics_;
        std::queue<std::string> qcritical_values_;
        std::queue<std::string> qerrors_;
    };
}

#endif

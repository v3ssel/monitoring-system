#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <unordered_map>
#include <set>
#include <queue>
#include <chrono>
#include <thread>
#include <mutex>

#include "interfaces/SearcherObserver.h"
#include "interfaces/AgentsObserver.h"
#include "workers/AgentsSearcher.h"
#include "workers/LogRecordsWriter.h"
#include "workers/ConsoleLogger.h"

namespace s21 {  
    class Kernel : public SearcherObserver, public AgentsObserver {
       public:
        Kernel(const std::string& agents_directory = "./agents",
               const std::string& logs_directory = "./logs",
               const std::string& configs_directory = "./config",
               const int update_agents_time = 2);
        ~Kernel();

        void start();
        void stop();

        void searchAgents();
        void analyzeSystem(std::shared_ptr<s21::Agent>& agent);
        void makeRecords();

        void startSearcher();
        void startWriter();

        void stopSearcher();
        void stopWriter();

        void NotifyNewAgentLoaded(const std::string& agent_name) override;
        std::shared_ptr<Agent>& getAgentByName(const std::string& agent_name);
        std::set<std::string>& getActiveAgents();

        void disableAgent(const std::string& agent_name);
        void enableAgent(const std::string& agent_name);
        void changeUpdateAgentTime(int new_time);

        void changeSearchDirectory(const std::string& new_directory);
        void changeConfigsDirectory(const std::string& new_directory);
        void changeLogsDirectory(const std::string& new_directory);

        void NotifyResult(const std::string& result) override;
        void NotifyCritical(const std::string& text) override;
        void NotifyError(const std::string& error) override;

        std::queue<std::string> takeCriticals();
        std::queue<std::string> takeErrors();
        
       private:
        std::unique_ptr<Logger> logger_;
        std::unique_ptr<AgentsSearcher> searcher_;
        std::unique_ptr<LogRecordsWriter> writer_;

        std::set<std::string> active_agents_;
        std::unordered_map<std::string, std::thread> agents_threads_;
        std::thread searching_thread_, records_thread_;
        bool stop_searcher_, stop_writer_;

        std::queue<std::string> qmetrics_, qcritical_values_, qerrors_;
        std::mutex qmetrics_mtx_, qcritical_values_mtx_, qerrors_mtx_;

        int update_agents_time_;
    };
}

#endif  // _KERNEL_H_

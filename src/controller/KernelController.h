#ifndef _KERNELCONTROLLER_H_
#define _KERNELCONTROLLER_H_

#include "../core/Kernel.h"
#include "../notifications/CriticalNotificationEmail.h"
#include "../notifications/CriticalNotificationTelegram.h"
#include "../notifications/Notifier.h"

namespace s21 {
    class KernelController {
       public:
        static KernelController& getInstance() {
            static KernelController instance;
            return instance;
        }
        ~KernelController();

        void setKernel(Kernel* kernel);
        void deleteKernel();

        std::shared_ptr<Agent>& getAgentByName(const std::string& agent_name);
        std::set<std::string>& getActiveAgents();
        std::chrono::milliseconds getAgentActiveTime(const std::string& agent_name);

        void startKernel();
        void stopKernel();

        void startSearcher();
        void startWriter();

        void stopSearcher();
        void stopWriter();

        void disableAgent(const std::string& agent_name);
        void enableAgent(const std::string& agent_name);

        void changeSearchDirectory(const std::string& new_directory);
        void changeConfigsDirectory(const std::string& new_directory);
        void changeLogsDirectory(const std::string& new_directory);

        std::queue<std::string> takeCriticals();
        std::queue<std::string> takeErrors();

        void writeParamToConfig(const std::string& filename,
                                const std::string& param_name,
                                const std::string& param_value);

        void writeCompareSignToConfig(const std::string& filename,
                                      const std::string& metric_name,
                                      const std::string& compare_sign);

        void writeCriticalToConfig(const std::string& filename,
                                   const std::string& metric_name,
                                   const std::string& metric_value);

        void sendNotificationToTelegram(const std::string& receiver, const std::vector<std::string>& msgs);
        void sendNotificationToEmail(const std::string& receiver, const std::vector<std::string>& msgs);

       private:
        Kernel* kernel_ = nullptr;

        Notifier* notifier_ = nullptr;
        CriticalNotification* notification_strategy_ = nullptr;
        
        KernelController() = default;
        KernelController(const KernelController&) = delete;
        KernelController& operator=(const KernelController&) = delete;
    };
}

#endif  // _KERNELCONTROLLER_H_

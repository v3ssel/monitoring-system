#ifndef _KERNELCONTROLLER_H_
#define _KERNELCONTROLLER_H_

#include "../core/Kernel.h"

namespace s21 {
    class KernelController {
       public:
        static KernelController& getInstance() {
            static KernelController instance;
            return instance;
        }
       
        void setKernel(Kernel* kernel);
        std::shared_ptr<Agent>& getAgentByName(const std::string& agent_name);

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

       private:
        Kernel* kernel_ = nullptr;
        
        KernelController() = default;
        KernelController(const KernelController&) = delete;
        KernelController& operator=(const KernelController&) = delete;
    };
}

#endif  // _KERNELCONTROLLER_H_

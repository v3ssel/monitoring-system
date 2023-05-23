#ifndef __AGENTCPU_H__
#define __AGENTCPU_H__

class AgentCPU : public Agent {
    public:
        double cpu;
        int processes;
        void analyzeSystem() {

        };
        std::string toString() {
            return "cpu : " + this.cpu + " | processes : " + this.processes;
        };
};

#endif

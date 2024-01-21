CXX = g++ -g
CXXFLAGS = -std=c++17
# LIBSFLAGS = -Wl,-rpath=./agents/
PROJECTDIR = $(CURDIR)
AGENTSDIR = $(PROJECTDIR)/agents
AGENTS_SOURCES_DIR = $(PROJECTDIR)/core/agents_src

all: build

build:
	$(CXX) $(CXXFLAGS) test.cpp \
					   $(CURDIR)/core/Agent.cpp \
					   $(CURDIR)/core/ConsoleLogger.cpp \
					   $(CURDIR)/core/workers/AgentsFactory.cpp \
					   $(CURDIR)/core/workers/AgentsSearcher.cpp \
					   $(CURDIR)/core/workers/AgentConfigReader.cpp \
					   $(CURDIR)/core/workers/LogRecordsWriter.cpp \
					   $(CURDIR)/core/Kernel.cpp -o test
	@./test

AgentCPU:
	$(CXX) $(AGENTS_SOURCES_DIR)/AgentCPU.cpp -c -o AgentCPU.o
	$(CXX) $(CURDIR)/core/Agent.cpp -c -o AgentBase.o
	$(CXX) $(CURDIR)/core/workers/AgentConfigReader.cpp -c -o AgentConfigReader.o
	$(CXX) -s -shared AgentBase.o AgentCPU.o AgentConfigReader.o -o $(AGENTSDIR)/libagentCPU.so
# $(CXX) $(AGENTS_SOURCES_DIR)/commandCaller.cpp -c -o commandCaller.o
# $(CXX) -dynamiclib agentCPU.o commandCaller.o -install_name $(PWD)/agents/libagentCPU.so -o libagentCPU.so
# mv libagentCPU.so agents/

AgentMemory:
	$(CXX) $(AGENTS_SOURCES_DIR)/agentMemory.cpp -c -o agentMemory.o
	$(CXX) $(CURDIR)/core/agent.cpp -c -o agentBase.o
	$(CXX) $(AGENTS_SOURCES_DIR)/commandCaller.cpp -c -o commandCaller.o
	$(CXX) -s -shared agentBase.o agentMemory.o commandCaller.o -o $(AGENTSDIR)/libagentMemory.so
# $(CXX) -dynamiclib agentMemory.o commandCaller.o -install_name $(PWD)/agents/libagentMemory.so -o libagentMemory.so
# mv libagentMemory.so agents/

AgentNetwork:
	$(CXX) agentNetwork.cpp -c -o agentNetwork.o
	$(CXX) commandCaller.cpp -c -o commandCaller.o
	$(CXX) -dynamiclib agentNetwork.o commandCaller.o -install_name $(PWD)/agents/libagentNetwork.so -o libagentNetwork.so
	mv libagentNetwork.so agents/

clean: 
	@rm -f test  *.o ./agents/*.so ./logs/*.txt

re: clean all

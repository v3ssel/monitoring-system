CXX = g++ -g
CXXFLAGS = -std=c++17
# LIBSFLAGS = -Wl,-rpath=./agents/
PROJECTDIR = $(CURDIR)
BUILDDIR = $(CURDIR)/build
AGENTSDIR = $(PROJECTDIR)/agents
AGENTS_SOURCES_DIR = $(PROJECTDIR)/core/agents_src

all: build

build: $(BUILDDIR)-kernel
	cd $(BUILDDIR)-kernel && \
	cmake ../ -DBUILD_KERNEL=ON -G "Unix Makefiles" && \
	cmake --build . --config=Release

	$(CXX) test.cpp libMonitoringKernel.dll -o test.exe

AgentCPU: $(BUILDDIR)-agentcpu
	cd $(BUILDDIR)-agentcpu && \
	cmake ../ -DBUILD_AGENTCPU=ON -G "Unix Makefiles" && \
	cmake --build . --config=Release


AgentMemory:
	cd $(BUILDDIR)-agentcpu && \
	cmake ../ -DBUILD_AGENTMEMORY=ON -G "Unix Makefiles" && \
	cmake --build . --config=Release
# $(CXX) $(AGENTS_SOURCES_DIR)/agentMemory.cpp -c -o agentMemory.o
# $(CXX) $(CURDIR)/core/agent.cpp -c -o agentBase.o
# $(CXX) $(AGENTS_SOURCES_DIR)/commandCaller.cpp -c -o commandCaller.o
# $(CXX) -s -shared agentBase.o agentMemory.o commandCaller.o -o $(AGENTSDIR)/libagentMemory.so
# $(CXX) -dynamiclib agentMemory.o commandCaller.o -install_name $(PWD)/agents/libagentMemory.so -o libagentMemory.so
# mv libagentMemory.so agents/

AgentNetwork:
	$(CXX) agentNetwork.cpp -c -o agentNetwork.o
	$(CXX) commandCaller.cpp -c -o commandCaller.o
	$(CXX) -dynamiclib agentNetwork.o commandCaller.o -install_name $(PWD)/agents/libagentNetwork.so -o libagentNetwork.so
	mv libagentNetwork.so agents/

$(BUILDDIR)-%:
	mkdir -p $@

clean: 
	@rm -f test  *.o ./agents/*.so ./logs/*.txt

re: clean all

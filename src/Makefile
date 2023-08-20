CXX = g++
CXXFLAGS = -std=c++17

all: build

build: AgentCPU
	$(CXX) $(CXXFLAGS) test.cpp ./agents/* -o test
	@./test

AgentCPU:
	$(CXX) agentCPU.cpp -c -o agentCPU.o
	$(CXX) commandCaller.cpp -c -o commandCaller.o
	$(CXX) -dynamiclib agentCPU.o commandCaller.o -install_name $(PWD)/agents/libagentCPU.so -o libagentCPU.so
	mv libagentCPU.so agents/

AgentMemory:
	$(CXX) agentMemory.cpp -c -o agentMemory.o
	$(CXX) commandCaller.cpp -c -o commandCaller.o
	$(CXX) -dynamiclib agentMemory.o commandCaller.o -install_name @rpath/$(PWD)/agents/libagentMemory.so -o libagentMemory.so
	mv libagentMemory.so agents/

AgentNetwork:
	$(CXX) agentNetwork.cpp -c -o agentNetwork.o
	$(CXX) commandCaller.cpp -c -o commandCaller.o
	$(CXX) -dynamiclib agentNetwork.o commandCaller.o -install_name $(PWD)/agents/libagentNetwork.so -o libagentNetwork.so
	mv libagentNetwork.so agents/

clean: 
	@rm -f test  *.o ./agents/*.so

re: clean all

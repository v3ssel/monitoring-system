CXX = g++
CXXFLAGS = -std=c++17

all: build
	@./test

build: AgentCPU
	$(CXX) $(CXXFLAGS) test.cpp ./agents/* -o test

AgentCPU:
	$(CXX) -c agentCPU.cpp -o agentCPU.o
	$(CXX) -c commandCaller.cpp -o commandCaller.o
	# $(CXX) -shared -Wl,-rpath,/$(PWD)/agents/ -o libagentCPU.so agentCPU.o commandCaller.o
	$(CXX) -shared agentCPU.o commandCaller.o -install_name @rpath$(PWD)/agents/libagentCPU.so -o libagentCPU.so
	mv libagentCPU.so agents/

AgentMemory:
	$(CXX) agentMemory.cpp -c -o agentMemory.o
	# $(CXX) -dynamiclib agentCPU.o -install_name @rpath/$(PWD)/agents/libagentCPU.so -o libagentCPU.so
	$(CXX) -dynamiclib agentMemory.o -install_name @rpath/$(PWD)/agents/libagentMemory.so -o libagentMemory.so
	mv libagentMemory.so agents/

AgentNetwork:
	$(CXX) agentNetwork.cpp -c -o agentNetwork.o
	$(CXX) -dynamiclib agentNetwork.o -install_name @rpath/$(PWD)/agents/libagentNetwork.so -o libagentNetwork.so
	mv libagentNetwork.so agents/

clean: 
	@rm -f test  *.o ./agents/*.so

re: clean all

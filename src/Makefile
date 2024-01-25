CXX = g++ -g
CXXFLAGS = -std=c++17
# LIBSFLAGS = -Wl,-rpath=./agents/
PROJECTDIR = $(CURDIR)
BUILDDIR = $(CURDIR)/build
AGENTSDIR = $(PROJECTDIR)/agents
AGENTS_SOURCES_DIR = $(PROJECTDIR)/core/agents_src

all: build

install:
	cd $(BUILDDIR)-kernel && \
	cmake ../ -DINSTALLING=ON -G "Unix Makefiles" && \
	cmake --build . --config=Debug

build: $(BUILDDIR)-kernel
	cd $(BUILDDIR)-kernel && \
	cmake ../ -DINSTALLING=ON -G "Unix Makefiles" && \
	cmake --build . --config=Debug

	$(CXX) test.cpp libMonitoringKernel.dll -o test.exe

Agents: $(BUILDDIR)-agents
	cd $(BUILDDIR)-agents && \
	cmake ../ -DBUILDAGENTS=ON \
			  -DBUILD_AGENTCPU=ON \
			  -DBUILD_AGENTMEMORY=ON \
			  -DBUILD_AGENTNETWORK=ON \
			  -DBUILD_AGENTEXTRA=ON -G "Unix Makefiles" && \
	cmake --build . --config=Release

AgentCPU: $(BUILDDIR)-agents
	cd $(BUILDDIR)-agents && \
	cmake ../ -DBUILDAGENTS=ON -DBUILD_AGENTCPU=ON -G "Unix Makefiles" && \
	cmake --build . --config=Release


AgentMemory: $(BUILDDIR)-agents
	cd $(BUILDDIR)-agents && \
	cmake ../ -DBUILDAGENTS=ON -DBUILD_AGENTMEMORY=ON -G "Unix Makefiles" && \
	cmake --build . --config=Release

AgentNetwork: $(BUILDDIR)-agents
	cd $(BUILDDIR)-agents && \
	cmake ../ -DBUILDAGENTS=ON -DBUILD_AGENTNETWORK=ON -G "Unix Makefiles" && \
	cmake --build . --config=Release

AgentExtra: $(BUILDDIR)-agents
	cd $(BUILDDIR)-agents && \
	cmake ../ -DBUILDAGENTS=ON -DBUILD_AGENTEXTRA=ON -G "Unix Makefiles" && \
	cmake --build . --config=Release


tests: $(BUILDDIR)-testing
	cd $(BUILDDIR)-testing && \
	cmake ../ -DTESTING=ON -G "Unix Makefiles" && \
	cmake --build . --config=RelWithDebInfo


$(BUILDDIR)-%:
	mkdir -p $@

clean: 
	@rm -f test  *.o ./agents/*.so ./logs/*.txt

re: clean all

PROJECTDIR = $(CURDIR)
BUILDDIR = $(CURDIR)/build
AGENTSDIR = $(PROJECTDIR)/agents
AGENTS_SOURCES_DIR = $(PROJECTDIR)/core/agents_src

EXEC = MonitoringSystem.exe
TEST_EXEC = MSTests.exe

ifeq ($(shell uname), Darwin)
	EXEC = MonitoringSystem
	TEST_EXE = MSTests
	GCOVAPPLE = --ignore-errors inconsistent,mismatch
	COMPILER21 = -DCMAKE_CXX_COMPILER="/usr/local/bin/g++"
endif

ifeq ($(shell uname), Linux)
	EXEC = MonitoringSystem
	TEST_EXE = MSTests
endif


all: install Agents tests dist dvi clean

install: $(BUILDDIR)-app
	cd $(BUILDDIR)-app && \
	cmake ../ -DINSTALLING=ON -G "Unix Makefiles" && \
	cmake --build . --config=Release


uninstall: clean
	rm -rf $(EXEC) libMonitoringKernel.* $(AGENTSDIR)/*.so $(AGENTSDIR)/*.dll $(TEST_EXEC)


build: $(BUILDDIR)-kernel
	cd $(BUILDDIR)-kernel && \
	cmake ../ -DINSTALLING=ON -G "Unix Makefiles" && \
	cmake --build . --config=Debug

	g++ -g test.cpp libMonitoringKernel.dll -o test.exe


Agents: $(BUILDDIR)-agents
	cd $(BUILDDIR)-agents && \
	cmake ../ -DBUILD_AGENTS=ON \
			  -DBUILD_AGENTCPU=ON \
			  -DBUILD_AGENTMEMORY=ON \
			  -DBUILD_AGENTNETWORK=ON \
			  -DBUILD_AGENTEXTRA=ON -G "Unix Makefiles" && \
	cmake --build . --config=Release


AgentCPU: $(BUILDDIR)-agents
	cd $(BUILDDIR)-agents && \
	cmake ../ -DBUILD_AGENTS=ON -DBUILD_AGENTCPU=ON -G "Unix Makefiles" && \
	cmake --build . --config=Release


AgentMemory: $(BUILDDIR)-agents
	cd $(BUILDDIR)-agents && \
	cmake ../ -DBUILD_AGENTS=ON -DBUILD_AGENTMEMORY=ON -G "Unix Makefiles" && \
	cmake --build . --config=Release


AgentNetwork: $(BUILDDIR)-agents
	cd $(BUILDDIR)-agents && \
	cmake ../ -DBUILD_AGENTS=ON -DBUILD_AGENTNETWORK=ON -G "Unix Makefiles" && \
	cmake --build . --config=Release


AgentExtra: $(BUILDDIR)-agents
	cd $(BUILDDIR)-agents && \
	cmake ../ -DBUILD_AGENTS=ON -DBUILD_AGENTEXTRA=ON -G "Unix Makefiles" && \
	cmake --build . --config=Release


tests: $(BUILDDIR)-testing
	cd $(BUILDDIR)-testing && \
	cmake ../ -DTESTING=ON $(COMPILER21) -G "Unix Makefiles" && \
	cmake --build . --config=RelWithDebInfo


dist:


dvi:


$(BUILDDIR)-%:
	mkdir -p $@


clean: 
	@rm -rf build*


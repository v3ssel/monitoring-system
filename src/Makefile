PROJECTDIR = $(CURDIR)
BUILDDIR = $(CURDIR)/build
AGENTSDIR = $(PROJECTDIR)/agents
AGENTS_SOURCES_DIR = $(PROJECTDIR)/core/agents_src
DISTPATH = monitoring-system-archive

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


all: install agents tests dist dvi clean


install: $(BUILDDIR)-app
	cd $(BUILDDIR)-app && \
	cmake ../ -DINSTALLING=ON -G "Unix Makefiles" && \
	cmake --build . --config=Release


uninstall: clean
	rm -rf $(EXEC) libMonitoringKernel.* $(AGENTSDIR)/*.so $(AGENTSDIR)/*.dll $(TEST_EXEC)


agents: $(BUILDDIR)-agents
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
	mkdir -p $(DISTPATH)
	cp -R core $(DISTPATH)/
	cp -R notifications $(DISTPATH)/
	cp -R config $(DISTPATH)/
	cp -R controller $(DISTPATH)/
	cp -R gui $(DISTPATH)/
	cp CMakeLists.txt $(DISTPATH)
	tar -cf $(DISTPATH).tar $(DISTPATH)
	gzip -9f $(DISTPATH).tar
	rm -rf $(DISTPATH)


dvi:
	open README.md


$(BUILDDIR)-%:
	mkdir -p $@


clean: 
	@rm -rf build* $(DISTPATH)*


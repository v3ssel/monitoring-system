cmake_minimum_required(VERSION 3.5)
project(MonitoringKernel VERSION 0.1 LANGUAGES CXX)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
# add_compile_options(-Wall -Werror -Wextra)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(BUILD_AGENTCPU     OFF)
option(BUILD_AGENTMEMORY  OFF)
option(BUILD_AGENTNETWORK OFF)
option(BUILD_AGENTEXTRA   OFF)
option(BUILD_KERNEL       OFF)

set(COMMON_SOURCES
    agents/Agent.h
    agents/Agent.cpp
    agents/AgentConfigReader.h
    agents/AgentConfigReader.cpp
    # agents/CommandCaller.h
    # agents/CommandCaller.cpp

    interfaces/ConfigReader.h
    interfaces/Observer.h
    interfaces/AgentsObserver.h

    utils/Comparisons.h
)

if(BUILD_AGENTCPU)
    set(AGENTCPU_SOURCES
        ${COMMON_SOURCES}
        agents/examples/AgentCPU.h
        agents/examples/AgentCPU.cpp
    )

    add_library(AgentCPU SHARED
        ${AGENTCPU_SOURCES}
    )

    set_target_properties(AgentCPU PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/agents
    )
endif()

if(BUILD_AGENTMEMORY)
    set(AGENTMEMORY_SOURCES
        ${COMMON_SOURCES}
        agents/examples/AgentMemory.h
        agents/examples/AgentMemory.cpp
    )

    add_library(AgentMemory SHARED
        ${AGENTMEMORY_SOURCES}
    )

    set_target_properties(AgentMemory PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/agents
    )
endif()

if(BUILD_AGENTNETWORK)
    set(AGENTNETWORK_SOURCES
        ${COMMON_SOURCES}
        agents/examples/AgentNetwork.h
        agents/examples/AgentNetwork.cpp
    )

    add_library(AgentNetwork SHARED
        ${AGENTNETWORK_SOURCES}
    )

    set_target_properties(AgentNetwork PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/agents
    )
endif()

if(BUILD_AGENTEXTRA)
    set(AGENTEXTRA_SOURCES
        ${COMMON_SOURCES}
        agents/examples/AgentExtra.h
        agents/examples/AgentExtra.cpp
    )

    add_library(AgentExtra SHARED
        ${AGENTEXTRA_SOURCES}
    )

    set_target_properties(AgentExtra PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/agents
    )
endif()

if(BUILD_KERNEL)
    set(KERNEL_SOURCES
        ${COMMON_SOURCES}
        Kernel.h
        Kernel.cpp
        ConsoleLogger.h
        ConsoleLogger.cpp

        interfaces/Logger.h
        interfaces/RecordsWriter.h
        interfaces/SearcherObserver.h

        workers/AgentsFactory.h
        workers/AgentsFactory.cpp
        workers/AgentsSearcher.h
        workers/AgentsSearcher.cpp
        workers/LogRecordsWriter.h
        workers/LogRecordsWriter.cpp
    )

    add_library(MonitoringKernel SHARED
        ${KERNEL_SOURCES}
    )

    set_target_properties(MonitoringKernel PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR} 
    )
endif()

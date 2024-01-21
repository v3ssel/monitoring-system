cmake_minimum_required(VERSION 3.5)
project(MonitoringKernel VERSION 0.1 LANGUAGES CXX)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Werror -Wextra)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(BUILD_AGENTCPU OFF)
option(BUILD_AGENTMEM OFF)
option(BUILD_AGENTNET OFF)
option(BUILD_AGENTEXT OFF)
option(BUILD_KERNEL   OFF)

set(COMMON_SOURCES
    Agent.h
    Agent.cpp
    workers/AgentConfigReader.h
    workers/AgentConfigReader.cpp
)

if(BUILD_AGENTCPU)
    set(AGENTCPU_SOURCES
        ${COMMON_SOURCES}
        agents_src/AgentCPU.h
        agents_src/AgentCPU.cpp
    )

    add_library(AgentCPU SHARED
        ${AGENTCPU_SOURCES}
    )

    set_target_properties(AgentCPU PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/agents
    )
endif()

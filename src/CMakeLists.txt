cmake_minimum_required(VERSION 3.5)
project(MonitoringSystemProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
option(INSTALLING  OFF)
option(TESTING     OFF)
option(BUILD_AGENTS OFF)

if (BUILD_AGENTS)
    add_subdirectory(core)
endif()

if (INSTALLING)
    set(BUILD_MSKERNEL ON)
    add_subdirectory(gui)
endif()

if (TESTING)
    set(BUILD_MSKERNEL ON)
    add_subdirectory(tests)
endif()

cmake_minimum_required(VERSION 3.5)
project(MonitoringSystemProject VERSION 0.1 LANGUAGES CXX)

option(INSTALLING  OFF)
option(TESTING     OFF)
option(BUILDAGENTS OFF)

if (BUILDAGENTS)
    add_subdirectory(core)
endif()

if (INSTALLING)
    set(BUILD_KERNEL ON)
    add_subdirectory(gui)
endif()

if (TESTING)
    set(BUILD_KERNEL ON)
    add_subdirectory(tests)
endif()

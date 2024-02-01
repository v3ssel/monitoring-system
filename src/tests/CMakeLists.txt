cmake_minimum_required(VERSION 3.5)
project(MSTests VERSION 0.1 LANGUAGES CXX)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
# add_compile_options(-Wall -Werror -Wextra)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_definitions(-DGTEST_LANGUAGE_CXX17)

add_subdirectory(../core ${CMAKE_BINARY_DIR}/core)

add_executable(${PROJECT_NAME}
    main_tests.cpp
    Constants.h
    KernelTests.cpp
    AgentsSearcherTests.cpp
    AgentsFactoryTests.cpp
    AgentConfigReaderTests.cpp
    AgentConfigWriterTests.cpp
    LogRecordsWriterTests.cpp
    ConsoleLoggerTests.cpp
)

target_link_libraries(${PROJECT_NAME}
    ${GTEST_LIBRARIES}
    ${GTEST_MAIN_LIBRARIES}
    MonitoringKernel
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR} 
)


#include <filesystem>
#include <sstream>
#include <gtest/gtest.h>
#include "../core/Kernel.h"
#include "Constants.h"


TEST(Kernel, Run) {
    std::stringstream ss;
    std::streambuf* cout_buf = std::cout.rdbuf(ss.rdbuf());
    
    s21::Kernel kernel;
    kernel.start();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    kernel.stop();

    EXPECT_NO_THROW(kernel.takeErrors());
    EXPECT_NO_THROW(kernel.takeCriticals());

    EXPECT_FALSE(ss.str().empty());
    std::cout.rdbuf(cout_buf);
    kernel.setLoggerEnabled(false);
}

TEST(Kernel, RunWithStops) {
    std::stringstream ss;
    std::streambuf* cout_buf = std::cout.rdbuf(ss.rdbuf());

    s21::Kernel kernel;
    kernel.start();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    kernel.stopWriter();
    kernel.stopSearcher();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    kernel.changeUpdateAgentTime(1);
    kernel.startSearcher();
    kernel.startWriter();
    kernel.stop();

    kernel.start();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    kernel.stop();
    
    EXPECT_NO_THROW(kernel.takeErrors());
    EXPECT_NO_THROW(kernel.takeCriticals());
    
    EXPECT_FALSE(ss.str().empty());
    std::cout.rdbuf(cout_buf);
    kernel.setLoggerEnabled(false);
}

TEST(Kernel, EnablingAgents) {
    std::stringstream ss;
    std::streambuf* cout_buf = std::cout.rdbuf(ss.rdbuf());
    
    s21::Kernel kernel;
    kernel.changeUpdateAgentTime(1);
    kernel.start();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto agents = kernel.getActiveAgents();
    for (auto& agent : agents) {
        EXPECT_NE(kernel.getAgentActiveTime(agent).count(), 0);
        kernel.disableAgent(agent);
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    for (auto& agent : agents) {
        kernel.enableAgent(agent);
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    kernel.stop();
    
    EXPECT_NO_THROW(kernel.takeErrors());
    EXPECT_NO_THROW(kernel.takeCriticals());
    
    EXPECT_FALSE(ss.str().empty());
    std::cout.rdbuf(cout_buf);
    kernel.setLoggerEnabled(false);
}

TEST(Kernel, NonExistsDirs) {
    std::stringstream ss;
    std::streambuf* cout_buf = std::cout.rdbuf(ss.rdbuf());
    
    s21::Kernel kernel;
    kernel.changeUpdateAgentTime(1);
    kernel.start();
    kernel.changeConfigsDirectory("non-existing-dir");
    kernel.changeLogsDirectory("non-existing-dir");
    kernel.changeSearchDirectory("non-existing-dir");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    EXPECT_FALSE(kernel.takeErrors().empty());
    
    EXPECT_FALSE(ss.str().empty());
    std::cout.rdbuf(cout_buf);
    kernel.setLoggerEnabled(false);
}

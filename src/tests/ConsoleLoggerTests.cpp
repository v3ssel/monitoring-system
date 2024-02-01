#include <filesystem>
#include <memory>
#include <sstream>
#include <gtest/gtest.h>
#include "../core/workers/ConsoleLogger.h"

TEST(ConsoleLogger, Log) {
    std::unique_ptr<s21::Logger> logger = std::make_unique<s21::ConsoleLogger>();

    std::stringstream redirect_stream;
    std::streambuf* cout_buf = std::cout.rdbuf(redirect_stream.rdbuf());

    logger->setEnabled(true);
    logger->Log("InfoTest", s21::LogLevel::INFO);
    logger->Log("WarningTest", s21::LogLevel::WARNING);
    logger->Log("ErrorTest", s21::LogLevel::ERROR);

    EXPECT_NE(redirect_stream.str().find("InfoTest"), std::string::npos);
    EXPECT_NE(redirect_stream.str().find("WarningTest"), std::string::npos);
    EXPECT_NE(redirect_stream.str().find("ErrorTest"), std::string::npos);

    std::cout.rdbuf(cout_buf);
}

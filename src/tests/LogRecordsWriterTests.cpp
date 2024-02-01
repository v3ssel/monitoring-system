#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include "../core/workers/LogRecordsWriter.h"

TEST(LogRecordsWriter, Write) {
    s21::LogRecordsWriter writer("");

    std::queue<std::string> test_records;
    test_records.push("test-test-test-test");

    std::filesystem::path path = std::filesystem::current_path();
    writer.setLogsDirectory((path / "logs").string());
    writer.write(test_records);

    std::time_t t = std::time(nullptr);
    std::tm* lt = std::localtime(&t);
    char date_buf[10];
    strftime(date_buf, 10, "%y-%m-%d", lt);

    std::ifstream file((path / "logs" / (std::string(date_buf) + ".txt")).string());
    bool found = false;
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("test-test-test-test") != std::string::npos) {
            found = true;
            break;
        }
    }

    EXPECT_TRUE(found);
    EXPECT_TRUE(test_records.empty());
}

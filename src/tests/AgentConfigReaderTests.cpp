#include <filesystem>
#include <gtest/gtest.h>
#include "../core/agents/AgentConfigReader.h"
#include "../core/workers/AgentsFactory.h"
#include "Constants.h"

TEST(AgentConfigReader, Read) {
    std::filesystem::path agent_path = std::filesystem::current_path() / "agents" / Constants::agent_file;
    auto agent = s21::AgentsFactory::GetInstance().LoadAgent(agent_path.string());
    agent->config_name = ".confTST";
    agent->metrics_names.clear();
    agent->metrics_names.push_back("gt");
    agent->metrics_names.push_back("gteq");
    agent->metrics_names.push_back("lt");
    agent->metrics_names.push_back("lteq");
    agent->metrics_names.push_back("eq");
    agent->metrics_names.push_back("neq");

    std::unique_ptr<s21::ConfigReader> reader = std::make_unique<s21::AgentConfigReader>(agent.get());
    std::filesystem::path config_dir = std::filesystem::current_path() / "tests";
    reader->read(config_dir.string());

    EXPECT_EQ(agent->name, "AgentTest");
    EXPECT_EQ(agent->type, "TST");
    EXPECT_EQ(agent->getUpdateTime(), 10);
    EXPECT_EQ(agent->additional_params["additional"], "some text");

    EXPECT_EQ(agent->getCompareFncType("gt"), s21::CompareType::IS_GREATER);
    EXPECT_EQ(agent->getCompareFncType("gteq"), s21::CompareType::IS_GREATER_OR_EQ);
    EXPECT_EQ(agent->getCompareFncType("lt"), s21::CompareType::IS_LESS);
    EXPECT_EQ(agent->getCompareFncType("lteq"), s21::CompareType::IS_LESS_OR_EQ);
    EXPECT_EQ(agent->getCompareFncType("eq"), s21::CompareType::IS_EQ);
    EXPECT_EQ(agent->getCompareFncType("neq"), s21::CompareType::IS_NOT_EQ);

    EXPECT_EQ(agent->getCriticalValue("gt"), 10);
    EXPECT_EQ(agent->getCriticalValue("gteq"), 15);
    EXPECT_EQ(agent->getCriticalValue("lt"), 20);
    EXPECT_EQ(agent->getCriticalValue("lteq"), 25);
    EXPECT_EQ(agent->getCriticalValue("eq"), 30);
    EXPECT_EQ(agent->getCriticalValue("neq"), 35);
}

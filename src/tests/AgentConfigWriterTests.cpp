#include <filesystem>
#include <gtest/gtest.h>
#include "../core/workers/AgentsFactory.h"
#include "../core/agents/AgentConfigReader.h"
#include "../core/agents/AgentConfigWriter.h"
#include "Constants.h"

static std::shared_ptr<s21::Agent> getAgent() {
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

    return agent;
}

TEST(AgentConfigWriter, WriteParameter) {
    auto agent = getAgent();

    std::unique_ptr<s21::ConfigReader> reader = std::make_unique<s21::AgentConfigReader>(agent.get());
    std::filesystem::path config_dir = std::filesystem::current_path() / "tests";
    reader->read(config_dir.string());

    std::string prev_val = agent->additional_params["additional"];
    s21::AgentConfigWriter::write((config_dir / agent->config_name).string(), "additional", "", "replaced text");
    reader->read(config_dir.string());
    EXPECT_EQ(agent->additional_params["additional"], "replaced text");

    s21::AgentConfigWriter::write((config_dir / agent->config_name).string(), "additional", "", prev_val);
    reader->read(config_dir.string());
    EXPECT_EQ(agent->additional_params["additional"], prev_val);
}

TEST(AgentConfigWriter, WriteMetricSign) {
    auto agent = getAgent();

    std::unique_ptr<s21::ConfigReader> reader = std::make_unique<s21::AgentConfigReader>(agent.get());
    std::filesystem::path config_dir = std::filesystem::current_path() / "tests";
    reader->read(config_dir.string());

    s21::AgentConfigWriter::write((config_dir / agent->config_name).string(), "lt", "==", "");
    reader->read(config_dir.string());
    EXPECT_EQ(agent->getCompareFncType("lt"), s21::CompareType::IS_EQ);

    s21::AgentConfigWriter::write((config_dir / agent->config_name).string(), "lt", "<", "");
    reader->read(config_dir.string());
    EXPECT_EQ(agent->getCompareFncType("lt"), s21::CompareType::IS_LESS);
}

TEST(AgentConfigWriter, WriteMetricValue) {
    auto agent = getAgent();

    std::unique_ptr<s21::ConfigReader> reader = std::make_unique<s21::AgentConfigReader>(agent.get());
    std::filesystem::path config_dir = std::filesystem::current_path() / "tests";
    reader->read(config_dir.string());

    double prev_val = agent->getCriticalValue("neq");
    s21::AgentConfigWriter::write((config_dir / agent->config_name).string(), "neq", "", "455");
    reader->read(config_dir.string());
    EXPECT_EQ(agent->getCriticalValue("neq"), 455);

    s21::AgentConfigWriter::write((config_dir / agent->config_name).string(), "neq", "", std::to_string(prev_val));
    reader->read(config_dir.string());
    EXPECT_EQ(agent->getCriticalValue("neq"), prev_val);
}

#include <filesystem>
#include <gtest/gtest.h>
#include "../core/workers/AgentsFactory.h"
#include "Constants.h"

TEST(AgentsFactory, LoadAgent) {
    std::filesystem::path agent_path = std::filesystem::current_path() / "agents" / Constants::agent_file;
    auto agent = s21::AgentsFactory::GetInstance().LoadAgent(agent_path.string());

    EXPECT_NE(agent, nullptr);
    EXPECT_EQ(agent->name, Constants::agent_name);
}

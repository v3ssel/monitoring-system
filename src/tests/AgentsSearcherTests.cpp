#include <filesystem>
#include <gtest/gtest.h>
#include "../core/workers/AgentsSearcher.h"

class TestObsvr : public s21::SearcherObserver {
   public:
    void NotifyError(const std::string& message) override {
        last_err = message;
    }

    void NotifyNewAgentLoaded(const std::string& agent_name) override {
        names.insert(agent_name);
    }

    std::set<std::string> names;
    std::string last_err;
};

TEST(AgentSearcher, Search) {
    std::filesystem::path path = std::filesystem::current_path();
    
    TestObsvr obsvr;
    s21::AgentsSearcher searcher(&obsvr, (path / "agents").string(), (path / "config").string());

    searcher.search();

    EXPECT_EQ(obsvr.names.count("libAgentCPU.dll"), 1);
    EXPECT_TRUE(searcher.getAgent("libAgentCPU.dll")->is_active);
    EXPECT_TRUE(obsvr.last_err.empty());
}

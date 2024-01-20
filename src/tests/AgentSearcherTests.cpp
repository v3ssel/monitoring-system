#include <filesystem>
#include <gtest/gtest.h>
#include "../core/workers/AgentsSearcher.h"

class TestObsvr : public s21::SearcherObserver {
   public:
    void NotifyNewAgentLoaded(const std::string& agent_name) override {
        names.insert(agent_name);
    }

    std::set<std::string> names;
};

TEST(AgentSearcher, Search) {
    std::filesystem::path path = std::filesystem::current_path();
    
    TestObsvr obsvr;
    s21::AgentsSearcher searcher(&obsvr, (path / "agents").string(), (path / "config").string());

    searcher.search();

    EXPECT_EQ(obsvr.names.count("libagentCPU.so"), 1);
    EXPECT_TRUE(searcher.getAgent("libagentCPU.so")->is_active);
}

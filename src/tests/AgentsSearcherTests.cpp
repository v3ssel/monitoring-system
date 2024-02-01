#include <filesystem>
#include <gtest/gtest.h>
#include "../core/workers/AgentsSearcher.h"
#include "../core/interfaces/AgentsObserver.h"
#include "Constants.h"

class TestObsvr : public s21::SearcherObserver, public s21::AgentsObserver {
   public:
    void NotifyError(const std::string& message) override {
        last_err = message;
    }

    void NotifyNewAgentLoaded(const std::string& agent_name) override {
        names.insert(agent_name);
    }

    void NotifyResult(const std::string& message) override {}
    void NotifyCritical(const std::string& agent_name, const std::string& metric_name, const std::string& metric_value) override {}

    std::set<std::string> names;
    std::string last_err;
};

TEST(AgentSearcher, Search) {
    std::filesystem::path path = std::filesystem::current_path();
    
    TestObsvr obsvr;
    s21::AgentsSearcher searcher(&obsvr, (path / "agents").string(), (path / "config").string());

    searcher.search();

    EXPECT_EQ(obsvr.names.count(Constants::agent_file), 1);
    EXPECT_TRUE(searcher.getAgent(Constants::agent_file)->is_active);
    EXPECT_TRUE(obsvr.last_err.empty());
}

TEST(AgentSearcher, NonExistSearchDirectory) {
    std::filesystem::path path = std::filesystem::current_path();
    
    TestObsvr obsvr;
    s21::AgentsSearcher searcher(&obsvr, (path / "agents").string(), (path / "config").string());
    searcher.setSearchDirectory("non-exists-dir-name");

    searcher.search();

    EXPECT_FALSE(obsvr.last_err.empty());
}

TEST(AgentSearcher, NonExistConfigDirectory) {
    std::filesystem::path path = std::filesystem::current_path();
    
    TestObsvr obsvr;
    s21::AgentsSearcher searcher(&obsvr, (path / "agents").string(), (path / "config").string());

    searcher.search();
    searcher.getAgent(Constants::agent_file)->setObserver(&obsvr);
    searcher.setConfigDirectory("non-exists-dir-name");
    searcher.search();

    EXPECT_FALSE(obsvr.last_err.empty());
}

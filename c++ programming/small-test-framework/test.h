#pragma once
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <stdexcept>
#include <cstring>

class AbstractTest {
public:
    virtual void SetUp() = 0;
    virtual void TearDown() = 0;
    virtual void Run() = 0;
    virtual ~AbstractTest() {
    }
};

class AbstractTestProducer {
public:
    virtual std::unique_ptr<AbstractTest> Produce() = 0;
};

template <typename Test>
class TestProducer : public AbstractTestProducer {
public:
    std::unique_ptr<AbstractTest> Produce() override {
        auto res = std::make_unique<Test>();
        return res;
    }
};

class Pred {
public:
    virtual bool operator()(const std::string& test) const = 0;
};

class FullMatch : Pred {
public:
    FullMatch(const std::string& pred) {
        pred_ = pred;
    }

    bool operator()(const std::string& test) const override {
        if (pred_ == test) {
            return true;
        }
        return false;
    }

private:
    std::string pred_;
};

class Substr : public Pred {
public:
    Substr(const std::string& pred) {
        pred_ = pred;
    }

    bool operator()(const std::string& test) const override {
        if (strstr(test.data(), pred_.data())) {
            return true;
        }
        return false;
    }

private:
    std::string pred_;
};

class TestRegistry {
public:
    template <class TestClass>
    void RegisterClass(const std::string& class_name) {
        tests_[class_name] = std::make_unique<TestProducer<TestClass>>();
    }

    static TestRegistry& Instance() {
        static TestRegistry instance;
        return instance;
    }

    void RunTest(const std::string& test_name) {
        std::unique_ptr<AbstractTest> cur_test;
        cur_test = CreateTest(test_name);
        cur_test->SetUp();
        try {
            cur_test->Run();
        } catch (...) {
            cur_test->TearDown();
            throw;
        }
        cur_test->TearDown();
    }

    std::unique_ptr<AbstractTest> CreateTest(const std::string& class_name) {
        if (tests_.find(class_name) == tests_.end()) {
            throw std::out_of_range("");
        } else {
            return tests_[class_name]->Produce();
        }
    }

    template <class Predicate>
    void RunTests(Predicate callback) {
        for (auto& [key, value] : tests_) {
            if (callback(key)) {
                RunTest(key);
            } else {
                continue;
            }
        }
    }

    std::vector<std::string> ShowAllTests() const {
        std::vector<std::string> ans;
        for (auto& test : tests_) {
            if (test.first.empty()) {
                return ans;
            }
            ans.push_back(test.first);
        }
        return ans;
    }

    template <class Predicate>
    std::vector<std::string> ShowTests(Predicate callback) const {
        std::vector<std::string> ans;
        if (tests_.empty()) {
            return ans;
        } else {
            ans.reserve(tests_.size());
        }
        for (auto& test : tests_) {
            if (callback(test.first)) {
                if (test.first.empty()) {
                    return ans;
                }
                ans.push_back(test.first);
            }
        }
        return ans;
    }

    void Clear() {
        tests_.clear();
    }

private:
    std::map<std::string, std::unique_ptr<AbstractTestProducer>> tests_;
};

#pragma once

#include <list>
#include <string>
#include <unordered_map>

class LruCache {
private:
    std::list<std::pair<std::string, std::string>> values_;
    std::unordered_map<std::string, std::list<std::pair<std::string, std::string>>::iterator> refs_;
    size_t size_;

public:
    LruCache(size_t max_size) {
        size_ = max_size;
    }

    void Set(const std::string& key, const std::string& value) {
        auto res = refs_.find(key);
        if (res != refs_.end()) {
            auto iter = ((*res).second);
            values_.erase(iter);
        } else if (values_.size() == size_) {
            auto temp = *(--values_.end());
            refs_.erase(temp.first);
            values_.pop_back();
        }
        values_.push_front(std::make_pair(key, value));
        refs_[key] = values_.begin();
    }

    bool Get(const std::string& key, std::string* value) {
        auto res = refs_.find(key);
        if (res == refs_.end()) {
            return false;
        } else {
            auto iter = ((*res).second);
            values_.push_front(*iter);
            refs_[key] = values_.begin();
            values_.erase(iter);
            *value = (*values_.begin()).second;
            return true;
        }
        return false;
    }
};

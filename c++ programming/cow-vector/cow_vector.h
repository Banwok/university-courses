#pragma once

struct State {
    int ref_count;
    std::string* str_mem;
    size_t size_;
};

class COWVector {
private:
    State* state_;

public:
    COWVector() {
        state_ = new State{1, nullptr, 0};
    }

    ~COWVector() {
        if (state_->ref_count == 1) {
            if (state_->str_mem) {
                delete[] state_->str_mem;
            }
            delete state_;
        } else {
            state_->ref_count--;
        }
    }

    COWVector(const COWVector& other) {
        state_ = other.state_;
        ++state_->ref_count;
    }

    COWVector& operator=(const COWVector& other) {
        if (state_->ref_count == 1) {
            if (state_->str_mem) {
                delete[] state_->str_mem;
            }
            delete state_;
        } else {
            state_->ref_count--;
        }
        state_ = other.state_;
        ++state_->ref_count;
        return *this;
    }

    size_t Size() const {
        return state_->size_;
    }

    void Resize(size_t size) {
        if (size <= state_->size_) {
            return;
        }
        std::string* new_mem = new std::string[size];
        for (size_t i = 0; i < state_->size_; ++i) {
            new_mem[i] = state_->str_mem[i];
        }

        if (state_->ref_count != 1) {
            --state_->ref_count;
            state_ = new State{1, new_mem, size};
        } else {
            delete[] state_->str_mem;
            state_->str_mem = new_mem;
            state_->size_ = size;
        }
    }

    const std::string& Get(size_t at) {
        return state_->str_mem[at];
    }
    const std::string& Back() {
        return state_->str_mem[state_->size_ - 1];
    }

    void PushBack(const std::string& value) {
        Resize(state_->size_ + 1);
        state_->str_mem[state_->size_ - 1] = value;
    }

    void Set(size_t at, const std::string& value) {
        if (state_->ref_count != 1) {
            --state_->ref_count;
            state_ = CopyState();
        }
        state_->str_mem[at] = value;
    }

    State* CopyState() {
        std::string* new_mem = new std::string[state_->size_];
        for (size_t i = 0; i < state_->size_; ++i) {
            new_mem[i] = state_->str_mem[i];
        }
        return new State{1, new_mem, state_->size_};
    }
};
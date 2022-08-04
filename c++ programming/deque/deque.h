#pragma once

#include <initializer_list>

const size_t kBlocksize = 128;

struct Block {
    int* memory_;

    Block() {
        memory_ = new int[kBlocksize];
    }

    int& operator[](size_t ind) {
        return memory_[ind];
    }

    int operator[](size_t ind) const {
        return memory_[ind];
    }

    Block& operator=(Block& rhs) {
        for (size_t i = 0; i < kBlocksize; ++i) {
            memory_[i] = rhs.memory_[i];
        }
        return *this;
    }

    Block& operator=(const int val) {
        for (size_t i = 0; i < kBlocksize; ++i) {
            memory_[i] = val;
        }
        return *this;
    }

    ~Block() {
        delete[] memory_;
    }
};

class Deque {
public:
    Deque() {
        NullDeq();
    }

    Deque(const Deque& rhs) {
        begin_ = 0;
        size_ = rhs.size_;
        alloc_ = size_ / kBlocksize + static_cast<bool>(size_ % kBlocksize);
        NewBlock(rhs);
    }

    void Swap(Deque& rhs) {
        std::swap(mem_, rhs.mem_);
        std::swap(size_, rhs.size_);
        std::swap(begin_, rhs.begin_);
        std::swap(alloc_, rhs.alloc_);
    }

    Deque(Deque&& rhs) {
        Swap(rhs);
        rhs.mem_ = nullptr;
        rhs.alloc_ = 0;
    }

    explicit Deque(size_t size) {
        begin_ = 0;
        size_ = size;
        alloc_ = size_ / kBlocksize + static_cast<bool>(size_ % kBlocksize);
        mem_ = new Block*[alloc_];
        for (size_t i = 0; i < alloc_; ++i) {
            mem_[i] = new Block();
            (*mem_[i]) = 0;
        }
    }

    Deque(std::initializer_list<int> list) : size_(list.size()) {
        begin_ = 0;
        alloc_ = size_ / kBlocksize + static_cast<bool>(size_ % kBlocksize);
        mem_ = new Block*[alloc_];
        for (size_t i = 0; i < alloc_; ++i) {
            mem_[i] = new Block();
        }
        size_t count = 0;
        for (auto& i : list) {
            (*this)[count] = i;
            ++count;
        }
    }

    ~Deque() {
        for (size_t i = 0; i < alloc_; ++i) {
            delete mem_[i];
        }
        if (mem_) {
            delete[] mem_;
        }
    }

    Deque& operator=(Deque&& rhs) {
        Clear();
        Swap(rhs);
        rhs.mem_ = nullptr;
        rhs.alloc_ = 0;
        return *this;
    }

    void NewBlock(const Deque& rhs) {
        mem_ = new Block*[alloc_];
        for (size_t i = 0; i < alloc_; ++i) {
            mem_[i] = new Block();
            *(mem_[i]) = *(rhs.mem_[i]);
        }
    }

    Deque& operator=(const Deque& rhs) {
        if (&rhs == this) {
            return *this;
        }
        Clear();
        begin_ = 0;
        size_ = rhs.size_;
        alloc_ = size_ / kBlocksize + static_cast<bool>(size_ % kBlocksize);
        NewBlock(rhs);
        return *this;
    }

    void AddBlBack(size_t block_count) {
        auto new_data = new Block*[alloc_ + block_count];
        for (size_t i = 0; i < alloc_; ++i) {
            new_data[i] = mem_[i];
        }
        for (size_t i = alloc_; i < alloc_ + block_count; ++i) {
            new_data[i] = new Block();
            *(new_data[i]) = 0;
        }
        if (mem_) {
            delete[] mem_;
        }
        mem_ = new_data;
        alloc_ += block_count;
    }

    void AddBlFront(size_t block_count) {
        auto new_data = new Block*[alloc_ + block_count];
        for (size_t i = 0; i < block_count; ++i) {
            new_data[i] = new Block();
            *(new_data[i]) = 0;
        }
        for (size_t i = 0; i < alloc_; ++i) {
            new_data[block_count + i] = mem_[i];
        }
        if (mem_) {
            delete[] mem_;
        }
        begin_ += block_count * kBlocksize;
        mem_ = new_data;
        alloc_ += block_count;
    }

    void PushBack(int value) {
        if (begin_ + size_ == alloc_ * kBlocksize) {
            if (alloc_ == 0) {
                AddBlBack(1);
            } else {
                AddBlBack(alloc_);
            }
        }
        (*this)[size_] = value;
        ++size_;
    }

    void PushFront(int value) {
        if (begin_ == 0) {
            if (alloc_ == 0) {
                AddBlFront(1);
            } else {
                AddBlFront(alloc_);
            }
        }
        --begin_;
        (*this)[0] = value;
        ++size_;
    }

    void PopBack() {
        size_--;
    }

    void PopFront() {
        ++begin_;
        --size_;
    }

    int& operator[](size_t ind) {
        size_t block = (begin_ + ind) / kBlocksize;
        size_t offset = (begin_ + ind) % kBlocksize;
        return (*mem_[block])[offset];
    }

    int operator[](size_t ind) const {
        size_t block = (begin_ + ind) / kBlocksize;
        size_t offset = (begin_ + ind) % kBlocksize;
        return (*mem_[block])[offset];
    }

    size_t Size() const {
        return size_;
    }

    void NullDeq() {
        mem_ = nullptr;
        begin_ = 0;
        alloc_ = 0;
        size_ = 0;
    }

    void Clear() {
        for (size_t i = 0; i < alloc_; ++i) {
            delete mem_[i];
        }
        if (mem_) {
            delete[] mem_;
        }
        NullDeq();
    }

private:
    Block** mem_;
    size_t size_;
    size_t begin_;
    size_t alloc_;
};
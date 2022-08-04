#pragma once

#include <string>

class WeakPtr;

class SharedPtr {
public:
    SharedPtr() {
        ptr_ = nullptr;
        counter_ = nullptr;
    }

    SharedPtr(const SharedPtr& rhs) {
        if (rhs.ptr_ != nullptr) {
            ptr_ = rhs.ptr_;
            counter_ = rhs.counter_;
            if (ptr_ != nullptr) {
                ++(*counter_);
            }
        } else {
            ptr_ = nullptr;
            counter_ = nullptr;
        }
    }

    SharedPtr(SharedPtr&& rhs) {
        ptr_ = rhs.ptr_;
        counter_ = rhs.counter_;

        rhs.ptr_ = nullptr;
        rhs.counter_ = nullptr;
    }

    SharedPtr(std::string* str) {
        ptr_ = str;

        if (ptr_ != nullptr) {
            counter_ = new int(1);
        } else {
            counter_ = nullptr;
        }
    }

    SharedPtr(const WeakPtr& rhs);

    std::string operator*() const {
        return *ptr_;
    }

    std::string* Get() const {
        return ptr_;
    }

    int* GetCount() const {
        return counter_;
    }

    void Reset(std::string* ptr) {
        if (ptr_ != nullptr && counter_ != nullptr) {
            if (*counter_ > 0) {
                --(*counter_);
            }
            if (*counter_ == 0) {
                delete counter_;
                delete ptr_;
            }
        }
        counter_ = nullptr;
        ptr_ = nullptr;

        if (ptr != nullptr) {
            ptr_ = ptr;
            counter_ = new int(1);
        }
    }

    void Swap(SharedPtr ptr) {
        std::swap(ptr_, ptr.ptr_);
        std::swap(counter_, ptr.counter_);
    }

    SharedPtr& operator=(const SharedPtr& rhs) {
        if (rhs.ptr_ != ptr_) {
            Swap(rhs);
        }
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& rhs) {
        if (rhs.ptr_ != ptr_) {
            Swap(rhs);
        }
        return *this;
    }

    ~SharedPtr() {
        if (ptr_ != nullptr && counter_ != nullptr) {
            if (*counter_ > 0) {
                --(*counter_);
            }
            if (*counter_ == 0) {
                delete counter_;
                delete ptr_;
            }
            counter_ = nullptr;
            ptr_ = nullptr;
        }
    }

private:
    std::string* ptr_;
    int* counter_;
};

class WeakPtr {
public:
    WeakPtr() {
        ptr_ = nullptr;
        counter_ = nullptr;
    }

    WeakPtr(const WeakPtr& rhs) {
        ptr_ = rhs.ptr_;
        counter_ = rhs.counter_;
        if (ptr_ != nullptr) {
            ++(*counter_);
        }
    }

    WeakPtr(const SharedPtr& rhs) {
        ptr_ = rhs.Get();
        counter_ = rhs.GetCount();

        if (ptr_ != nullptr) {
            ++(*counter_);
        }
    }

    WeakPtr& operator=(const WeakPtr& rhs) {
        if (rhs.ptr_ != ptr_) {
            Swap(rhs);
        }
        return *this;
    }

    WeakPtr& operator=(WeakPtr&& rhs) {
        if (rhs.ptr_ != ptr_) {
            Swap(rhs);
        }
        return *this;
    }

    void Swap(WeakPtr ptr) {
        std::swap(ptr_, ptr.ptr_);
        std::swap(counter_, ptr.counter_);
    }

    SharedPtr Lock() {
        return SharedPtr(*this);
    }

    bool IsExpired() const {
        return ((counter_ == nullptr) || (*counter_ == 1));
    }

    std::string* Get() const {
        return ptr_;
    }

    int* GetCount() const {
        return counter_;
    }

    ~WeakPtr() {
        if (ptr_ != nullptr && counter_ != nullptr) {
            if (*counter_ > 0) {
                --(*counter_);
            }
            if (*counter_ == 0) {
                delete counter_;
                delete ptr_;
            }
        }
        counter_ = nullptr;
        ptr_ = nullptr;
    }

private:
    std::string* ptr_;
    int* counter_;
};

SharedPtr::SharedPtr(const WeakPtr& rhs) {
    if (rhs.Get() != nullptr) {
        if (*rhs.GetCount() == 1) {
            ptr_ = nullptr;
            counter_ = nullptr;
        } else {
            ptr_ = rhs.Get();
            counter_ = rhs.GetCount();
            ++(*counter_);
        }
    } else {
        ptr_ = nullptr;
        counter_ = nullptr;
    }
}
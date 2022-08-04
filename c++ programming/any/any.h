#pragma once

#include <memory>

class Any {
    struct Base {
        virtual ~Base() = default;
        virtual std::shared_ptr<Base> Clone() = 0;
    };

    template <class T>
    struct Derived : public Base {
        Derived(T value) : value_(value) {
        }
        std::shared_ptr<Base> Clone() override {
            return std::make_unique<Derived<T>>(value_);
        }
        const T& GetDerived() const {
            return value_;
        }

    private:
        T value_;
    };

public:
    Any() {
    }

    template <class T>
    Any(const T& value) : ptr_(std::make_shared<Derived<T>>(value)) {
    }

    template <class T>
    Any& operator=(const T& value) {
        ptr_ = std::make_shared<Derived<T>>(value);
        return *this;
    }

    Any(const Any& rhs) {
        ptr_ = (rhs.ptr_) ? rhs.ptr_->Clone() : nullptr;
    }
    Any& operator=(const Any& rhs) {
        Any tmp(rhs);
        Swap(tmp);
        return *this;
    }
    ~Any() {
    }

    bool Empty() const {
        return ptr_ == nullptr;
    }

    void Clear() {
        ptr_ = nullptr;
    }
    void Swap(Any& rhs) {
        std::swap(ptr_, rhs.ptr_);
    }

    template <class T>
    const T& GetValue() const {
        auto ptr = std::dynamic_pointer_cast<Derived<T>>(ptr_);
        if (!ptr) {
            throw std::bad_cast();
        }
        return ptr->GetDerived();
    }

private:
    std::shared_ptr<Base> ptr_ = nullptr;
};
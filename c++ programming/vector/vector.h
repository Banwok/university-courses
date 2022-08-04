#pragma once
#include <iostream>
#include <initializer_list>
#include <iterator>

class Vector {
private:
    size_t size_;
    size_t capacity_;
    int* arr_;

public:
    class Iterator : public std::iterator<int, std::random_access_iterator_tag> {
    private:
        int* ptr_;

    public:
        Iterator() : ptr_(nullptr) {
        }

        Iterator(int* n) : ptr_(n) {
        }

        bool operator==(const Iterator& other) const {
            return (ptr_ == other.ptr_);
        }

        bool operator!=(const Iterator& other) {
            return (ptr_ != other.ptr_);
        }

        Iterator operator+=(size_t n) {
            ptr_ = ptr_ + n;
            return ptr_;
        }

        Iterator operator+(size_t n) {
            Iterator temp = ptr_ + n;
            return temp;
        }

        size_t operator-(Iterator n) {
            Iterator i = n;
            size_t res = 0;
            while (i != *this) {
                ++res;
                ++i;
            }
            return res;
        }

        Iterator operator-=(size_t n) {
            ptr_ = ptr_ - n;
            return ptr_;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ptr_++;
            return temp;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            ptr_--;
            return temp;
        }

        Iterator& operator++() {
            ptr_++;
            return *this;
        }

        Iterator& operator--() {
            ptr_--;
            return *this;
        }

        int* operator->() const {
            return ptr_;
        }

        int& operator*() {
            return *ptr_;
        }

        ~Iterator() {
        }
    };

    Vector() {
        size_ = 0;
        capacity_ = 1;
        arr_ = new int[capacity_];
    }

    Vector(size_t s) {
        size_ = s;
        capacity_ = size_;
        arr_ = new int[capacity_];

        for (size_t i = 0; i < size_; i++) {
            arr_[i] = 0;
        }
    }

    Vector(const Vector& obj) {
        size_ = obj.size_;
        capacity_ = obj.capacity_;
        arr_ = new int[capacity_];

        for (size_t i = 0; i < obj.size_; i++) {
            arr_[i] = obj.arr_[i];
        }
    }

    Vector(const std::initializer_list<int>& list) : Vector(list.size()) {
        int count = 0;
        for (auto& element : list) {
            arr_[count] = element;
            ++count;
        }
    }

    Vector& operator=(const Vector& obj) {
        if (obj == *this) {
            return *this;
        }
        delete[] arr_;

        size_ = obj.size_;
        capacity_ = obj.capacity_;
        arr_ = new int[obj.size_];

        for (size_t i = 0; i < obj.size_; i++) {
            arr_[i] = obj.arr_[i];
        }
        return *this;
    }

    size_t Size() const {
        return size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    const int& operator[](size_t n) const {
        return arr_[n];
    }

    int& operator[](size_t n) {
        return arr_[n];
    }

    void Reserve(size_t n) {
        if (n <= capacity_) {
            return;
        }

        int* new_array = new int[n];

        for (size_t i = 0; i < capacity_; i++) {
            new_array[i] = arr_[i];
        }

        capacity_ = n;
        delete[] arr_;
        arr_ = new_array;
    }

    void PopBack() {
        if (size_ != 0) {
            size_--;
        }
    }

    void Clear() {
        size_ = 0;
    }

    void PushBack(const int& value) {
        if ((capacity_ - size_) == 0) {
            Reserve(2 * capacity_);
        }

        arr_[size_] = value;
        ++size_;
    }

    void Swap(Vector& obj) {
        size_t temp = size_;
        size_ = obj.size_;
        obj.size_ = temp;

        int* temps = arr_;
        arr_ = obj.arr_;
        obj.arr_ = temps;
    }

    Iterator Begin() {
        Iterator temp;
        temp = arr_;
        return temp;
    }

    Iterator End() {
        Iterator temp;
        temp = arr_ + size_;
        return temp;
    }

    Iterator begin() {  // NOLINT
        Iterator temp;
        temp = arr_;
        return temp;
    }

    Iterator end() const {  // NOLINT
        Iterator temp;
        temp = arr_ + size_;
        return temp;
    }

    friend bool operator==(const Vector& first, const Vector& second) {
        if (first.size_ != second.size_) {
            return false;
        } else {
            for (size_t i = 0; i < first.size_; ++i) {
                if (first[i] != second[i]) {
                    return false;
                }
            }
        }

        return true;
    }

    ~Vector() {
        delete[] arr_;
    }
};
#pragma once

#include <algorithm>
size_t count = 0;
// your type should inherit ListHook
class ListHook {
public:
    ListHook() : prev_(nullptr), next_(nullptr) {
    }

    bool IsLinked() const {
        return prev_ != nullptr && next_ != nullptr;
    }

    void Unlink() {
        if (IsLinked()) {
            ListHook* prev = prev_;
            ListHook* next = next_;
            count++;
            prev->next_ = next;
            next->prev_ = prev;
            prev_ = nullptr;
            next_ = nullptr;
        }
    }

    // Must unlink element from list
    ~ListHook() {
        if (IsLinked()) {
            Unlink();
        }
    }

    ListHook(const ListHook&) = delete;

private:
    template <class T>
    friend class List;

    ListHook* prev_;
    ListHook* next_;

    bool NullLink(ListHook* other) {
        return next_ == nullptr && prev_ == nullptr && other->next_ == nullptr &&
               other->prev_ == nullptr;
    }
    // that helper function might be useful
    void LinkAfter(ListHook* other) {
        if (!NullLink(other)) {
            ListHook* next = next_;
            other->prev_ = this;
            other->next_ = next;
            next->prev_ = other;
            next_ = other;
        } else {
            next_ = other;
            other->prev_ = this;
        }
    }
};

template <typename T>
class List {
public:
    class Iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
    public:
        Iterator(ListHook* node) {
            node_ = node;
        }

        Iterator& operator++() {
            node_ = node_->next_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            node_ = node_->next_;
            return tmp;
        }

        T& operator*() const {
            return *static_cast<T*>(node_);
        }

        T* operator->() const {
            return static_cast<T*>(node_);
        }

        bool operator==(const Iterator& rhs) const {
            if (node_ == rhs.node_) {
                return true;
            }
            return false;
        }

        bool operator!=(const Iterator& rhs) const {
            if (!this->operator==(rhs)) {
                return true;
            }
            return false;
        }

    private:
        ListHook* node_;
    };

    List() {
        fbegin_.LinkAfter(&fend_);
        size_ = 0;
        count = 0;
    }

    List(const List&) = delete;

    List(List&& other) {
        size_ = other.size_;
        fbegin_.next_ = other.fbegin_.next_;
        fend_.prev_ = other.fend_.prev_;
        other.fbegin_ = ListHook();
        other.fend_ = ListHook();
        other.fbegin_.LinkAfter(&other.fend_);
    }

    // must unlink all elements from list
    ~List() {
        while (!IsEmpty()) {
            fbegin_.next_->Unlink();
        }
        count = 0;
        size_ = 0;
    }

    List& operator=(const List&) = delete;

    List& operator=(List&& other) {
        if (this != &other) {
            size_ = other.size_;
            this->~List();
            new (this) List(std::move(other));
            return *this;
        }
        return *this;
    }

    bool IsEmpty() const {
        return fbegin_.next_ == &fend_;
    }

    // that method is allowed to be O(n)
    size_t Size() const {
        if (count > 0) {
            return size_ - count;
        }
        return size_;
    }

    // note that IntrusiveList doesn't own elements,
    // and never copies or moves T
    void PushBack(T* elem) {
        fend_.prev_->LinkAfter(elem);
        ++size_;
    }

    void PushFront(T* elem) {
        fbegin_.LinkAfter(elem);
        ++size_;
    }

    T& Front() {
        return *static_cast<T*>(fbegin_.next_);
    }

    const T& Front() const {
        return *static_cast<T*>(fbegin_.next_);
    }

    T& Back() {
        return *static_cast<T*>(fend_.prev_);
    }

    const T& Back() const {
        return *static_cast<T*>(fend_.prev_);
    }

    void PopBack() {
        fend_.prev_->Unlink();
    }

    void PopFront() {
        fbegin_.next_->Unlink();
    }

    Iterator Begin() {
        return {fbegin_.next_};
    }

    Iterator End() {
        return {&fend_};
    }

    // complexity of this function must be O(1)
    Iterator IteratorTo(T* element) {
        return {element};
    }

private:
    ListHook fbegin_ = ListHook();
    size_t size_ = 0;
    ListHook fend_ = ListHook();
};

template <typename T>
typename List<T>::Iterator begin(List<T>& list) {  // NOLINT
    return list.Begin();
}

template <typename T>
typename List<T>::Iterator end(List<T>& list) {  // NOLINT
    return list.End();
}

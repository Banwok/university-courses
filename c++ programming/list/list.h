#pragma once
#include <stdexcept>

template <typename T>
class List {
    class NodeBase {
    public:
        NodeBase* next_;
        NodeBase* prev_;

        NodeBase() : next_(nullptr), prev_(nullptr) {
        }

        NodeBase(NodeBase* next, NodeBase* prev) : next_(next), prev_(prev) {
        }

        virtual ~NodeBase() = default;
    };

    class ListNode : public NodeBase {
    public:
        ListNode() = delete;

        ListNode(T v) : NodeBase(), value_(v) {
        }

        ListNode(NodeBase* next, NodeBase* prev, const T& elem)
            : NodeBase(next, prev), value_(elem) {
        }

        ListNode(NodeBase* next, NodeBase* prev, T&& elem)
            : NodeBase(next, prev), value_(std::move(elem)) {
        }

        T value_;
    };

public:
    class Iterator {
    public:
        Iterator() = default;

        Iterator(NodeBase* p) : ptr_(p) {
        }

        Iterator& operator++() {
            ptr_ = ptr_->next_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp;
            temp.ptr_ = ptr_;
            ++*this;
            return temp;
        }

        Iterator& operator--() {
            ptr_ = ptr_->prev_;
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp;
            temp.ptr_ = ptr_;
            --*this;
            return temp;
        }

        T& operator*() const {
            return static_cast<ListNode*>(ptr_)->value_;
        }

        T* operator->() const {
            return &static_cast<ListNode*>(ptr_)->value_;
        }

        bool operator==(const Iterator& rhs) const {
            return ptr_ == rhs.ptr_;
        }

        bool operator!=(const Iterator& rhs) const {
            return ptr_ != rhs.ptr_;
        }

    private:
        NodeBase* ptr_;
    };

    List() {
        InitList();
        size_ = 0;
    }

    void InitList() {
        head_.next_ = head_.prev_ = &head_;
    }

    List(const List& list) {
        if (&list.head_ != nullptr) {
            InitList();
            auto curr = list.head_.next_;
            while (curr != &list.head_) {
                PushBack(static_cast<ListNode*>(curr)->value_);
                curr = curr->next_;
            }
        }
        size_ = list.size_;
    }

    List(List&& list) {
        head_ = list.head_;
        head_.prev_->next_ = &head_;
        head_.next_->prev_ = &head_;
        size_ = list.size_;
        list.head_.next_ = list.head_.prev_ = &list.head_;
        list.size_ = 0;
    }

    ~List() {
        auto curr = head_.next_;
        size_ = 0;
        while (curr != &head_) {
            auto del_elem = curr;
            curr = curr->next_;
            delete del_elem;
        }
        InitList();
    }

    List& operator=(const List& list) {
        if (&list.head_ != &head_) {
            InitList();
            auto curr = list.head_.next_;
            while (curr != &list.head_) {
                PushBack(static_cast<ListNode*>(curr)->value_);
                curr = curr->next_;
            }
        }
        return *this;
    }

    List& operator=(List&& list) {
        head_ = list.head_;
        head_.prev_->next_ = &head_;
        head_.next_->prev_ = &head_;
        size_ = list.size_;
        list.head_.next_ = list.head_.prev_ = &list.head_;
        list.size_ = 0;
        return *this;
    }

    bool IsEmpty() const {
        return size_ == 0;
    }

    size_t Size() const {
        return size_;
    }

    void PushBack(const T& elem) {
        if (size_ >= 0) {
            size_++;
            auto last = head_.prev_;
            head_.prev_ = new ListNode(&head_, last, elem);
            last->next_ = head_.prev_;
        }
    }

    void PushBack(T&& elem) {
        if (size_ >= 0) {
            size_++;
            auto last = head_.prev_;
            head_.prev_ = new ListNode(&head_, last, std::move(elem));
            last->next_ = head_.prev_;
        }
    }

    void PushFront(const T& elem) {
        if (size_ >= 0) {
            size_++;
            auto first = head_.next_;
            head_.next_ = new ListNode(first, &head_, elem);
            first->prev_ = head_.next_;
        }
    }

    void PushFront(T&& elem) {
        if (size_ >= 0) {
            size_++;
            auto first = head_.next_;
            head_.next_ = new ListNode(first, &head_, std::move(elem));
            first->prev_ = head_.next_;
        }
    }

    T& Front() {
        if (size_ == 0) {
            throw std::runtime_error("Not implemented");
        }
        return (static_cast<ListNode*>(head_.next_))->value_;
    }

    const T& Front() const {
        if (size_ == 0) {
            throw std::runtime_error("Not implemented");
        }
        return (static_cast<ListNode*>(head_.next_))->value_;
    }

    T& Back() {
        if (size_ == 0) {
            throw std::runtime_error("Not implemented");
        }
        return (static_cast<ListNode*>(head_.prev_))->value_;
    }

    const T& Back() const {
        if (size_ == 0) {
            throw std::runtime_error("Not implemented");
        }
        return (static_cast<ListNode*>(head_.prev_))->value_;
    }

    void PopBack() {
        if (size_ != 0) {
            auto del_elem = head_.prev_;
            head_.prev_->prev_->next_ = &head_;
            head_.prev_ = head_.prev_->prev_;
            delete del_elem;
            size_--;
        }
    }

    void PopFront() {
        if (size_ != 0) {
            auto del_elem = head_.next_;
            head_.next_->next_->prev_ = &head_;
            head_.next_ = head_.next_->next_;
            delete del_elem;
            size_--;
        }
    }

    Iterator Begin() {
        return Iterator(head_.next_);
    }

    Iterator End() {
        return Iterator(&head_);
    }

private:
    size_t size_;
    NodeBase head_;
};

template <typename T>
typename List<T>::Iterator begin(List<T>& list) {  // NOLINT
    return list.Begin();
}

template <typename T>
typename List<T>::Iterator end(List<T>& list) {  // NOLINT
    return list.End();
}

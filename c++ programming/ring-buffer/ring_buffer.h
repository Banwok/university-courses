#pragma once

class RingBuffer {
protected:
    size_t size_queue = 0;
    size_t capac;
    std::vector<int> queue;

public:
    explicit RingBuffer(size_t capacity) {
        capac = capacity;
        queue.resize(capacity);
    }

    size_t Size() const {
        return size_queue;
    }

    bool Empty() const {
        if (size_queue == 0) {
            return true;
        }
        return false;
    }

    bool TryPush(int element) {
        if (size_queue < queue.size()) {
            queue[--capac] = element;
            ++size_queue;
            return true;
        }
        return false;
    }

    bool TryPop(int *element) {
        if (size_queue == 0) {
            return false;
        }
        *element = queue.back();
        --size_queue;
        queue.pop_back();
        return true;
    }
};

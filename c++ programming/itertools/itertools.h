#pragma once
#include <utility>

template <class Iterator>
class Sequence {
public:
    Sequence(Iterator begin, Iterator end) : begin_(begin), end_(end) {
    }

    Iterator begin() const {  // NOLINT
        return begin_;
    }

    Iterator end() const {  // NOLINT
        return end_;
    }

private:
    Iterator begin_, end_;
};

class RangeIterator {
public:
    RangeIterator(int64_t from, int64_t to, int step) {
        from_ = from;
        to_ = to;
        step_ = step;
        value_ = from;
    }

    RangeIterator operator++() {
        value_ += step_;
        return *this;
    }

    int64_t& operator*() {
        return value_;
    }

    RangeIterator Begin() {
        auto res = RangeIterator(from_, to_, step_);
        return res;
    }

    RangeIterator End() {
        auto res = RangeIterator(to_, from_, step_);
        return res;
    }

    bool operator==(const RangeIterator& rhs) {
        if ((value_ >= rhs.value_) && (value_ > from_)) {
            return true;
        }
        return false;
    }

    bool operator!=(const RangeIterator& rhs) {
        if ((value_ < rhs.value_) && (value_ >= from_)) {
            return true;
        }
        return false;
    }

protected:
    int64_t value_;
    int step_;
    int64_t from_;
    int64_t to_;
};

template <class IteratorA, class IteratorB>
class ZipIterator {
public:
    ZipIterator(IteratorA begin_a, IteratorB begin_b) : begin_a_(begin_a), begin_b_(begin_b) {
    }

    ZipIterator operator++() {
        ++begin_a_;
        ++begin_b_;
        return *this;
    }

    auto operator*() {
        auto res = std::make_pair(*begin_a_, *begin_b_);
        return res;
    }

    bool operator==(const ZipIterator<IteratorA, IteratorB>& rhs) {
        if ((begin_a_ == rhs.begin_a_) || (begin_b_ == rhs.begin_b_)) {
            return true;
        }
        return false;
    }

    bool operator!=(const ZipIterator<IteratorA, IteratorB>& rhs) {
        if ((begin_a_ != rhs.begin_a_) && (begin_b_ != rhs.begin_b_)) {
            return true;
        }
        return false;
    }

protected:
    IteratorA begin_a_;
    IteratorB begin_b_;
};

template <class Iterator>
class GroupIterator {
public:
    GroupIterator(Iterator begin, Iterator end) {
        begin_ = begin;
        end_ = end;
    }

    auto operator*() {
        auto current_begin = begin_;
        auto current_val = *begin_;
        while (current_begin != end_) {
            if (*current_begin == current_val) {
                ++current_begin;
            } else {
                break;
            }
        }
        auto res = Sequence(begin_, current_begin);
        return res;
    }

    GroupIterator operator++() {
        auto current_val = *begin_;
        while (begin_ != end_) {
            if (*begin_ == current_val) {
                ++begin_;
            } else {
                break;
            }
        }
        return *this;
    }

    bool operator==(const GroupIterator<Iterator>& rhs) {
        if (begin_ == rhs.begin_) {
            return true;
        }
        return false;
    }

    bool operator!=(const GroupIterator<Iterator>& rhs) {
        if (begin_ != rhs.begin_) {
            return true;
        }
        return false;
    }

protected:
    Iterator begin_, end_;
};

auto Range(int64_t to) {
    auto range =
        Sequence<RangeIterator>(RangeIterator(0, to, 1).Begin(), RangeIterator(0, to, 1).End());
    return range;
}

auto Range(int64_t from, int64_t to) {
    auto range = Sequence<RangeIterator>(RangeIterator(from, to, 1).Begin(),
                                         RangeIterator(from, to, 1).End());
    return range;
}

auto Range(int64_t from, int64_t to, int step) {
    auto range = Sequence<RangeIterator>(RangeIterator(from, to, step).Begin(),
                                         RangeIterator(from, to, step).End());
    return range;
}

template <class VA, class VB>
auto Zip(const VA& a, const VB& b) {
    auto zip = Sequence<ZipIterator<decltype((static_cast<const VA*>(nullptr))->begin()),
                                    decltype((static_cast<const VB*>(nullptr))->begin())>>(
        ZipIterator<decltype((static_cast<const VA*>(nullptr))->begin()),
                    decltype((static_cast<const VB*>(nullptr))->begin())>(a.begin(), b.begin()),
        ZipIterator<decltype((static_cast<const VA*>(nullptr))->begin()),
                    decltype((static_cast<const VB*>(nullptr))->begin())>(a.end(), b.end()));
    return zip;
}

template <class T>
auto Group(const T& a) {
    auto group = Sequence<GroupIterator<decltype((static_cast<const T*>(nullptr))->begin())>>(
        GroupIterator(a.begin(), a.end()), GroupIterator(a.end(), a.end()));
    return group;
}
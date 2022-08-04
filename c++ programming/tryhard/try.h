#pragma once

#include <functional>
#include <stdexcept>
#include <memory>
#include <cstring>

class Basic {
private:
    std::exception_ptr exptr_;

protected:
    std::unique_ptr<std::string> what_;

    Basic() = default;

    Basic(std::exception const& ex) : what_(new std::string(ex.what())) {
    }

    Basic(std::exception_ptr exptr) {
        exptr_ = exptr;
    }

public:
    void Throw() {
        if (exptr_) {
            std::rethrow_exception(exptr_);
        } else if (what_) {
            throw std::runtime_error(*what_);
        } else {
            throw std::runtime_error("No exception");
        }
    }

    bool IsFailed() const {
        if (static_cast<bool>(what_)) {
            return true;
        } else if (static_cast<bool>(exptr_)) {
            return true;
        } else {
            return false;
        }
    }
};

template <class T>
class Try : public Basic {
public:
    std::unique_ptr<T> value_;

    Try() {
    }

    Try(T rhs) : Basic(), value_(new T(rhs)) {
    }

    bool TryExist() {
        if (value_ != nullptr) {
            return true;
        }
        return false;
    }

    Try(std::exception const& ex) : Basic(ex) {
    }

    Try(std::exception_ptr exptr) : Basic(exptr) {
    }

    T const& Value() const {
        if (value_) {
            return *value_;
        } else if (what_) {
            throw std::runtime_error(*what_);
        } else {
            throw std::runtime_error("Object is empty");
        }
    }
};

template <>
class Try<void> : public Basic {
public:
    bool exist = false;

    Try() : Basic(std::exception()) {
        exist = true;
    }

    Try(const Try&) : Basic() {
        exist = true;
    }

    bool TryExist() {
        return exist;
    }

    Try(std::exception const& ex) : Basic(ex) {
        exist = true;
    }

    Try(std::exception_ptr exptr) : Basic(exptr) {
        exist = true;
    }
};

template <class ReturnType>
Try<ReturnType> TryRunHelper(std::function<Try<ReturnType>()> f) {
    try {
        return f();
    } catch (int en) {
        return Try<ReturnType>(std::runtime_error(std::strerror(en)));
    } catch (const char* s) {
        return Try<ReturnType>(std::runtime_error(s));
    } catch (const std::exception& e) {
        return Try<ReturnType>(std::current_exception());
    } catch (...) {
        return Try<ReturnType>(std::runtime_error("Unknown exception"));
    }

    return Try<ReturnType>(std::exception());
}

template <class Function, class... Args>
auto TryRun(Function func, Args... args)
    -> Try<typename std::enable_if<!std::is_same<void, decltype(func(args...))>::value,
                                   decltype(func(args...))>::type> {
    using ReturnType = decltype(func(args...));
    if (func != nullptr) {
        auto fu = [&func, &args...] {
            ReturnType ret = func(args...);
            return Try<ReturnType>(ret);
        };
        auto res = TryRunHelper<ReturnType>(fu);
        return res;
    }
    throw std::runtime_error("Object is empty");
}

template <class Function, class... Args>
auto TryRun(Function func, Args... args)
    -> Try<typename std::enable_if<std::is_same<void, decltype(func(args...))>::value>::type> {
    using ReturnType = decltype(func(args...));

    auto res = TryRunHelper<ReturnType>([&func, &args...] {
        func(args...);
        return Try<ReturnType>();
    });
    if (res.TryExist()) {
        return res;
    }
    throw std::runtime_error("Object is empty");
}
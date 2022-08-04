#pragma once

#include <string>
#include <cstring>

class StringView {
protected:
    size_t sbeg = 0;
    size_t send = std::string::npos;
    const char* pt;

public:
    StringView(const std::string& orig) {
        send = orig.length();
        pt = orig.c_str();
    }
    StringView(const std::string& orig, size_t beg_s) {
        sbeg = beg_s;
        pt = orig.c_str();
        pt = pt + sbeg;
        send = std::strlen(pt);
    }
    StringView(const std::string& orig, size_t beg_s, size_t end_s) {
        sbeg = beg_s;
        send = end_s;
        pt = orig.c_str();
        pt = pt + sbeg;
    }
    StringView(const char* orig) {
        send = std::strlen(orig);
        pt = orig;
    }
    StringView(const char* orig, size_t end_s) {
        send = end_s;
        pt = orig;
    }
    size_t Size() const {
        return send;
    }
    const char& operator[](size_t i) const;
};

const char& StringView::operator[](size_t i) const {
    return *(pt + i);
}

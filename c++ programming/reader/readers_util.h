#pragma once

#include <vector>
#include <memory>
#include <ios>
#include <sstream>

#include <reader.h>

class LimitReader : public Reader {
protected:
    std::unique_ptr<Reader> reader_;
    size_t limit_;

public:
    LimitReader(std::unique_ptr<Reader> reader, size_t limit)
        : reader_(std::move(reader)), limit_(limit) {
    }

    virtual size_t Read(char* buf, size_t len) override {
        len = std::min(limit_, len);
        size_t read_len = reader_->Read(buf, len);
        limit_ -= read_len;
        return read_len;
    }
};

class TeeReader : public Reader {
protected:
    std::vector<std::unique_ptr<Reader>> readers_;
    size_t curr_ = 0;

public:
    TeeReader(std::vector<std::unique_ptr<Reader>> readers) {
        for (auto& current_reader : readers) {
            readers_.push_back(std::move(current_reader));
        }
    }

    virtual size_t Read(char* buf, size_t len) override {
        size_t red_len = 0;
        size_t shift;
        while (curr_ < readers_.size() && len != 0) {
            size_t curr_len = readers_[curr_]->Read(buf, len);
            red_len += curr_len;
            buf += curr_len;
            len -= curr_len;
            if (len != 0) {
                ++curr_;
            } else {
                ++shift;
            }
        }
        return red_len;
    }
};

class HexDecodingReader : public Reader {
protected:
    std::unique_ptr<Reader> reader_;

public:
    HexDecodingReader(std::unique_ptr<Reader> reader) : reader_(std::move(reader)) {
    }

    virtual size_t Read(char* buf, size_t len) override {
        char hex[4];
        size_t ind;
        size_t dec;
        size_t shift;
        for (ind = 0; ind < len; ++ind) {
            if (!reader_->Read(hex, 2)) {
                if (hex[3] == '\0') {
                    return ind;
                }
                return 0;
            }
            ++shift;
            std::istringstream(hex) >> std::hex >> dec;
            buf[ind] = static_cast<char>(dec);
        }
        return ind;
    }
};

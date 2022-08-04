#pragma once

#include <variant>
#include <optional>
#include <istream>
#include <error.h>

struct SymbolToken {
    std::string name;

    bool operator==(const SymbolToken& other) const {
        if (name == other.name) {
            return true;
        }
        return false;
    }
};

struct QuoteToken {
    bool operator==(const QuoteToken&) const {
        return true;
    }
};

struct DotToken {
    bool operator==(const DotToken&) const {
        return true;
    }
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int value;

    bool operator==(const ConstantToken& other) const {
        if (value == other.value) {
            return true;
        }

        return false;
    }
};

typedef std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken> Token;

// Интерфейс позволяющий читать токены по одному из потока.
class Tokenizer {
public:
    explicit Tokenizer(std::istream* in) : is_end_(false), in_(in) {
        Next();
    }

    bool IsEnd() {
        return is_end_;
    }

    void Next() {
        if (in_->peek() == EOF) {
            is_end_ = true;
            return;
        }
        std::string curr;
        SpacesRead();
        if (std::isdigit(in_->peek()) || in_->peek() == '-' || in_->peek() == '+') {
            curr += in_->get();
            while (std::isdigit(in_->peek())) {
                curr += in_->get();
            }
            if (std::isdigit(curr[0])) {
                token_ = Token(ConstantToken{std::stoi(curr)});
            } else if (curr.size() > 1 && curr[0] == '-') {
                token_ = Token(ConstantToken{std::stoi(curr)});
            } else if (curr.size() > 1 && curr[0] == '+') {
                token_ = Token(ConstantToken{std::stoi(curr)});
            } else {
                token_ = Token(SymbolToken{curr});
            }
            return;
        }
        if (!ChekEof()) {
            is_end_ = true;
            return;
        }
        curr = in_->get();
        switch (curr[0]) {
            case '(':
                token_ = Token(BracketToken::OPEN);
                op_++;
                break;
            case ')':
                token_ = Token(BracketToken::CLOSE);
                clos_++;
                break;
            case '.':
                token_ = Token(DotToken());
                break;
            case '\'':
                token_ = Token(QuoteToken{});
                break;
            default:
                while (CheckBr() && ChekEof()) {
                    curr += in_->get();
                }
                token_ = Token(SymbolToken{curr});
                break;
        }
    }

    void SpacesRead() {
        while (in_->peek() == ' ' || in_->peek() == '\n') {
            in_->get();
        }
    }

    bool CheckBr() {
        if (in_->peek() != ' ' && in_->peek() != ')' && in_->peek() != '(') {
            return true;
        }
        return false;
    }

    bool ChekEof() {
        if (in_->peek() != EOF && in_->peek() != '\n') {
            return true;
        }
        return false;
    }

    Token GetToken() {
        return token_;
    }

    void ChekError() {
        if (clos_ > op_) {
            throw SyntaxError("Unexpectet close bracket");
        }
    }

private:
    Token token_;
    std::istream* in_;
    bool is_end_;
    int op_ = 0;
    int clos_ = 0;
};
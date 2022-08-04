#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <stack>
#include <map>

class Tokenizer {
public:
    Tokenizer(std::istream* in) : in_(in) {
    }

    enum TokenType { kUnknown, kNumber, kSymbol, kEnd };

    void Consume() {
        while (!in_->eof() && in_->peek() == ' ') {
            in_->get();
        }
        if (in_->eof()) {
            type_ = TokenType::kEnd;
            return;
        }
        if (isdigit(in_->peek())) {
            std::string number;
            while (isdigit(in_->peek())) {
                number += in_->get();
            }
            number_ = std::stoll(number);
            type_ = TokenType::kNumber;
            return;
        }
        char c = in_->peek();
        in_->get();
        symbol_ = c;
        type_ = TokenType::kSymbol;
    }

    TokenType GetType() {
        return type_;
    }

    int64_t GetNumber() {
        return number_;
    }

    char GetSymbol() {
        return symbol_;
    }

private:
    std::istream* in_;

    TokenType type_ = TokenType::kUnknown;
    int64_t number_;
    char symbol_;
};

class Expression {
public:
    virtual ~Expression() = default;
    virtual int64_t Evaluate() = 0;
};

class Number : public Expression {
private:
    int64_t number_;

public:
    Number(const int64_t& number) : number_(number) {
    }

    int64_t Evaluate() override {
        return number_;
    }
};

class UnaryMinus : public Expression {
protected:
    std::unique_ptr<Expression> number_;

public:
    UnaryMinus(std::unique_ptr<Expression> number) : number_(std::move(number)) {
    }

    int64_t Evaluate() override {
        return -number_->Evaluate();
    }
};

class CalcPlus : public Expression {
protected:
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;

public:
    CalcPlus(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : left_(std::move(left)), right_(std::move(right)) {
    }

    int64_t Evaluate() override {
        return left_->Evaluate() + right_->Evaluate();
    }
};

class CalcMinus : public Expression {
protected:
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;

public:
    CalcMinus(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : left_(std::move(left)), right_(std::move(right)) {
    }

    int64_t Evaluate() override {
        return left_->Evaluate() - right_->Evaluate();
    }
};

class CalcMul : public Expression {
protected:
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;

public:
    CalcMul(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : left_(std::move(left)), right_(std::move(right)) {
    }

    int64_t Evaluate() override {
        return left_->Evaluate() * right_->Evaluate();
    }
};

class CalcDiv : public Expression {
protected:
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;

public:
    CalcDiv(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : left_(std::move(left)), right_(std::move(right)) {
    }

    int64_t Evaluate() override {
        return left_->Evaluate() / right_->Evaluate();
    }
};

std::unique_ptr<Expression> CheckUnary(Tokenizer* tok, std::map<char, int>& priority,
                                       int& plus_minus, int& mul_div);

std::unique_ptr<Expression> CalcExpr(Tokenizer* tok, std::map<char, int>& priority,
                                     int last_precedence, int& plus_minus, int& mul_div) {
    auto expr = CheckUnary(tok, priority, plus_minus, mul_div);

    while (tok->GetSymbol() != ')' && priority[tok->GetSymbol()] >= last_precedence &&
           tok->GetType() != Tokenizer::kEnd) {
        char operation = tok->GetSymbol();
        switch (operation) {
            case '+':
                mul_div = 0;
                if (plus_minus != 0) {
                    --plus_minus;
                    return expr;
                } else {
                    auto t1 = CalcExpr(tok, priority, priority[operation], ++plus_minus, mul_div);
                    expr = std::unique_ptr<CalcPlus>(new CalcPlus(std::move(expr), std::move(t1)));
                }
                break;
            case '-':
                mul_div = 0;
                if (plus_minus != 0) {
                    --plus_minus;
                    return expr;
                } else {
                    auto t1 = CalcExpr(tok, priority, priority[operation], ++plus_minus, mul_div);
                    expr =
                        std::unique_ptr<CalcMinus>(new CalcMinus(std::move(expr), std::move(t1)));
                }
                break;
            case '*':
                if (mul_div != 0) {
                    mul_div = 0;
                    return expr;
                } else {
                    auto t1 = CalcExpr(tok, priority, priority[operation], plus_minus, ++mul_div);
                    expr = std::unique_ptr<CalcMul>(new CalcMul(std::move(expr), std::move(t1)));
                }
                break;
            case '/':
                if (mul_div != 0) {
                    mul_div = 0;
                    return expr;
                } else {
                    auto t1 = CalcExpr(tok, priority, priority[operation], plus_minus, ++mul_div);
                    expr = std::unique_ptr<CalcDiv>(new CalcDiv(std::move(expr), std::move(t1)));
                }
                break;
            default:
                break;
        }
    }
    return expr;
}

std::unique_ptr<Expression> CheckUnary(Tokenizer* tok, std::map<char, int>& priority,
                                       int& plus_minus, int& mul_div) {
    auto previous_type = tok->GetType();
    auto previous_symbol = tok->GetSymbol();
    tok->Consume();
    if ((tok->GetType() == Tokenizer::kSymbol) && tok->GetSymbol() == '-' &&
        (previous_type == Tokenizer::kUnknown || previous_type == Tokenizer::kSymbol)) {
        char operation = tok->GetSymbol();
        auto t = CalcExpr(tok, priority, priority[operation], ++plus_minus, mul_div);
        if (previous_symbol != '+' && tok->GetSymbol() != '+' && tok->GetSymbol() != '-' &&
            tok->GetSymbol() != ')') {
            tok->Consume();
        }
        return std::unique_ptr<UnaryMinus>(new UnaryMinus(std::move(t)));
    } else if (tok->GetSymbol() == '(' && tok->GetType() == Tokenizer::kSymbol) {
        int cur_plus = 0;
        int cur_mul = 0;
        auto t = CalcExpr(tok, priority, 0, cur_plus, cur_mul);
        tok->Consume();
        return t;
    }
    auto t = tok->GetNumber();
    tok->Consume();
    return std::unique_ptr<Number>(new Number(t));
}

std::unique_ptr<Expression> ParseExpression(Tokenizer* tok) {
    std::map<char, int> priority = {{'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}, {'(', 3}, {')', 3}};

    int plus_minus = 0;
    int mul_div = 0;

    return CalcExpr(tok, priority, 0, plus_minus, mul_div);
}

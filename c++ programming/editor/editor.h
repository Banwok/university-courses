#pragma once
#include <vector>
#include <string>
#include <memory>

class ICommand {
public:
    virtual bool Do() = 0;
    virtual void Undo() = 0;
    virtual ~ICommand() = default;
};

class Editor {
    class TypeCommand : public ICommand {
    public:
        TypeCommand(Editor* ed, char ch) {
            ed_ = ed;
            ch_ = ch;
        }
        void Undo() override {
            ed_->Back();
        }
        bool Do() override {
            return ed_->Insert(ch_);
        }
        Editor* ed_;
        char ch_;
    };

    class BackspaceCommand : public ICommand {
    public:
        BackspaceCommand(Editor* ed) {
            ed_ = ed;
        }
        void Undo() override {
            ed_->Insert(ch_);
        }
        bool Do() override {
            ch_ = ed_->Back();
            return ch_;
        }
        Editor* ed_;
        char ch_;
    };

    class LShiftCommand : public ICommand {
    public:
        LShiftCommand(Editor* ed) {
            ed_ = ed;
        }
        void Undo() override {
            ed_->RShift();
        }
        bool Do() override {
            return ed_->LShift();
        }
        Editor* ed_;
    };

    class RShiftCommand : public ICommand {
    public:
        RShiftCommand(Editor* ed) {
            ed_ = ed;
        }
        void Undo() override {
            ed_->LShift();
        }
        bool Do() override {
            return ed_->RShift();
        }
        Editor* ed_;
    };

public:
    const std::string& GetText() const {
        return buffer_;
    }

    void Type(char c) {
        AddCommand(std::make_unique<TypeCommand>(this, c));
    }

    void Backspace() {
        AddCommand(std::make_unique<BackspaceCommand>(this));
    }

    void ShiftLeft() {
        AddCommand(std::make_unique<LShiftCommand>(this));
    }

    void ShiftRight() {
        AddCommand(std::make_unique<RShiftCommand>(this));
    }

    void Undo() {
        if (!do_.empty()) {
            do_.back()->Undo();
            undo_.push_back(std::move(do_.back()));
            do_.pop_back();
        }
    }

    void Redo() {
        if (!undo_.empty()) {
            undo_.back()->Do();
            do_.push_back(std::move(undo_.back()));
            undo_.pop_back();
        }
    }

private:
    bool LShift() {
        if (pos_ == 0) {
            return false;
        } else {
            --pos_;
            return true;
        }
        return false;
    }

    bool RShift() {
        if (pos_ == buffer_.size()) {
            return false;
        } else {
            ++pos_;
            return true;
        }
        return false;
    }

    char Back() {
        if (pos_ == 0) {
            return false;
        } else {
            --pos_;
            char temp = buffer_[pos_];
            buffer_.erase(pos_, 1);
            return temp;
        }
        return false;
    }

    bool Insert(char c) {
        buffer_.insert(pos_, 1, c);
        ++pos_;
        return true;
    }

    void AddCommand(std::unique_ptr<ICommand>&& command) {
        if (command->Do()) {
            do_.push_back(std::move(command));
            undo_.clear();
        } else {
            return;
        }
    }
    std::vector<std::unique_ptr<ICommand>> do_;
    std::vector<std::unique_ptr<ICommand>> undo_;
    std::string buffer_;
    size_t pos_ = 0;
};

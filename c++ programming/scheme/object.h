#pragma once

#include <memory>
#include <string>
#include <istream>
#include <vector>
#include <unordered_map>
#include <error.h>

class Scope;

class Object : public std::enable_shared_from_this<Object> {
public:
    virtual bool IsFalse() {
        return false;
    }

    virtual void PrintTo(std::ostream* out) = 0;

    virtual std::shared_ptr<Object> Eval(const std::shared_ptr<Scope>& scope) = 0;

    virtual ~Object() = default;
};

class Scope {
public:
    Scope() = default;

    Scope(std::shared_ptr<Scope>& parent) : parent(parent) {
    }

    std::shared_ptr<Object> GetSecond(const std::string& name) {
        auto it = scheme_map.find(name);
        if (it == scheme_map.end()) {
            if (parent) {
                return parent->GetSecond(name);
            }
            throw NameError("NameError");
        }
        return it->second;
    }

    std::unordered_map<std::string, std::shared_ptr<Object>> scheme_map;
    std::shared_ptr<Scope> parent;
};

class Number : public Object {
public:
    Number(int64_t value) : value_(value) {
    }

    int64_t GetValue() const {
        return value_;
    }
    int64_t& SetValue() {
        return value_;
    }

    virtual void PrintTo(std::ostream* out) override;

    virtual std::shared_ptr<Object> Eval(const std::shared_ptr<Scope>&) override;

protected:
    int64_t value_;
};

class Symbol : public Object {
public:
    Symbol() = default;

    Symbol(const char name) {
        name_ = name;
    }

    Symbol(std::string name) : name_(name) {
    }

    const std::string& GetName() const {
        return name_;
    }

    virtual void PrintTo(std::ostream* out) override;

    virtual std::shared_ptr<Object> Eval(const std::shared_ptr<Scope>& scope) override;

protected:
    std::string name_;
};

class Cell : public Object {
public:
    Cell() = default;
    Cell(std::shared_ptr<Object> first, std::shared_ptr<Object> second)
        : first_(first), second_(second) {
    }

    std::shared_ptr<Object> GetFirst() const {
        return first_;
    }

    std::shared_ptr<Object> GetSecond() const {
        return second_;
    }

    void SetFirst(std::shared_ptr<Object> ptr) {
        first_ = ptr;
    }
    void SetSecond(std::shared_ptr<Object> ptr) {
        second_ = ptr;
    }

    virtual void PrintTo(std::ostream* out) override;

    virtual std::shared_ptr<Object> Eval(const std::shared_ptr<Scope>& scope) override;

protected:
    std::shared_ptr<Object> first_;
    std::shared_ptr<Object> second_;
};

class Boolean : public Symbol {
public:
    Boolean(bool val) {
        if (val) {
            name_ = "#t";
        } else {
            name_ = "#f";
        }
    }

    bool IsFalse() override {
        if (name_ == "#f") {
            return true;
        }
        return false;
    }

    virtual std::shared_ptr<Object> Eval(const std::shared_ptr<Scope>&) override;
};

class Function : public Object {
public:
    virtual std::shared_ptr<Object> Eval(const std::shared_ptr<Scope>& scope) override;

    void PrintTo(std::ostream* out) override;

    virtual bool CheckFunction() {
        return false;
    }

    virtual std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                          const std::vector<std::shared_ptr<Object>>& args) = 0;
};

template <class T>
bool Is(const std::shared_ptr<Object>& obj);

class LambdaFunction : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;

    void SetScope(const std::shared_ptr<Scope>& scope) {
        current_scope_ = scope;
    }

    void SetArgs(std::vector<std::shared_ptr<Object>> args) {
        for (const auto& obj : args) {
            if (!Is<Symbol>(obj)) {
                throw SyntaxError("Wrong argument name");
            }
            args_.push_back(obj);
        }
    }

    void AddToLambda(std::shared_ptr<Object> form) {
        lambda_.push_back(form);
    }

private:
    std::shared_ptr<Scope> current_scope_;
    std::vector<std::shared_ptr<Object>> args_;
    std::vector<std::shared_ptr<Object>> lambda_;
};

enum class Math { Plus, Minus, Multiply, Divide };
class SimpleOperations : public Function {
public:
    explicit SimpleOperations(Math mat) : mat_(mat) {
    }

    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;

private:
    const Math mat_;
};

enum class Cheks { Not, Null, Pair, Number, Boolean, Symbol, List };
class SimpleCheker : public Function {
public:
    explicit SimpleCheker(Cheks ch) : ch_(ch) {
    }

    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;

private:
    const Cheks ch_;
};

enum class ComparOperators { Equality, More, Less, MoreOrEqual, LessOrEqual };
class Comparison : public Function {
public:
    explicit Comparison(ComparOperators comp) : comp_(comp) {
    }

    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;

private:
    ComparOperators comp_;
};

enum class MinAndMax { Min, Max };
class MinMax : public Function {
public:
    explicit MinMax(MinAndMax mm) : mm_(mm) {
    }

    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;

private:
    MinAndMax mm_;
};

enum class CarCdr { Car, Cdr, SetCar, SetCdr };
class GeterSeter : public Function {
public:
    GeterSeter(CarCdr get_set) : get_set_(get_set) {
    }

    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;

private:
    CarCdr get_set_;
};

enum class Lists { List, ListRef, ListTail };
class SomeList : public Function {
public:
    explicit SomeList(Lists lst) : lst_(lst) {
    }

    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;

private:
    Lists lst_;
};

enum class AbsCons { Abs, Cons };
class NumberMod : public Function {
public:
    explicit NumberMod(AbsCons mod) : mod_(mod) {
    }

    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;

private:
    AbsCons mod_;
};

enum class Logic { Quote, If, And, Or };
class LogicOperators : public Function {
public:
    LogicOperators(Logic logic) : logic_(logic) {
    }
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
    bool CheckFunction() override {
        return true;
    }

private:
    Logic logic_;
};

enum class Variables { Define, Set };
class ModifVariables : public Function {
public:
    ModifVariables(Variables var) : var_(var) {
    }

    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
    bool CheckFunction() override {
        return true;
    }

private:
    Variables var_;
};

class Lambda : public Function {
public:
    std::shared_ptr<Object> Apply(const std::shared_ptr<Scope>& scope,
                                  const std::vector<std::shared_ptr<Object>>& args) override;
    bool CheckFunction() override {
        return true;
    }
};

template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<T>(obj);
}

template <class T>
bool Is(const std::shared_ptr<Object>& obj) {
    auto ptr = obj.get();
    if (dynamic_cast<T*>(ptr)) {
        return true;
    }
    return false;
}

inline void PrintGlobal(const std::shared_ptr<Object>& obj, std::ostream* out) {
    if (!obj) {
        *out << "()";
        return;
    }
    obj->PrintTo(out);
}

std::vector<std::shared_ptr<Object>> MakeVectorArg(const std::shared_ptr<Object>& head);
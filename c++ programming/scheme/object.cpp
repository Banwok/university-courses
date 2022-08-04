#include <object.h>

// EVAL
std::shared_ptr<Object> Cell::Eval(const std::shared_ptr<Scope>& scope) {
    if (first_ == nullptr) {
        throw RuntimeError("Eval Cell problem");
    }
    auto ptr = first_->Eval(scope);
    auto fun = std::dynamic_pointer_cast<Function>(ptr);
    if (!fun) {
        throw RuntimeError("Eval Cell problem");
    }
    std::vector<std::shared_ptr<Object>> args = MakeVectorArg(second_);
    if (fun && !fun->CheckFunction()) {
        for (auto& arg : args) {
            arg = arg->Eval(scope);
        }
    }
    return fun->Apply(scope, args);
}

std::shared_ptr<Object> Number::Eval(const std::shared_ptr<Scope>&) {
    return shared_from_this();
}

std::shared_ptr<Object> Symbol::Eval(const std::shared_ptr<Scope>& scope) {
    return scope->GetSecond(name_);
}

std::shared_ptr<Object> Boolean::Eval(const std::shared_ptr<Scope>&) {
    return shared_from_this();
}

std::shared_ptr<Object> Function::Eval(const std::shared_ptr<Scope>&) {
    throw RuntimeError("Can't eval function");
}

// PRINT
void Cell::PrintTo(std::ostream* out) {
    *out << '(';
    PrintGlobal(first_, out);
    auto next = As<Cell>(second_);
    if (!next && second_) {
        *out << " . ";
        PrintGlobal(second_, out);
    } else {
        while (next) {
            *out << " ";
            PrintGlobal(next->first_, out);
            auto next_obj = As<Cell>(next->second_);
            if (!next_obj && next->second_) {
                *out << " . ";
                PrintGlobal(next->second_, out);
            }
            next = next_obj;
        }
    }
    *out << ')';
}

void Number::PrintTo(std::ostream* out) {
    *out << value_;
}

void Symbol::PrintTo(std::ostream* out) {
    *out << name_;
}

void Function::PrintTo(std::ostream*) {
    throw RuntimeError("Can't print function");
}

// APPLY
std::shared_ptr<Object> SimpleOperations::Apply(const std::shared_ptr<Scope>&,
                                                const std::vector<std::shared_ptr<Object>>& args) {
    int64_t value;
    if (mat_ == Math::Plus || mat_ == Math::Multiply) {
        value = (mat_ == Math::Plus) ? 0 : 1;
        for (const auto& arg : args) {
            auto number = std::dynamic_pointer_cast<Number>(arg);
            if (!number) {
                throw RuntimeError("Arguments must be numbers");
            }
            if (mat_ == Math::Multiply) {
                value *= number->GetValue();
            } else {
                value += number->GetValue();
            }
        }
    }

    if (mat_ == Math::Minus || mat_ == Math::Divide) {
        if (args.empty()) {
            throw RuntimeError("Wrongs argument");
        }
        value = std::dynamic_pointer_cast<Number>(args[0])->GetValue();
        if (!value) {
            throw RuntimeError("Arguments must be numbers");
        }
        for (size_t i = 1; i < args.size(); ++i) {
            auto number = std::dynamic_pointer_cast<Number>(args[i]);
            if (!number) {
                throw RuntimeError("Arguments must be numbers");
            }
            if (mat_ == Math::Divide) {
                value /= number->GetValue();
            } else {
                value -= number->GetValue();
            }
        }
    }
    return std::make_shared<Number>(value);
}

std::shared_ptr<Object> SimpleCheker::Apply(const std::shared_ptr<Scope>&,
                                            const std::vector<std::shared_ptr<Object>>& args) {
    if (args.size() != 1) {
        throw RuntimeError("RuntimeError");
    }
    if (ch_ == Cheks::Not) {
        if (args[0] == nullptr) {
            return std::make_shared<Boolean>(false);
        }
        if (args[0]->IsFalse()) {
            return std::make_shared<Boolean>(true);
        }
        return std::make_shared<Boolean>(false);
    } else if (ch_ == Cheks::Null) {
        if (args[0] != nullptr) {
            return std::make_shared<Boolean>(false);
        }
    } else if (ch_ == Cheks::Pair) {
        return std::make_shared<Boolean>(Is<Cell>(args[0]));
    } else if (ch_ == Cheks::Number) {
        return std::make_shared<Boolean>(Is<Number>(args[0]));
    } else if (ch_ == Cheks::Boolean) {
        return std::make_shared<Boolean>(
            Is<Symbol>(args[0]) &&
            (As<Symbol>(args[0])->GetName() == "#f" || As<Symbol>(args[0])->GetName() == "#t"));
    } else if (ch_ == Cheks::Symbol) {
        return std::make_shared<Boolean>(Is<Symbol>(args[0]));
    } else if (ch_ == Cheks::List) {
        auto checker = args[0];
        while (checker) {
            if (!Is<Cell>(checker)) {
                return std::make_shared<Boolean>(false);
            }
            checker = As<Cell>(checker)->GetSecond();
        }
    }
    return std::make_shared<Boolean>(true);
}

std::shared_ptr<Object> Comparison::Apply(const std::shared_ptr<Scope>&,
                                          const std::vector<std::shared_ptr<Object>>& args) {
    if (args.empty()) {
        return std::make_shared<Boolean>(true);
    }
    int64_t value = As<Number>(args[0])->GetValue();
    for (size_t i = 1; i < args.size(); ++i) {
        if (!Is<Number>(args[i]) || !Is<Number>(args[i - 1])) {
            throw RuntimeError("RuntimeError");
        }
        if (comp_ == ComparOperators::Equality) {
            if (value != As<Number>(args[i])->GetValue()) {
                return std::make_shared<Boolean>(false);
            }
        } else if (comp_ == ComparOperators::More) {
            if (As<Number>(args[i - 1])->GetValue() <= As<Number>(args[i])->GetValue()) {
                return std::make_shared<Boolean>(false);
            }
        } else if (comp_ == ComparOperators::Less) {
            if (As<Number>(args[i - 1])->GetValue() >= As<Number>(args[i])->GetValue()) {
                return std::make_shared<Boolean>(false);
            }
        } else if (comp_ == ComparOperators::MoreOrEqual) {
            if (As<Number>(args[i - 1])->GetValue() < As<Number>(args[i])->GetValue()) {
                return std::make_shared<Boolean>(false);
            }
        } else if (comp_ == ComparOperators::LessOrEqual) {
            if (As<Number>(args[i - 1])->GetValue() > As<Number>(args[i])->GetValue()) {
                return std::make_shared<Boolean>(false);
            }
        }
    }
    return std::make_shared<Boolean>(true);
}

std::shared_ptr<Object> MinMax::Apply(const std::shared_ptr<Scope>&,
                                      const std::vector<std::shared_ptr<Object>>& args) {
    if (args.empty()) {
        throw RuntimeError("RuntimeError");
    }
    if (!Is<Number>(args[0])) {
        throw RuntimeError("RuntimeError");
    }
    int64_t min = As<Number>(args[0])->GetValue();
    int64_t max = As<Number>(args[0])->GetValue();
    for (const auto& obj : args) {
        if (!Is<Number>(obj)) {
            throw RuntimeError("RuntimeError");
        }
        int64_t current = As<Number>(obj)->GetValue();
        min = (min > current) ? current : min;
        max = (max < current) ? current : max;
    }
    if (mm_ == MinAndMax::Min) {
        return std::make_shared<Number>(min);
    }
    return std::make_shared<Number>(max);
}

std::shared_ptr<Object> GeterSeter::Apply(const std::shared_ptr<Scope>&,
                                          const std::vector<std::shared_ptr<Object>>& args) {

    if (get_set_ == CarCdr::Car || get_set_ == CarCdr::Cdr) {
        if (args.size() != 1) {
            throw RuntimeError("RuntimeError");
        }
        if (get_set_ == CarCdr::Cdr) {
            return As<Cell>(args[0])->GetSecond();
        }
        return As<Cell>(args[0])->GetFirst();
    }
    if (args.size() != 2) {
        throw RuntimeError("RuntimeError");
    }
    if (get_set_ == CarCdr::SetCdr) {
        As<Cell>(args[0])->SetSecond(args[1]);
        return args[0];
    }
    As<Cell>(args[0])->SetFirst(args[1]);
    return args[0];
}

std::shared_ptr<Object> SomeList::Apply(const std::shared_ptr<Scope>&,
                                        const std::vector<std::shared_ptr<Object>>& args) {
    if (lst_ == Lists::List) {
        if (args.empty()) {
            return nullptr;
        }
        auto new_cell = std::make_shared<Cell>();
        auto res = new_cell;
        for (size_t ind = 0; ind < args.size(); ++ind) {
            new_cell->SetFirst(args[ind]);
            if (ind != args.size() - 1) {
                new_cell->SetSecond(std::make_shared<Cell>());
            }
            new_cell = As<Cell>(new_cell->GetSecond());
        }
        return res;
    }

    if (!Is<Cell>(args[0]) && !Is<Number>(args[1]) && args.size() != 2) {
        throw RuntimeError("Arguments must be list");
    }
    auto scope = args[0];
    auto value = As<Number>(args[1])->GetValue();
    while (value != 0) {
        if (!Is<Cell>(scope)) {
            throw RuntimeError("List is too short");
        }
        scope = As<Cell>(scope)->GetSecond();
        --value;
    }

    if (lst_ == Lists::ListTail) {
        if (!Is<Cell>(scope) && scope) {
            throw RuntimeError("RuntimeError");
        } else if (!scope) {
            return nullptr;
        }
        return As<Cell>(scope);
    }

    if (!Is<Cell>(scope)) {
        throw RuntimeError("RuntimeError");
    }
    return As<Cell>(scope)->GetFirst();
}

std::shared_ptr<Object> NumberMod::Apply(const std::shared_ptr<Scope>&,
                                         const std::vector<std::shared_ptr<Object>>& args) {
    if (mod_ == AbsCons::Abs) {
        if (args.size() != 1) {
            throw RuntimeError("RuntimeError");
        }
        if (!Is<Number>(args[0])) {
            throw RuntimeError("RuntimeError");
        }
        return std::make_shared<Number>(std::abs(As<Number>(args[0])->GetValue()));
    } else if (mod_ == AbsCons::Cons) {
        if (args.size() != 2) {
            throw RuntimeError("RuntimeError");
        }
        auto new_cell = std::make_shared<Cell>();
        new_cell->SetFirst(args[0]);
        new_cell->SetSecond(args[1]);
        return new_cell;
    }
}

std::shared_ptr<Object> LogicOperators::Apply(const std::shared_ptr<Scope>& scope,
                                              const std::vector<std::shared_ptr<Object>>& args) {
    if (logic_ == Logic::Quote) {
        if (args.size() != 1) {
            throw SyntaxError("Syntax error");
        }
        return args[0];
    } else if (logic_ == Logic::If) {
        if (args.size() < 2 || args.size() > 3) {
            throw SyntaxError("Syntax error");
        }
        auto condition = args[0];
        auto if_true = args[1];
        auto result = condition->Eval(scope);
        if (result && !result->IsFalse()) {
            return if_true->Eval(scope);
        }
        if (args.size() == 2) {
            return nullptr;
        }
        auto if_false = args[2];
        return if_false->Eval(scope);
    } else if (logic_ == Logic::Or) {
        for (const auto& obj : args) {
            auto res = obj->Eval(scope);
            if (!res->IsFalse()) {
                return res;
            }
        }
        return std::make_shared<Boolean>(false);
    } else if (logic_ == Logic::And) {
        size_t i = 0;
        for (const auto& obj : args) {
            auto res = obj->Eval(scope);
            if (res->IsFalse()) {
                return std::make_shared<Boolean>(false);
            }
            if (i == args.size() - 1) {
                return res;
            }
            ++i;
        }
        return std::make_shared<Boolean>(true);
    }
}

std::shared_ptr<Object> ModifVariables::Apply(const std::shared_ptr<Scope>& scope,
                                              const std::vector<std::shared_ptr<Object>>& args) {
    if (args.size() != 2) {
        throw SyntaxError("Wrong number");
    }
    if (var_ == Variables::Define) {
        if (Is<Symbol>(args[0])) {
            scope->scheme_map[As<Symbol>(args[0])->GetName()] = args[1]->Eval(scope);
        } else if (Is<Cell>(args[0])) {
            auto new_lambda_function = std::make_shared<LambdaFunction>();
            new_lambda_function->SetArgs(MakeVectorArg(As<Cell>(args[0])->GetSecond()));
            new_lambda_function->SetScope(scope);
            for (size_t ind = 1; ind < args.size(); ++ind) {
                new_lambda_function->AddToLambda(args[ind]);
            }
            scope->scheme_map[As<Symbol>(As<Cell>(args[0])->GetFirst())->GetName()] =
                new_lambda_function;
        }
        return nullptr;
    }
    if (Is<Symbol>(args[0])) {
        scope->GetSecond(As<Symbol>(args[0])->GetName());
        scope->scheme_map[As<Symbol>(args[0])->GetName()] = args[1]->Eval(scope);
    } else {
        throw RuntimeError("RuntimeError");
    }
    return nullptr;
}

std::shared_ptr<Object> Lambda::Apply(const std::shared_ptr<Scope>& scope,
                                      const std::vector<std::shared_ptr<Object>>& args) {
    if (args.size() < 2 || (!Is<Cell>(args[0]) && args[0])) {
        throw SyntaxError("Syntax error");
    }
    auto new_lambda = std::make_shared<LambdaFunction>();
    new_lambda->SetArgs(MakeVectorArg(args[0]));
    auto local_scope = std::make_shared<Scope>();
    local_scope->parent = scope;
    new_lambda->SetScope(local_scope);
    for (size_t ind = 1; ind < args.size(); ++ind) {
        new_lambda->AddToLambda(args[ind]);
    }
    return new_lambda;
}

std::shared_ptr<Object> LambdaFunction::Apply(const std::shared_ptr<Scope>& scope,
                                              const std::vector<std::shared_ptr<Object>>& args) {
    for (size_t ind = 0; ind < args.size(); ++ind) {
        current_scope_->scheme_map[As<Symbol>(args_[ind])->GetName()] = args[ind]->Eval(scope);
    }
    size_t i = 0;
    for (const auto& obj : lambda_) {
        if (i != lambda_.size() - 1) {
            obj->Eval(current_scope_);
        } else {
            return obj->Eval(current_scope_);
        }
        ++i;
    }
    return nullptr;
}

std::vector<std::shared_ptr<Object>> MakeVectorArg(const std::shared_ptr<Object>& head) {
    std::vector<std::shared_ptr<Object>> elements;
    if (head) {
        auto current = As<Cell>(head);
        while (current != nullptr) {
            elements.push_back(current->GetFirst());
            auto next = current->GetSecond();
            if (!Is<Cell>(next) && next) {
                throw std::runtime_error("wrong argument list");
            }
            current = As<Cell>(next);
        }
    }
    return elements;
}

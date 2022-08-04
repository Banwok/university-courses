#include "scheme.h"
#include "parser.h"

Interpretor::Interpretor() : global_scope_(std::make_shared<Scope>()) {
    global_scope_->scheme_map["+"] = std::make_shared<SimpleOperations>(Math::Plus);
    global_scope_->scheme_map["-"] = std::make_shared<SimpleOperations>(Math::Minus);
    global_scope_->scheme_map["*"] = std::make_shared<SimpleOperations>(Math::Multiply);
    global_scope_->scheme_map["/"] = std::make_shared<SimpleOperations>(Math::Divide);

    global_scope_->scheme_map["not"] = std::make_shared<SimpleCheker>(Cheks::Not);
    global_scope_->scheme_map["null?"] = std::make_shared<SimpleCheker>(Cheks::Null);
    global_scope_->scheme_map["pair?"] = std::make_shared<SimpleCheker>(Cheks::Pair);
    global_scope_->scheme_map["number?"] = std::make_shared<SimpleCheker>(Cheks::Number);
    global_scope_->scheme_map["boolean?"] = std::make_shared<SimpleCheker>(Cheks::Boolean);
    global_scope_->scheme_map["symbol?"] = std::make_shared<SimpleCheker>(Cheks::Symbol);
    global_scope_->scheme_map["list?"] = std::make_shared<SimpleCheker>(Cheks::List);

    global_scope_->scheme_map["="] = std::make_shared<Comparison>(ComparOperators::Equality);
    global_scope_->scheme_map[">"] = std::make_shared<Comparison>(ComparOperators::More);
    global_scope_->scheme_map["<"] = std::make_shared<Comparison>(ComparOperators::Less);
    global_scope_->scheme_map[">="] = std::make_shared<Comparison>(ComparOperators::MoreOrEqual);
    global_scope_->scheme_map["<="] = std::make_shared<Comparison>(ComparOperators::LessOrEqual);

    global_scope_->scheme_map["min"] = std::make_shared<MinMax>(MinAndMax::Min);
    global_scope_->scheme_map["max"] = std::make_shared<MinMax>(MinAndMax::Max);

    global_scope_->scheme_map["car"] = std::make_shared<GeterSeter>(CarCdr::Car);
    global_scope_->scheme_map["cdr"] = std::make_shared<GeterSeter>(CarCdr::Cdr);
    global_scope_->scheme_map["set-car!"] = std::make_shared<GeterSeter>(CarCdr::SetCar);
    global_scope_->scheme_map["set-cdr!"] = std::make_shared<GeterSeter>(CarCdr::SetCdr);

    global_scope_->scheme_map["list"] = std::make_shared<SomeList>(Lists::List);
    global_scope_->scheme_map["list-ref"] = std::make_shared<SomeList>(Lists::ListRef);
    global_scope_->scheme_map["list-tail"] = std::make_shared<SomeList>(Lists::ListTail);

    global_scope_->scheme_map["abs"] = std::make_shared<NumberMod>(AbsCons::Abs);
    global_scope_->scheme_map["cons"] = std::make_shared<NumberMod>(AbsCons::Cons);

    global_scope_->scheme_map["quote"] = std::make_shared<LogicOperators>(Logic::Quote);
    global_scope_->scheme_map["if"] = std::make_shared<LogicOperators>(Logic::If);
    global_scope_->scheme_map["and"] = std::make_shared<LogicOperators>(Logic::And);
    global_scope_->scheme_map["or"] = std::make_shared<LogicOperators>(Logic::Or);

    global_scope_->scheme_map["define"] = std::make_shared<ModifVariables>(Variables::Define);
    global_scope_->scheme_map["set!"] = std::make_shared<ModifVariables>(Variables::Set);
    global_scope_->scheme_map["lambda"] = std::make_shared<Lambda>();
}

std::shared_ptr<Object> Interpretor::Eval(std::shared_ptr<Object> in) {
    if (in == nullptr) {
        throw RuntimeError("First element of the list must be function");
    }
    return in->Eval(global_scope_);
}
#include "scheme_test.h"

void SchemeTest::ExpectEq(std::string expression, std::string result) {
    std::stringstream ss{expression};
    Tokenizer tokenizer{&ss};
    auto obj = Read(&tokenizer);
    auto res = interpretator.Eval(obj);
    std::ostringstream os;
    PrintGlobal(res, &os);
    REQUIRE(os.str() == result);
}

void SchemeTest::ExpectNoError(std::string expression) {
    std::stringstream ss{expression};
    Tokenizer tokenizer{&ss};
    auto obj = Read(&tokenizer);
    REQUIRE_NOTHROW(interpretator.Eval(obj));
}

void SchemeTest::ExpectSyntaxError(std::string expression) {
    std::stringstream ss{expression};
    Tokenizer tokenizer{&ss};
    REQUIRE_THROWS_AS(interpretator.Eval(Read(&tokenizer)), SyntaxError);
}

void SchemeTest::ExpectRuntimeError(std::string expression) {
    std::stringstream ss{expression};
    Tokenizer tokenizer{&ss};
    auto obj = Read(&tokenizer);
    REQUIRE_THROWS_AS(interpretator.Eval(obj), RuntimeError);
}

void SchemeTest::ExpectNameError(std::string expression) {
    std::stringstream ss{expression};
    Tokenizer tokenizer{&ss};
    auto obj = Read(&tokenizer);
    REQUIRE_THROWS_AS(interpretator.Eval(obj), NameError);
}
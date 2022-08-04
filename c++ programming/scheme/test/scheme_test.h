#pragma once

#include <catch.hpp>
#include "scheme.h"

struct SchemeTest {
    SchemeTest() = default;

    Interpretor interpretator = Interpretor();

    void ExpectEq(std::string expression, std::string result);

    void ExpectNoError(std::string expression);

    void ExpectSyntaxError(std::string expression);

    void ExpectRuntimeError(std::string expression);

    void ExpectNameError(std::string expression);
};
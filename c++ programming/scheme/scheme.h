#pragma once

#include <memory>
#include <string>
#include <parser.h>
#include <sstream>

class Interpretor {
public:
    Interpretor();

    ~Interpretor() {
        global_scope_->scheme_map.clear();
    }

    std::shared_ptr<Object> Eval(std::shared_ptr<Object> in);

private:
    std::shared_ptr<Scope> global_scope_;
};
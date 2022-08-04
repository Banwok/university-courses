#include <parser.h>
#include <istream>

std::shared_ptr<Object> Read(Tokenizer* tokenizer) {
    Token token;
    if (!tokenizer->IsEnd()) {
        token = tokenizer->GetToken();
    } else {
        return nullptr;
    }

    if (std::get_if<ConstantToken>(&token)) {
        tokenizer->Next();
        auto tok = std::get<ConstantToken>(token).value;
        return std::make_shared<Number>(tok);
    }
    if (SymbolToken* tok = std::get_if<SymbolToken>(&token)) {
        tokenizer->Next();
        if (tok->name == "#f") {
            return std::make_shared<Boolean>(false);
        } else if (tok->name == "#t") {
            return std::make_shared<Boolean>(true);
        }
        return std::make_shared<Symbol>(tok->name);
    }
    if (std::get_if<QuoteToken>(&token)) {
        tokenizer->Next();
        auto new_cell = std::make_shared<Cell>();
        auto list = std::make_shared<Cell>();
        new_cell->SetFirst(std::make_shared<Symbol>("quote"));
        list->SetFirst(Read(tokenizer));
        new_cell->SetSecond(list);
        return new_cell;
    }
    if (std::get_if<BracketToken>(&token)) {
        auto tok = tokenizer->GetToken();
        return ReadList(tokenizer);
    }
    throw SyntaxError("Unknown token");
}

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer) {
    bool found_dot = false;
    bool after_dot = false;
    bool fl = true;
    bool close = false;
    Token token;
    std::shared_ptr<Cell> current_ptr = nullptr;
    std::shared_ptr<Cell> first_ptr = nullptr;

    tokenizer->Next();
    while (fl) {
        if (!tokenizer->IsEnd()) {
            token = tokenizer->GetToken();
        } else {
            throw SyntaxError("EOF problem");
        }
        close = (token == Token{BracketToken::CLOSE});
        if (after_dot && !close) {
            throw SyntaxError("Closing bracket problem");
        }
        found_dot = (token == Token{DotToken{}});
        if (found_dot) {
            tokenizer->Next();
        }
        if (close) {
            if (found_dot && !after_dot) {
                throw SyntaxError("Dot problem");
            }
            tokenizer->Next();
            tokenizer->ChekError();
            return first_ptr;
        }
        auto obj_ptr = Read(tokenizer);
        if (first_ptr == nullptr) {
            if (found_dot) {
                throw SyntaxError("Dot problem");
            }
            first_ptr = std::shared_ptr<Cell>(new Cell);
            current_ptr = first_ptr;
            current_ptr->SetFirst(obj_ptr);
        } else {
            if (!found_dot) {
                auto new_cell_ptr = std::shared_ptr<Cell>(new Cell);
                current_ptr->SetSecond(new_cell_ptr);
                current_ptr = new_cell_ptr;
                current_ptr->SetFirst(obj_ptr);
            } else {
                current_ptr->SetSecond(obj_ptr);
                after_dot = true;
            }
        }
    }
}

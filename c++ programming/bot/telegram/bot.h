#pragma once
#include "client.h"

class Bot {
public:
    Bot(std::string& token) : token_(token), client_("https://api.telegram.org", token_) {
    }
    void OffsetUpdate(int id) {
        offset_ = id;
        std::ofstream file;
        file.open("offset.txt");
        file << offset_;
        file.close();
    }
    void ReadOffset() {
        std::ifstream file;
        file.open("offset.txt");
        file >> offset_;
        file.close();
    }
    void Run();

private:
    int timeout_ = 30;
    bool bot_run_ = true;
    int offset_ = 0;
    void Stop();
    void Crash();
    std::string token_;
    Client client_;
};
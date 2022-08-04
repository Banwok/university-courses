#include "bot.h"
#include <random>

void Bot::Run() {
    ReadOffset();
    while (bot_run_) {
        auto messages = client_.GetMessages(offset_ + 1, timeout_);
        for (auto& i : messages) {
            auto str = i.str_;
            OffsetUpdate(i.up_id_);
            if (str == "/start") {
                client_.SendMessage(i.ch_id_,
                                    "\n/random \n/weather \n/styleguide \n/stop \n/crash");
            } else if (str == "/random") {
                static std::mt19937 generator;
                std::uniform_int_distribution random_number(0, 1000000);
                int number = random_number(generator);
                client_.SendMessage(i.ch_id_, std::to_string(number));
            } else if (str == "/weather") {
                client_.SendMessage(i.ch_id_, "Winter is coming");
            } else if (str == "/styleguide") {
                client_.SendMessage(i.ch_id_, "joke");
            } else if (str == "/stop") {
                bot_run_ = false;
            } else if (str == "/crash") {
                abort();
            } else {
                client_.SendMessage(i.ch_id_, "Please use this command:");
                client_.SendMessage(i.ch_id_,
                                    "\n /random \n /weather \n /styleguide \n /stop \n /crash");
            }
        }
    }
}
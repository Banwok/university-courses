#include <iostream>
#include "telegram/bot.h"

int main() {
    std::string token = "5254417764:AAGDtULQ_gR9s19mc2bXEVOpdjiFS1vuK1E";
    Bot bot(token);
    bot.Run();
    return 0;
}
#pragma once

#include <array>
#include <stdexcept>
#include <queue>

enum Winner { kFirst, kSecond, kNone };

struct GameResult {
    Winner winner;
    int turn;
};

GameResult SimulateWarGame(const std::array<int, 5>& first_deck,
                           const std::array<int, 5>& second_deck) {
    GameResult res;
    std::queue<int> deck1;
    std::queue<int> deck2;
    int d1, d2;
    for (auto i : first_deck) {
        deck1.push(i);
    }
    for (auto i : second_deck) {
        deck2.push(i);
    }
    res.turn = 0;
    while (!deck1.empty() && !deck2.empty() && res.turn < 1'000'000) {
        d1 = deck1.front();
        d2 = deck2.front();
        deck1.pop();
        deck2.pop();
        res.turn++;
        if (d1 == 0 && d2 == 9) {
            deck1.push(d1);
            deck1.push(d2);
        } else if (d2 == 0 && d1 == 9) {
            deck2.push(d1);
            deck2.push(d2);
        } else if (d1 > d2) {
            deck1.push(d1);
            deck1.push(d2);
        } else if (d1 < d2) {
            deck2.push(d1);
            deck2.push(d2);
        } else if (d1 == d2) {
            deck1.push(d1);
            deck2.push(d2);
        }
    }
    if (deck1.empty()) {
        res.winner = Winner::kSecond;
    } else if (deck2.empty()) {
        res.winner = Winner::kFirst;
    } else {
        res.winner = Winner::kNone;
    }
    return res;
}

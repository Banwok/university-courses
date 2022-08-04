#pragma once

#include "dungeon.h"

#include <stdexcept>
#include <unordered_map>
#include <list>
#include <vector>

int* Val(Door* door) {
    if (door == nullptr) {
        return nullptr;
    }
    return reinterpret_cast<int*>(door);
}
int* Val(Room* room) {
    if (room == nullptr) {
        return nullptr;
    }
    return reinterpret_cast<int*>(room);
}

Room* FindFinalRoom(Room* starting_room) {
    std::vector<Room*> move;
    std::unordered_map<int*, bool> visit;
    std::vector<Door*> cl_doors;
    std::list<std::string> keys;
    Room* curr;
    int odoor = 0;
    move.push_back(starting_room);
    while (!move.empty()) {
        curr = move.back();
        visit[Val(curr)] = true;
        move.pop_back();
        if ((*curr).IsFinal()) {
            return curr;
        }
        for (size_t i = 0; i < (*curr).NumKeys(); ++i) {
            keys.push_back((*curr).GetKey(i));
            auto it = cl_doors.begin();
            auto end = cl_doors.end();
            while (it != end) {
                if ((*it)->TryOpen((*curr).GetKey(i))) {
                    if (visit.find(Val(*it)) == visit.end()) {
                        visit[Val(*it)] = true;
                        --odoor;
                        move.push_back((*it)->GoThrough());
                    }
                    cl_doors.erase(it);
                    ++odoor;
                    break;
                }
                ++it;
            }
        }
        for (size_t i = 0; i < (*curr).NumDoors(); ++i) {
            auto door = (*curr).GetDoor(i);
            if ((*door).IsOpen()) {
                if (visit.find(Val(door)) == visit.end()) {
                    visit[Val(door)] = true;
                    move.push_back((*door).GoThrough());
                }
            } else {
                for (auto& key : keys) {
                    if ((*door).TryOpen(key)) {
                        if (visit.find(Val(door)) == visit.end()) {
                            visit[Val(door)] = true;
                            move.push_back((*door).GoThrough());
                        }
                        break;
                    }
                }
                cl_doors.push_back(door);
                ++odoor;
            }
        }
    }
    return nullptr;
}
#pragma once

#include <vector>
#include <stdexcept>

void FilterEven(std::vector<int> *data) {
    (*data).erase(remove_if((*data).begin(), (*data).end(), [](int a) { return a % 2 != 0; }),
                  (*data).end());
}

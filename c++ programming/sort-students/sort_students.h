#pragma once

#include <vector>
#include <stdexcept>
#include <algorithm>

struct Student {
    std::string name, surname;
    int year, month, day;
};

enum class SortType { kByName, kByDate };

bool SortName(Student a, Student b) {
    if (a.surname < b.surname) {
        return true;
    } else if (a.surname == b.surname) {
        if (a.name < b.name) {
            return true;
        } else if (a.name == b.name) {
            if (a.year < b.year) {
                return true;
            } else if (a.year == b.year) {
                if (a.month < b.month) {
                    return true;
                } else if (a.month == b.month) {
                    if (a.day < b.day) {
                        return true;
                    } else if (a.day == b.day) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool SortDate(Student a, Student b) {
    if (a.year < b.year) {
        return true;
    } else if (a.year == b.year) {
        if (a.month < b.month) {
            return true;
        } else if (a.month == b.month) {
            if (a.day < b.day) {
                return true;
            } else if (a.day == b.day) {
                if (a.surname < b.surname) {
                    return true;
                } else if (a.surname == b.surname) {
                    if (a.name < b.name) {
                        return true;
                    } else if (a.name == b.name) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

inline void SortStudents(std::vector<Student> *students, SortType sort_type) {
    if (sort_type == SortType::kByName) {
        std::sort((*students).begin(), (*students).end(), SortName);
    }
    if (sort_type == SortType::kByDate) {
        std::sort((*students).begin(), (*students).end(), SortDate);
    }
}

#pragma once

#include <stdexcept>
#include <map>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

struct StudentName {
    std::string name, surname;
};

struct Date {
    int year, month, day;
};

bool SortBall(std::tuple<StudentName, Date, int, std::vector<std::string>> a,
              std::tuple<StudentName, Date, int, std::vector<std::string>> b) {
    if (std::get<int>(a) > std::get<int>(b)) {
        return true;
    } else if (std::get<int>(a) == std::get<int>(b)) {
        Date a_data = std::get<Date>(a);
        Date b_data = std::get<Date>(b);
        if (a_data.year < b_data.year) {
            return true;
        } else if (a_data.year == b_data.year) {
            if (a_data.month < b_data.month) {
                return true;
            } else if (a_data.month == b_data.month) {
                if (a_data.day < b_data.day) {
                    return true;
                } else if (a_data.day == b_data.day) {
                    StudentName a_nm = std::get<StudentName>(a);
                    StudentName b_nm = std::get<StudentName>(b);
                    if (a_nm.surname < b_nm.surname) {
                        return true;
                    } else if (a_nm.surname == b_nm.surname) {
                        if (a_nm.name < b_nm.name) {
                            return true;
                        } else if (a_nm.name == b_nm.name) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool SortName(StudentName a, StudentName b) {
    if (a.surname < b.surname) {
        return true;
    } else if (a.surname == b.surname) {
        if (a.name < b.name) {
            return true;
        } else if (a.name == b.name) {
            return true;
        }
    }
    return false;
}

std::map<std::string, std::vector<StudentName>> GetStudents(
    const std::vector<std::pair<std::string, int>>& university_info,
    const std::vector<std::tuple<StudentName, Date, int, std::vector<std::string>>>&
        students_info) {
    std::map<std::string, std::vector<StudentName>> res;
    std::map<std::string, int> tmp;
    std::vector<std::tuple<StudentName, Date, int, std::vector<std::string>>> students_sorted =
        students_info;
    std::sort(students_sorted.begin(), students_sorted.end(), SortBall);
    for (auto i : university_info) {
        std::vector<StudentName> nm;
        res[i.first] = nm;
    }
    for (auto i : university_info) {
        tmp[i.first] = (i.second);
    }

    for (auto it : students_sorted) {
        auto vec_prior = std::get<std::vector<std::string>>(it);
        for (auto j : vec_prior) {
            auto map_iter = res.find(j);
            if (map_iter == res.end()) {
                break;
            } else if (tmp[j] != 0) {
                (map_iter->second).push_back(std::get<StudentName>(it));
                tmp[j]--;
                break;
            }
        }
    }

    for (auto i : res) {
        std::sort((i.second).begin(), (i.second).end(), SortName);
        res.insert_or_assign(i.first, i.second);
    }
    return res;
}

#include <catch.hpp>
#include <sort_students.h>

#include <iostream>

bool operator==(const Student& lhs, const Student& rhs) {
    return lhs.name == rhs.name && lhs.surname == rhs.surname && lhs.year == rhs.year &&
           lhs.month == rhs.month && lhs.day == rhs.day;
}

std::ostream& operator<<(std::ostream& s, const Student& lhs) {
    s << "(" << lhs.name << "," << lhs.surname << "," << lhs.day << "," << lhs.month << ","
      << lhs.year << ")";
    return s;
}

Student MakeStudent(std::string_view name, std::string_view surname, int year, int month, int day) {
    Student result;
    result.name = name;
    result.surname = surname;
    result.year = year;
    result.month = month;
    result.day = day;
    return result;
}

TEST_CASE("by_date") {
    std::vector<Student> test;
    std::vector<Student> expected;
    test.push_back(MakeStudent("c", "b", 1999, 9, 19));
    test.push_back(MakeStudent("a", "b", 2000, 8, 12));
    test.push_back(MakeStudent("b", "a", 2000, 11, 12));
    test.push_back(MakeStudent("a", "c", 1998, 10, 10));
    test.push_back(MakeStudent("a", "b", 2000, 11, 12));

    expected.push_back(MakeStudent("a", "c", 1998, 10, 10));
    expected.push_back(MakeStudent("c", "b", 1999, 9, 19));
    expected.push_back(MakeStudent("a", "b", 2000, 8, 12));
    expected.push_back(MakeStudent("b", "a", 2000, 11, 12));
    expected.push_back(MakeStudent("a", "b", 2000, 11, 12));

    SortStudents(&test, SortType::kByDate);
    REQUIRE(expected == test);
}

TEST_CASE("by_name") {
    std::vector<Student> test;
    std::vector<Student> expected;
    test.push_back(MakeStudent("c", "b", 1999, 9, 19));
    test.push_back(MakeStudent("a", "b", 2000, 8, 12));
    test.push_back(MakeStudent("b", "a", 2000, 11, 12));
    test.push_back(MakeStudent("a", "c", 1998, 10, 10));
    test.push_back(MakeStudent("a", "b", 2000, 11, 12));

    expected.push_back(MakeStudent("b", "a", 2000, 11, 12));
    expected.push_back(MakeStudent("a", "b", 2000, 8, 12));
    expected.push_back(MakeStudent("a", "b", 2000, 11, 12));
    expected.push_back(MakeStudent("c", "b", 1999, 9, 19));
    expected.push_back(MakeStudent("a", "c", 1998, 10, 10));

    SortStudents(&test, SortType::kByName);
    REQUIRE(expected == test);
}

TEST_CASE("GUSini") {
    std::vector<Student> test;
    std::vector<Student> expected;
    test.push_back(MakeStudent("a", "b", 2000, 8, 12));
    test.push_back(MakeStudent("b", "c", 2000, 11, 12));
    test.push_back(MakeStudent("a", "b", 2000, 8, 12));

    expected.push_back(MakeStudent("a", "b", 2000, 8, 12));
    expected.push_back(MakeStudent("a", "b", 2000, 8, 12));
    expected.push_back(MakeStudent("b", "c", 2000, 11, 12));

    SortStudents(&test, SortType::kByName);
    REQUIRE(expected == test);
}

#pragma once

#include <vector>

namespace aligusnet {
    struct Person
    {
        unsigned name;
        unsigned age;
        float latitude;
        float longitude;
    };

    float person_distance(const aligusnet::Person& p1, const aligusnet::Person& p2);
    std::vector<Person> get_person9_test_data();
}

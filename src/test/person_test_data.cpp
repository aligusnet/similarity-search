#include "person_test_data.h"
#include <cmath>

namespace aligusnet {
    float person_distance(const Person& p1, const Person& p2)
    {
        float phi = p1.latitude - p2.latitude;
        float lambda = p1.longitude - p2.longitude;
        float age = static_cast<float>(p1.age) - p2.age;
        return sqrt(phi * phi + lambda * lambda + age * age);
    }

    std::vector<Person> get_person9_test_data()
    {
        std::vector<Person> persons{
            { 1, 100, 0, 0 },
            { 2, 10, 10, 0 },
            { 3, 30, 10, 10 },
            { 4, 50, 0, 30 },
            { 5, 50, 5, 5 },
            { 6, 70, 20, 10 },
            { 7, 20, 30, 0 },
            { 8, 25, 10, 40 },
            { 9, 45, 30, 30 },
        };

        return persons;
    }
}

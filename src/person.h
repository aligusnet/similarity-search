#pragma once
#include <iosfwd>

namespace aligusnet {

struct Person {
  unsigned name;
  unsigned age;
  float latitude;
  float longitude;
};

std::ostream &operator<<(std::ostream &os, const Person &person);

}  // namespace aligusnet

#include "person.h"
#include <iostream>

namespace aligusnet {

std::ostream &operator<<(std::ostream &os, const Person &person) {
  os << "[Person: "
     << person.name << ", "
     << person.age << " years old ("
     << person.latitude << ", "
     << person.longitude << ")]";
  return os;
}

}  // namespace aligusnet

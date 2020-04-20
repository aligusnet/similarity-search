#pragma once
#include <vector>
#include "person.h"

namespace aligusnet {

struct GeneratorParams {
  struct NormalParams {
    float mean;
    float stddev;
  };

  struct UniformParams {
    unsigned lower;
    unsigned upper;
  };

  NormalParams longitude;
  NormalParams latitude;
  UniformParams age;

  unsigned number;
};

std::vector<Person> generate(const std::vector<GeneratorParams> &params_list);

}  // namespace aligusnet

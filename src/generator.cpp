#include "generator.h"
#include <random>

namespace aligusnet {

void generate(const GeneratorParams &params,
              unsigned start_index,
              std::vector<Person> *ppeople) {

    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned> age_distribution(params.age.lower, params.age.upper);
    std::normal_distribution<float> longitude_distribution(params.longitude.mean, params.longitude.stddev);
    std::normal_distribution<float> latitude_distribution(params.longitude.mean, params.longitude.stddev);

    std::vector<Person> &people = *ppeople;
    for (unsigned i = 0; i < params.number; ++i) {
      unsigned index = i + start_index;
      people[index].name = index+1;
      people[index].age = age_distribution(generator);
      people[index].longitude = longitude_distribution(generator);
      people[index].latitude = latitude_distribution(generator);
    }
}


std::vector<Person> generate(const std::vector<GeneratorParams> &params_list) {
  unsigned total_number = 0;
  for (const auto &params: params_list) {
    total_number += params.number;
  }

  std::vector<Person> people(total_number);

  unsigned start_index = 0;
  for (const auto &params: params_list) {
    generate(params, start_index, &people);
    start_index += params.number;
  }
  return people;
}

}  // namespace aligusnet

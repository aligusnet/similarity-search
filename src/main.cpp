#include <cmath>
#include <iostream>
#include <chrono>
#include <numeric>

#include "generator.h"
#include "vantage_point_tree.h"
#include "naive_knn.h"

const unsigned g_neighbors_number = 10;


// A world cosists of cities.
// Every city is described by GeneratorParams.
using World = std::vector<aligusnet::GeneratorParams>;
namespace worlds {
  // 1,000,000
  World small = {
    {{-40, 20}, {80, 10}, {13, 100}, 300000},
    {{50, 30}, {-30, 30}, {7, 110}, 250000},
    {{1, 40}, {50, 3}, {0, 80}, 350000},
    {{-10, 30}, {70, 20}, {10, 70}, 100000}
  };

  // 100,000,000
  World normal = {
    {{-40, 20}, {80, 10}, {13, 100}, 30000000},
    {{50, 30}, {-30, 30}, {7, 110}, 25000000},
    {{1, 40}, {50, 3}, {0, 80}, 35000000},
    {{-10, 30}, {70, 20}, {10, 70}, 10000000}
  };

  // 300,000,000
  World gigantic = {
    {{-40, 20}, {80, 10}, {13, 100}, 90000000},
    {{50, 30}, {-30, 30}, {7, 110}, 75000000},
    {{1, 40}, {50, 3}, {0, 80}, 105000000},
    {{-10, 30}, {70, 20}, {10, 70}, 30000000}
  };

  // test world
  World test = {
    {{40, 30}, {70, 10}, {0, 100}, 10000},
    {{-50, 30}, {20, 30}, {0, 110}, 10000},
    {{10, 40}, {20, 30}, {0, 80}, 10000},
    {{-70, 30}, {-70, 20}, {0, 70}, 10000}
  };
}

// calculates 'distance' between 2 persons
// it is used to find nearest persons
float distance(const aligusnet::Person& p1, const aligusnet::Person& p2)
{
    float phi = p1.latitude-p2.latitude;
    float lambda = p1.longitude-p2.longitude;
    float age = static_cast<float>(p1.age) - p2.age;
    return sqrt(phi*phi + lambda*lambda + age*age);
}


// generates people from given world params and
// builds vantage point tree, a data structure
// that facilitates searching of nearest persons
aligusnet::VantagePointTree<aligusnet::Person> build_vpt(const World &world) {
  using namespace aligusnet;

  auto begin = std::chrono::high_resolution_clock::now();
  std::vector<Person> people = generate(world);
  auto end = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
  std::cout << people.size() << " persons generation took " << duration << " ms" << std::endl;

  begin = std::chrono::high_resolution_clock::now();
  VantagePointTree<Person> vpt(std::move(people), distance);
  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
  std::cout << "tree construction took " << duration << " ms" << std::endl;
  return vpt;
}


void run_ui(const World &world) {
  using namespace aligusnet;

  auto vpt = build_vpt(world);

  std::cout << "Please type 'quit' or press Ctrl+D to quit." << std::endl;
  while (std::cin) {
    std::cout << "Please type age (unsigned), latitude and longitude (both floats) separated by spaces: " << std::endl;
    std::cout << "> ";
    unsigned age;
    float latitude, longitude;
    if (!(std::cin >> age >> latitude >> longitude)) {
      std::cout << std::endl << "Goodbye!" << std::endl;
      break;
    }

    Person target{0, age, latitude, longitude};
    std::cout << "Searching for neighbors of " << target << "..." << std::endl;
    auto begin = std::chrono::high_resolution_clock::now();
    auto res = vpt.search(target, g_neighbors_number);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count();
    std::cout << "search took " << duration << " microseconds" << std::endl;
    for (size_t i = 0; i < res.size(); ++i) {
      std::cout << res[i].first << " " << res[i].second << std::endl;
    }
  }
}

void run_benchmark(const World &world) {
  auto vpt = build_vpt(world);

  auto tests = generate(worlds::test);
  std::vector<int> durations;
  durations.reserve(tests.size());

  for (const auto &test : tests) {
    auto begin = std::chrono::high_resolution_clock::now();
    auto res = vpt.search(test, g_neighbors_number);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count();
    durations.push_back(duration);
  }

  std::sort(durations.begin(), durations.end());

  double mean = std::accumulate(durations.begin(), durations.end(), 0);
  mean /= durations.size();

  auto quantile95 = durations[durations.size()*0.95];
  auto quantile99 = durations[durations.size()*0.99];

  std::cout << "average value: " << mean << " microseconds" << std::endl;
  std::cout << "0.95 quantile: " << quantile95 << " microseconds" << std::endl;
  std::cout << "0.99 quantile: " << quantile99 << " microseconds" << std::endl;
}

float calculate_error(const std::vector<std::pair<aligusnet::Person, float>> &lhs,
                      const std::vector<std::pair<aligusnet::Person, float>> &rhs) {
  auto acc_fn = [](float acc, const std::pair<aligusnet::Person, float> &p1) {
    return acc + p1.second;
  };

  float lhs_sum = std::accumulate(lhs.begin(), lhs.end(), 0.0, acc_fn);
  float rhs_sum = std::accumulate(rhs.begin(), rhs.end(), 0.0, acc_fn);

  return fabs(lhs_sum - rhs_sum);
}

void run_test(const World &world) {
  using namespace aligusnet;

  auto people = generate(world);
  auto tests = generate(worlds::test);

  VantagePointTree<Person> vpt(people, distance);
  NaiveKNN<Person> naive(people, distance);
  float accumulated_error = 0;
  unsigned iter_no = 0;
  for (const auto &test : tests) {
    auto res_vpt = vpt.search(test, g_neighbors_number);
    auto res_naive = naive.search(test, g_neighbors_number);
    accumulated_error += calculate_error(res_vpt, res_naive);
    ++iter_no;
    if (iter_no % 10 == 0) {
      std::cout << "After " << iter_no << " iterations.";
      std::cout << " Accumulated error: " << accumulated_error;
      std::cout << ". Average error: " << accumulated_error/iter_no;
      std::cout << "\r";
      std::cout.flush();
    }
  }

  std::cout << std::endl;
}

void usage(const char *progname) {
  std::cout << "Usage: " << progname << " <action> <world>" << std::endl;
  std::cout << "supported actions:" << std::endl;
  std::cout << "\tui\t\t - run ui" << std::endl;
  std::cout << "\tbenchmark\t - run performance test" << std::endl;
  std::cout << "\ttest\t\t - run tests" << std::endl;
  std::cout << "supported worlds:" << std::endl;
  std::cout << "\tsmall\t\t - 1 mln world" << std::endl;
  std::cout << "\tnormal\t\t - 100 mln world" << std::endl;
  std::cout << "\tgigantic\t - 300 mln world" << std::endl;
}

const World &get_world(char c) {
  switch (c) {
    case 's': return worlds::small;
    case 'n': return worlds::normal;
    case 'g': return worlds::gigantic;
    default: throw std::runtime_error("invalid world");
  }
}


std::function<void(const World &)> get_action(char c) {
  switch (c) {
    case 'u': return run_ui;
    case 'b': return run_benchmark;
    case 't': return run_test;
    default: throw std::runtime_error("invalid action");
  }
}

int main(int argc, char **argv) {

  if (argc != 3) {
    usage(argv[0]);
    return 0;
  }

  try {
    auto action = get_action(argv[1][0]);
    auto world = get_world(argv[2][0]);
    action(world);
    return 0;
  } catch (const std::exception &ex) {
    std::cerr << "caught exception: " << ex.what() << std::endl;
    usage(argv[0]);
    return 1;
  }

}

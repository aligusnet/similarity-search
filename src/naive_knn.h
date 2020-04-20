#pragma once
#include <vector>
#include <algorithm>

namespace aligusnet {

// Naive implementation of K Nearest Neighbors algorithm
// seearch time is O(n + k*log(k)) = O(n) if k << n
template <typename T>
class NaiveKNN {
public:
  using DistanceFn = std::function<float (const T&, const T&)>;

  NaiveKNN(const std::vector<T> &items, DistanceFn dist_fn):
      items_(items), dist_fn_(dist_fn) {
  }

  // Search K Nearest Neighbors
  std::vector<std::pair<T, float>> search(const T &target, unsigned k) {
    auto cmp_fn = [target, this] (const T &lhs, const T &rhs) {
      return dist_fn_(target, lhs) < dist_fn_(target,  rhs);
    };

    std::nth_element(items_.begin(), items_.begin()+k, items_.end(), cmp_fn);

    std::sort(items_.begin(), items_.begin()+k, cmp_fn);

    std::vector<std::pair<T, float>> results;
    for (size_t i = 0; i < k; ++i) {
      results.push_back({items_[i], dist_fn_(target, items_[i])});
    }

    return results;
  }

private:
  std::vector<T> items_;
  DistanceFn dist_fn_;
};

}  // namespace aligusnet


#pragma once

#include <vector>
#include <queue>
#include <algorithm>
#include <functional>

namespace aligusnet {

// Vantage-Point tree
// https://en.wikipedia.org/wiki/Vantage-point_tree
// Complexity guarantee from http://pnylab.com/pny/papers/vptree/vptree.pdf:
// Vantage-Point tree construction executes in O(n*log(n))
// and expected search time is O(log(n)).

template <typename T>
class VantagePointTree {
public:
  using DistanceFn = std::function<float (const T&, const T&)>;
  VantagePointTree(std::vector<T> &&items, DistanceFn dist_fn):
      items_(items), dist_fn_(dist_fn) {
        init();
  }

  VantagePointTree(const std::vector<T> &items, DistanceFn dist_fn):
      items_(items), dist_fn_(dist_fn) {
        init();
  }

  // Search K Nearest Neighbors
  std::vector<std::pair<T, float>> search(const T &target, unsigned k) const {
    NeighborFinder finder(this, target, k);
    finder.search(root_index_);
    return finder.build_results();
  }

private:
  struct Node {
    unsigned index=-1;  // index of input point
    unsigned left=-1;   // index of the left child
    unsigned right=-1;  // index of the right child
    float bound;        // radius
  };

  std::vector<T> items_;
  std::vector<Node> nodes_;
  DistanceFn dist_fn_;
  unsigned root_index_;  // always 0 but keep it to avoid "magic numbers"

  void init() {
    std::random_shuffle(items_.begin(), items_.end());
    nodes_.resize(items_.size());
    root_index_ = build(0, items_.size());
  }

  unsigned build(unsigned start, unsigned finish) {
    if (start >= finish) {
      return items_.size();
    }
    Node &node = nodes_[start];
    node.index = start;

    if (finish - start == 1) {
      return start;
    }

    unsigned median = (start + finish + 1)/2;
    auto begin = items_.begin();
    const auto &start_item = items_[start];
    auto cmp_fn = [start_item, this] (const T &lhs, const T &rhs) {
      return dist_fn_(start_item, lhs) < dist_fn_(start_item,  rhs);
    };
    std::nth_element(begin + start + 1, begin + median, begin + finish, cmp_fn);

    node.bound = dist_fn_(start_item, items_[median]);
    node.left = build(start+1, median);
    node.right = build(median, finish);

    return start;
  }

  struct QueueItem {
    unsigned index;
    float distance;
    bool operator<( const QueueItem& rhs ) const {
        return distance < rhs.distance;
    }
  };

  struct NeighborFinder {
    const unsigned k;
    const T &target;
    const std::vector<T> &items;
    const std::vector<Node> &nodes;
    std::priority_queue<QueueItem> queue;
    DistanceFn dist_fn;
    float record;

    NeighborFinder(const VantagePointTree<T> *vpt, const T &target, unsigned k):
      k(k),
      target(target),
      items(vpt->items_),
      nodes(vpt->nodes_),
      queue{},
      dist_fn(vpt->dist_fn_),
      record{std::numeric_limits<float>::max()} {
    }

    void search(unsigned index) {
      if ( !is_valid_index(index) ) {
        return;
      }

      const Node &node = nodes[index];
      auto dist = dist_fn(items[node.index], target);

      if (dist < record) {
        if (queue.size() == k) {
          queue.pop();
        }
        queue.push({node.index, dist});

        // we can improve record now
        if (queue.size() == k) {
          record = queue.top().distance;
        }
      }

      // heurustic described by Peter N. Yianilos:
      // http://pnylab.com/pny/papers/vptree/vptree.pdf
      // crucial part of the algorithm, actually
      if (dist < node.bound) {
        if (dist - record <= node.bound) {
          search(node.left);
        }

        if (dist + record >= node.bound) {
          search(node.right);
        }
      } else {
        if (dist + record >= node.bound) {
          search(node.right);
        }

        if (dist - record <= node.bound) {
          search(node.left);
        }
      }

    }

    std::vector<std::pair<T, float>> build_results() {
      std::vector<std::pair<T, float>> results;
      while(!queue.empty()) {
        const auto &item = queue.top();
        results.push_back({items[item.index], item.distance});
        queue.pop();
      }

      std::reverse(results.begin(), results.end());
      return results;
    }

    bool is_valid_index(unsigned index) const {
      return index < items.size();
    }
  };
};

}  // namespace aligusnet

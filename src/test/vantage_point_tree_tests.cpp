#include <vantage_point_tree.h>
#include "catch.hpp"
#include "person_test_data.h"

namespace aligusnet {
    TEST_CASE("Vantage-Point Tree smoke test", "VPTree") {
        VantagePointTree<Person> vpTree{ get_person9_test_data(), person_distance };

        SECTION("Search 90-5-10 3-NN") {
            auto results = vpTree.search({ 0, 90, 5, 10 }, 3);
            REQUIRE(results.size() == 3);
            REQUIRE(results[0].first.name == 1);
            REQUIRE(results[1].first.name == 6);
            REQUIRE(results[2].first.name == 5);
        }
    }
}

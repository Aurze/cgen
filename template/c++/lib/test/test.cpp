#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../src/fibonacci.h"
#include <catch2/catch.hpp>

SCENARIO("Computing fibonacci sequence", "[fibonacci]") {
  GIVEN("0") {
    auto n = 0;
    WHEN("I get the fibonacci value") {
      auto x = fibonacci(n);
      THEN("I get 0") { REQUIRE(x == 0); }
    }
  }
  GIVEN("1") {
    auto n = 1;
    WHEN("I get the fibonacci value") {
      auto x = fibonacci(n);
      THEN("I get 1") { REQUIRE(x == 1); }
    }
  }
  GIVEN("45") {
    auto n = 45;
    WHEN("I get the fibonacci value") {
      auto x = fibonacci(n);
      THEN("I get 109441") { REQUIRE(x == 109441); }
    }
  }
}

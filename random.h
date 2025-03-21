#pragma once

#include <chrono>
#include <random>

namespace Random {
// Returns a seeded Mersenne Twister
// Note: we'd prefer to return a std::seed_seq (to initialize a std::mt19937),
// but std::seed can't be copied, so it can't be returned by value. Instead,
// we'll create a std::mt19937, seed it, and then return the std::mt19937 (which
// can be copied).
inline std::mt19937 generate() {
  std::random_device rd{};

  // Create seed_seq with clock and 7 random numbers from std::random_device
  std::seed_seq ss{
      static_cast<std::seed_seq::result_type>(
          std::chrono::steady_clock::now().time_since_epoch().count()),
      rd(),
      rd(),
      rd(),
      rd(),
      rd(),
      rd(),
      rd()};

  return std::mt19937{ss};
}

// Here's our global std::mt19937 object.
// The inline keyword means we only have one global instance for our whole
// program.
inline std::mt19937 mt{generate()}; // generates a seeded std::mt19937 and
                                    // copies it into our global object

// Generate a random double between [min, max] (inclusive)
// * also handles cases where the two arguments have different types but can be
// converted to double
inline double get(double min, double max) {
  return std::uniform_real_distribution{min, max}(mt);
}

// Generate a random int between [min, max] (inclusive)
// * also handles cases where the two arguments have different types but can be
// converted to int
inline int getInt(int min, int max) {
  return std::uniform_int_distribution{min, max}(mt);
}

// Generate a random double using normal distribution with given mean and normal
// diviation
inline double getNormal(double mean = 0.0, double normalDiv = 1.0) {
  return std::normal_distribution<double>{mean, normalDiv}(mt);
}
} // namespace Random

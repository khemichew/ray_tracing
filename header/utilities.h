#pragma once

#include <limits>
#include <random>

// Constants 
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility methods 
inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

/*
 * Returns a random real number in [0, 1).
 */
inline double random_double() {
  static std::random_device seed_device;
  static std::mt19937 generator(seed_device());
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  return distribution(generator);
}

/*
 * Restricts the value of x in range [min, max].
 */
inline double clamp(double x, double min, double max) {
  if (x < min) {
    return min;
  }
  if (x > max) {
    return max;
  }
  return x;
}

// Common headers 
#include "ray.h"
#include "vec3.h"
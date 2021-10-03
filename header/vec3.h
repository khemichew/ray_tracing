#pragma once

#include <iostream>

class vec3 {
private:
  double e[3];

public:
  vec3() : e{0, 0, 0} {}
  vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}
  // TODO: Initialise copy constructor

  double x() const { return e[0]; }
  double y() const { return e[1]; }
  double z() const { return e[2]; }

  vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

  double operator[](int i) const { return e[i]; }

  double &operator[](int i) { return e[i]; }

  vec3 &operator+=(const vec3 &other) {
    e[0] += other.e[0];
    e[1] += other.e[1];
    e[2] += other.e[2];
    return *this;
  }

  vec3 &operator*=(const double t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }

  vec3 &operator/=(const double t) { return *this *= 1 / t; }

  double length() const { return sqrt(length_squared()); }

  double length_squared() const {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
  }
};

// Type classes for vec3
using point3 = vec3; // 3D point
using Colour = vec3; // RGB colour

// Utility functions
inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
  return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
  return vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
  return vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
  return vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline vec3 operator*(double t, const vec3 &v) {
  return vec3(t * v.x(), t * v.y(), t * v.z());
}

inline vec3 operator*(const vec3 &v, double t) { return t * v; }

inline vec3 operator/(const vec3 &v, double t) { return (1 / t) * v; }

inline double dot(const vec3 &u, const vec3 &v) {
  return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
  return vec3(u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(),
              u.x() * v.y() - u.y() * v.x());
}

inline vec3 unit_vector(const vec3 &v) { return vec3(v) / v.length(); }

inline vec3 random_vector() {
  return vec3(random_double(), random_double(), random_double());
}

inline vec3 random_vector(double min, double max) {
  return vec3(random_double(min, max), random_double(min, max),
              random_double(min, max));
}

inline vec3 random_in_unit_sphere() {
  vec3 vector;
  do {
    vector = random_vector(-1, 1);
  } while (vector.length_squared() >= 1);
  return vector;
}

inline vec3 random_unit_vector() {
  return unit_vector(random_in_unit_sphere());
}

inline vec3 random_in_hemisphere(const vec3& normal) {
  vec3 in_unit_sphere = random_in_unit_sphere();

  // Uniform scatter direction for all angles away from the hit point
  if (dot(in_unit_sphere, normal) > 0.0) {
    return in_unit_sphere;
  } else {
    return -in_unit_sphere;
  }
}
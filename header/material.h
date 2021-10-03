#pragma once

#include "hittable.h"

class Material {
public:
  virtual bool scatter(const Ray &r_in, const hit_record &rec,
                       Colour &attenuation, Ray &scattered) const = 0;
};

class Lambertian : public Material {
private:
  Colour albedo;

public:
  explicit Lambertian(const Colour &colour) : albedo(colour){};

  bool scatter(const Ray &r_in, const hit_record &rec, Colour &attenuation,
               Ray &scattered) const override {
    auto scatter_direction = rec.normal + random_unit_vector();

    // Catch degenerate scatter direction (eg. when obtaining unit vector)
    if (scatter_direction.near_zero()) {
      scatter_direction = rec.normal;
    }

    scattered = Ray(rec.p, scatter_direction);
    attenuation = albedo;
    return true;
  }

  Colour get_albedo() { return albedo; }
};

class Metal : public Material {
private:
  Colour albedo;
  double fuzz;

public:
  Metal(const Colour &colour, double fuzziness)
      : albedo(colour), fuzz(fuzziness < 1 ? fuzziness : 1){};

  bool scatter(const Ray &r_in, const hit_record &rec, Colour &attenuation,
               Ray &scattered) const override {
    vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
    attenuation = albedo;
    return dot(scattered.direction(), rec.normal) > 0;
  }

  Colour get_albedo() { return albedo; }
};
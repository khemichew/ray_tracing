#include "header/camera.h"
#include "header/colour.h"
#include "header/hittables.h"
#include "header/material.h"
#include "header/sphere.h"
#include "header/utilities.h"
#include <cmath>
#include <iostream>

Colour ray_colour(const Ray &r);
double hit_sphere(const point3 &center, double radius, const Ray &r);

/* Ray tracer:
1) Calculate the ray from the eye to the pixel;
2) Determine which objects the ray intersects;
3) Compute a colour for the intersection point(s).
*/

// TODO: Stratification

Colour ray_colour(const Ray &r, const Hittables &world, int depth) {
  hit_record rec;

  // No more light reflected
  if (depth <= 0) {
    return Colour(0, 0, 0);
  }

  // Hittable objects absorb/reflect half the light energy on each bounce
  if (world.hit(r, 0.001, infinity, rec)) {
    Ray scattered;
    Colour attenuation;

    if (rec.material_ptr->scatter(r, rec, attenuation, scattered)) {
      return attenuation * ray_colour(scattered, world, depth - 1);
    }

    return Colour(0, 0, 0);
    //    point3 target = rec.p + random_in_hemisphere(rec.normal);
    //    return 0.5 * ray_colour(Ray(rec.p, target - rec.p), world, depth - 1);
  }

  vec3 unit_dir = unit_vector(r.direction());

  auto t = 0.5 * (unit_dir.y() + 1.0);
  return (1.0 - t) * Colour(1.0, 1.0, 1.0) + t * Colour(0.5, 0.7, 1.0);
}

/*
Equation of a sphere is (x - c_x)^2 + (y - c_y)^2 + (z - c_z)^2 = r^2

Ray is represented as P(t) = A + t * b, and C = {C_x, C_y, C_z}

Expanding the equation of the circle is as follows:
t^2 * (b dot b) + 2t * (b dot (A - C)) + ((A - C) dot (A - C)) - r^2 = 0

Solving this quadratic equation will tell us whether the point P(t) lies in
the sphere (which we can use the discriminant to verify)
*/
double hit_sphere(const point3 &center, double radius, const Ray &r) {
  vec3 oc = r.origin() - center;
  auto a = r.direction().length_squared();
  auto half_b = dot(oc, r.direction());
  auto c = oc.length_squared() - radius * radius;
  auto discriminant = half_b * half_b - a * c;

  if (discriminant < 0) {
    return -1.0;
  }
  return (-half_b - sqrt(discriminant)) / a;
}

int main() {
  // Image
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 100;
  const int max_depth = 50;

  // World
  Hittables world;
  // Materials
  auto material_ground = std::make_shared<Lambertian>(Colour(0.8, 0.8, 0.0));
  auto material_center = std::make_shared<Lambertian>(Colour(0.7, 0.3, 0.3));
  auto material_left = std::make_shared<Metal>(Colour(0.8, 0.8, 0.8));
  auto material_right = std::make_shared<Metal>(Colour(0.8, 0.6, 0.2));

  // Spheres
  world.add(std::make_shared<Sphere>(point3(0.0, -100.5, -1.0), 100.0,
                                     material_ground));
  world.add(
      std::make_shared<Sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
  world.add(
      std::make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
  world.add(
      std::make_shared<Sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

  // Camera
  Camera camera;

  // TODO: try to produce more image types than PPM
  // 	   eg. stb_image.h at https://github.com/nothings/stb

  // Render
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int j = image_height - 1; j >= 0; j--) {
    std::cerr << "\rScanlines remaining: " << j << '\n' << std::flush;

    for (int i = 0; i < image_width; i++) {
      Colour pixel = Colour(0, 0, 0);

      for (int s = 0; s < samples_per_pixel; s++) {
        auto u = (i + random_double()) / (image_width - 1);
        auto v = (j + random_double()) / (image_height - 1);
        Ray r = camera.get_ray(u, v);
        pixel += ray_colour(r, world, max_depth);
      }

      write_colour(std::cout, pixel, samples_per_pixel);
    }
  }

  std::cerr << "\nDone.\n";
}
#include "header/colour.h"
#include "header/hittables.h"
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

Colour ray_colour(const Ray &r, const Hittables &world) {
  hit_record rec;
  if (world.hit(r, 0, infinity, rec)) {
    return 0.5 * (rec.normal + Colour(1.0, 1.0, 1.0));
  }

  // point3 sphere_center = point3(0, 0, -1);
  // auto t = hit_sphere(sphere_center, 0.5, r);

  //// if ray hits sphere in positive direction
  // if (t > 0.0) {
  //	vec3 normal = unit_vector(r.at(t) - sphere_center);
  //	return 0.5 * Colour(normal.x() + 1, normal.y() + 1, normal.z() + 1);
  // }

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

  // World
  Hittables world;
  auto sphere1 = std::make_shared<Sphere>(point3(0, 0, -1), 0.5);
  auto sphere2 = std::make_shared<Sphere>(point3(0, -100.5, -1), 100);
  world.add(sphere1);
  world.add(sphere2);

  // Camera
  auto viewport_height = 2.0;
  auto viewport_width = aspect_ratio * viewport_height;
  auto focal_length = 1.0;

  auto origin = point3(0, 0, 0);
  auto horizontal = vec3(viewport_width, 0, 0);
  auto vertical = vec3(0, viewport_height, 0);
  auto lower_left_corner =
      origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

  // TODO: try to produce more image types than PPM
  // 	   eg. stb_image.h at https://github.com/nothings/stb

  // Render
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int j = image_height - 1; j >= 0; j--) {
    std::cerr << "\rScanlines remaining: " << j << '\n' << std::flush;

    for (int i = 0; i < image_width; i++) {
      auto u = static_cast<double>(i) / (image_width - 1);
      auto v = static_cast<double>(j) / (image_height - 1);
      Ray r = Ray(origin,
                  lower_left_corner + u * horizontal + v * vertical - origin);

      Colour pixel = ray_colour(r, world);
      write_colour(std::cout, pixel);
    }
  }

  std::cerr << "\nDone.\n";
}
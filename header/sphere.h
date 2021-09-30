#pragma once

#include "vec3.h"
#include "hittable.h"

class Sphere : public Hittable {
private:
	point3 center;
	double radius;

public:
	Sphere() {}
	Sphere(point3 center, double r) : center(center), radius(r) {};

	virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override {
		vec3 oc = r.origin() - center;
		auto a = r.direction().length_squared();
		auto half_b = dot(oc, r.direction());
		auto c = oc.length_squared() - radius * radius;

		auto discriminant = half_b * half_b - a * c;
		if (discriminant < 0) {
			return false;
		}

		auto sqrtd = sqrt(discriminant);

		// Find the nearest root that lies in the acceptable range.
		auto root = (-half_b - sqrtd) / a;
		if (root < t_min || root > t_max) {
			root = (-half_b + sqrtd) / a;
			if (root < t_min || root > t_max) {
				return false;
			}
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		vec3 outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(r, outward_normal);

		return true;
	}
};

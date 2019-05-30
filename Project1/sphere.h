#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere: public hitable {
    public: 
        sphere() {}
        sphere(vec3 cen, float r) : center(cen), radius(r) {};
        sphere(vec3 cen, float r, material *m) : center(cen), radius(r), mat_ptr(m) {};
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
        vec3 center;
        float radius;
        material *mat_ptr;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - a*c;

    // If roots exists; if we hit the sphere
    if (discriminant > 0) {
        
        //Find the normal
        float temp = (-b - sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);

            // Easy form for sphere normal
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }

        // Try the other root
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }

    return false;
}

// Chapter 4
bool hit_sphere(const vec3& center, float radius, const ray& r) {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discrim = b*b - 4*a*c;
    return (discrim > 0);
}

//Chapter 5
float hit_sphere2(const vec3& center, float radius, const ray& r) {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discrim = b*b - 4*a*c;
 
    if (discrim < 0 ) {
        return -1.0;
    } 
    else {
        return (-b - sqrt(discrim)) / (2.0 * a);
    }
}

#endif
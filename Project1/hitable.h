#ifndef HITABLEH
#define HITABLEH

#include "ray.h"

class material;

// Based off of parametric line. 
// vec3 normal is surface normal
// vec3 p is starting point. 
// float t is time elaspse

struct hit_record {
    float t;
    vec3 p;
    vec3 normal;
    material *mat_ptr;
};

class hitable {
    public:
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif
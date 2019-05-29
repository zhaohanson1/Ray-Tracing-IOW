#include <iostream>

#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"

inline float rand_unit() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
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

// Chapter 7
vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0 * vec3(rand_unit(), rand_unit(), rand_unit()) - vec3(1,1,1);
    } while (p.squared_length() >= 1.0);
    return p;
}

/*
vec3 color(const ray& r) {

    
    //Chapter 4: Sphere Check
    if (hit_sphere(vec3(0,0,-1), 0.99, r))
        return vec3(1,0,0);
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    

   // Chapter 5: Spheres
   float t = hit_sphere2(vec3(0,0,-1), 0.5, r);
   if (t > 0.0) {
       vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0,0,-1));
       return 0.5*vec3(N.x()+1, N.y()+1, N.z()+1);
   }
   vec3 unit_direction = unit_vector(r.direction());
   float t2 = 0.5*(unit_direction.y() + 1.0);
   return (1.0 - t2) * vec3(1.0, 1.0, 1.0) + t2*vec3(0.5, 0.7, 1.0);
}
*/

vec3 color(const ray& r, hitable *world) {
    hit_record rec;
    if (world->hit(r, 0.001, FLT_MAX, rec)) {
        // Ch 6
        //return 0.5*vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);
        //Ch 7
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5 * color( ray(rec.p, target - rec.p), world);
    } 
    else {
        vec3 unit_dir = unit_vector(r.direction());
        float t = 0.5*(unit_dir.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
}


int main() {
    int nx = 1280;
    int ny = 720;
    int ns = 1;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    //vec3 lower_left_corner(-2.0, -1.0, -1.0);
    //vec3 horizontal(4.0, 0.0, 0.0);
    //vec3 vertical(0.0, 2.0, 0.0);
    //vec3 origin(0.0, 0.0, 0.0);

    //Chapter 5.1
    hitable *list[2];
    list[0] = new sphere(vec3(0,0,-1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    hitable *world = new hitable_list(list,2);
    
    //Ch 6
    camera cam;

    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            /*
            // Ch 5
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical);

            vec3 p = r.point_at_parameter(2.0);
            vec3 col = color(r, world);
            */

            vec3 col(0, 0, 0);
            // Monte Carlo sample averaging for each pixel
            for (int s = 0; s < ns; s++) {
                float u = float(i + rand_unit()) / float(nx);
                float v = float(j + rand_unit()) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world);
           }
            col /= float(ns);
            // Gamma 2
            col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            std::cout << ir << " " << ig << " " << ib << "\n";

        }
    }
}
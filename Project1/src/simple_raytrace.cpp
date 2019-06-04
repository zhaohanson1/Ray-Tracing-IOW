#include <iostream>

#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"


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

//Chapter 8
vec3 color(const ray& r, hitable *world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001, FLT_MAX, rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation*color(scattered, world, depth+1);
        }
        else {
            return vec3(0,0,0);
        }
    } 
    else {
        vec3 unit_dir = unit_vector(r.direction());
        float t = 0.5*(unit_dir.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
}
hitable *random_scene() {
    int n = 500;
    hitable **list = new hitable*[n+1];
    list[0] =  new sphere(vec3(0,-1000,0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = rand_unit();
            vec3 center(a+0.9*rand_unit(),0.2,b+0.9*rand_unit()); 
            if ((center-vec3(4,0.2,0)).length() > 0.9) { 
                if (choose_mat < 0.8) {  // diffuse
                    list[i++] = new sphere(center, 0.2, new lambertian(vec3(rand_unit()*rand_unit(), rand_unit()*rand_unit(), rand_unit()*rand_unit())));
                }
                else if (choose_mat < 0.95) { // metal
                    list[i++] = new sphere(center, 0.2,
                            new metal(vec3(0.5*(1 + rand_unit()), 0.5*(1 + rand_unit()), 0.5*(1 + rand_unit())),  0.5*rand_unit()));
                }
                else {  // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hitable_list(list,i);
}

int main() {
    int nx = 1200;
    int ny = 800;
    int ns = 10;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    //vec3 lower_left_corner(-2.0, -1.0, -1.0);
    //vec3 horizontal(4.0, 0.0, 0.0);
    //vec3 vertical(0.0, 2.0, 0.0);
    //vec3 origin(0.0, 0.0, 0.0);

    /*
    //Chapter 5.1
    hitable *list[2];
    list[0] = new sphere(vec3(0,0,-1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    hitable *world = new hitable_list(list,2);
    */


    //Ch 8
    /*
    hitable *list[4];
    list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
    list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.2));
    //list[3] = new sphere(vec3(-1.5,0,-1.5), 0.5, new metal(vec3(0.8, 0.8, 0.8), 1.0));
    list[3] = new sphere(vec3(-1,0,-1), 0.5, new dielectric(1.5));
    //list[4] = new sphere(vec3(-1,0,-1), -0.45, new dielectric(1.5));
    hitable *world = new hitable_list(list,4);
    */
    hitable *world = random_scene();
  

 /*
    //Ch 10
    hitable *list[2];
    float R = cos(M_PI/4);
    list[0] = new sphere(vec3(-R,0,-1), R, new lambertian(vec3(0,0,1)));
    list[1] = new sphere(vec3(R,0,-1), R, new lambertian(vec3(1,0,0)));
    hitable *world = new hitable_list(list,2);
    */

    //Ch 6
    //camera cam;
    //camera cam(90, float(nx)/float(ny));
    //camera cam(vec3(-2, 2, 1), vec3(0,0,-1), vec3(0,1,0), 70, float(nx)/float(ny));

    //vec3 lookfrom(3,3,2);
    //vec3 lookat(0,0,-1);
    //float dist_to_focus = (lookfrom-lookat).length();
    //float aperture = 2.0;

    vec3 lookfrom(13,2,3);
    vec3 lookat(0,0,0);
    float dist_to_focus = 10.0;
    float aperture = 0.1;

    camera cam(lookfrom, lookat, vec3(0,1,0), 20, float(nx)/float(ny), aperture, dist_to_focus);

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
                col += color(r, world, 0);
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

    exit(0);
}
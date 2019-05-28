#include <iostream>
#include "vec3.h"

int main() {
    int nx = 200;
    int ny = 200;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 col(float(i) / float(nx), float(j) / float(ny), 0.2);
            float ir = int(255.99*col[0]);
            float ig = int(255*col[1]);
            float ib = int(255*col[2]);
            std::cout << ib << " " << ir << " " << ig << "\n";
        }
    }
}
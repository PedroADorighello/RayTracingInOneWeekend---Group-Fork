//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"


int main() {
    hittable_list world;

    // Chão de esferas de metal
    auto ground_material = make_shared<metal>(color(0.8, 0.8, 0.8), 0);
    for (int a = -10; a < 10; a++) {
        for (int b = -10; b < 10; b++) {
            point3 center(a, -0.5, b);
            world.add(make_shared<sphere>(center, 0.5, ground_material));
        }
    }

    // Materiais
    auto spiral1_material = make_shared<lambertian>(color(0.9, 0.1, 0.1));  // Vermelho difuso
    auto spiral2_material = make_shared<metal>(color(0.1, 0.9, 0.1), 0.0);  // Verde espelhado
    auto glass_material = make_shared<dielectric>(1.5);  // Vidro

    const int spiral_steps = 80;
    const double spiral_radius = 4.5;
    const double spiral_height = 10.0;
    const double spiral_turns = 3.0;

    // Espiral externa vermelha
    for (int i = 0; i < spiral_steps; i++) {
        double t = double(i) / spiral_steps;
        double angle = t * spiral_turns * 2 * pi;
        double x = spiral_radius * cos(angle);
        double z = spiral_radius * sin(angle);
        double y = t * spiral_height;
        
        world.add(make_shared<sphere>(point3(x, y, z), 0.5, spiral1_material));
    }

    // Espiral interna verde
    for (int i = 0; i < spiral_steps; i++) {
        double t = double(i) / spiral_steps;
        double angle = t * (spiral_turns + 0.3) * 2 * pi;
        double x = (spiral_radius * 0.6) * cos(angle);
        double z = (spiral_radius * 0.6) * sin(angle);
        double y = t * spiral_height + 0.3;
        
        world.add(make_shared<sphere>(point3(x, y, z), 0.35, spiral2_material));
    }

    // Pilar central de esferas de vidro
    for (int i = 0; i < 10; i++) {
        double y = i * 1.1;
        world.add(make_shared<sphere>(point3(0, y, 0), 1.2, glass_material));
    }

    camera cam;

    cam.aspect_ratio      = 16/9;
    cam.image_width       = 600;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 50;
    cam.lookfrom = point3(0, 5, 10);
    cam.lookat   = point3(0, spiral_height/2, 0);
    cam.vup      = vec3(0, 1, 0);

    cam.focus_dist    = (cam.lookfrom - cam.lookat).length();

    cam.render(world);
}
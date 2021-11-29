#include <iostream>
#include <fstream>

#include "color.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "hittable.hpp"
#include "sphere.hpp"
#include "hittable_list.hpp"
#include "rtweekend.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "pyramid.hpp"
#include "triangle.hpp"

// Função auxiliar para criar um fundo de imagem colorido.
color ray_color(const ray &r, const hittable &world, int depth)
{
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0, 0, 0);

    hit_record rec;
    if (world.hit(r, 0.001, infinity, rec))
    {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return color(0, 0, 0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
    // Image settings.
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 480;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // World settings and init.
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto mat_red = make_shared<lambertian>(color(1.0, 0.0, 0.0));
    auto mat_green = make_shared<lambertian>(color(0.0, 1.0, 0.0));
    auto mat_blue = make_shared<lambertian>(color(0.0, 0.0, 1.0));
    auto mat_metal = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);

    // world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    // world.add(make_shared<sphere>(point3(0.0, 0.0, 0.0), 0.5, mat_blue));
    // world.add(make_shared<sphere>(point3(-1.0, 0.0, 0.0), 0.5, mat_red));
    // world.add(make_shared<sphere>(point3( -1.0,    0.0, -1.0),  0.5, mat_metal));

    world.add(make_shared<pyramid>(
        point3(0.0, 0, 0.0), 1.0, 0.5,
        mat_green,
        mat_red,
        mat_blue,
        mat_red,
        mat_blue));

    world.add(make_shared<pyramid>(
        point3(1.0, 0, 0.0), 1.0, 0.5,
        mat_green,
        mat_red,
        mat_blue,
        mat_red,
        mat_blue));

    // Camera settings.
    const auto viewport_height = 2.0;
    const auto viewport_width = aspect_ratio * viewport_height;
    const auto focal_length = 1.0;

    // Camera position.
    point3 lookfrom(0, 2, 5);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = (lookfrom - lookat).length();
    auto aperture = 2.0;
    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    const auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

    // Create output image.
    std::ofstream image_file;
    image_file.open("image.ppm");
    image_file << "P3\n"
               << image_width << " " << image_height << "\n255\n";

    // Render image.
    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(image_file, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone\n";

    // Close image file.
    image_file.close();
}

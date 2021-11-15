#pragma once

#include "hittable.hpp"
#include "vec3.hpp"
#include "triangle.hpp"

class pyramid : public hittable {
    public:
        pyramid(point3 cen, 
                double h, 
                double r, 
                shared_ptr<material> mat0, 
                shared_ptr<material> mat1, 
                shared_ptr<material> mat2, 
                shared_ptr<material> mat3, 
                shared_ptr<material> mat4
            ) : 
                base_center(cen), 
                height(h), 
                base_radius(r), 
                mat_ptr(mat0),
                mat1(mat1),
                mat2(mat2),
                mat3(mat3),
                mat4(mat4) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 base_center;
        double height;
        double base_radius;
        shared_ptr<material> mat_ptr;
        shared_ptr<material> mat1;
        shared_ptr<material> mat2;
        shared_ptr<material> mat3;
        shared_ptr<material> mat4;
};

bool pyramid::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    struct s {
        vec3 &v0;
        vec3 &v1;
        vec3 &v2;
        vec3 &intersection_point;
    };


    vec3 base0 = base_center - vec3(base_radius, 0, 0);
    vec3 base1 = base_center - vec3(0, 0, base_radius);
    vec3 base2 = base_center + vec3(base_radius, 0, 0);
    vec3 base3 = base_center + vec3(0, 0, base_radius);
    vec3 top   = base_center + vec3(0, height, 0);

    bool hit0 = triangle(base2, base1, base0, mat_ptr).hit(r, t_min, t_max, rec);
    bool hit1 = triangle(base3, base2, base0, mat_ptr).hit(r, t_min, t_max, rec);
    bool hit2 = triangle(base0, base1, top, mat1).hit(r, t_min, t_max, rec);
    bool hit3 = triangle(base1, base2, top, mat2).hit(r, t_min, t_max, rec);
    bool hit4 = triangle(base2, base3, top, mat3).hit(r, t_min, t_max, rec);
    bool hit5 = triangle(base3, base0, top, mat4).hit(r, t_min, t_max, rec);

    if (hit0 || hit1 || hit2 || hit3 || hit4 || hit5 ) return true;
    else return false;
};

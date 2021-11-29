#pragma once

#include "hittable.hpp"
#include "vec3.hpp"

class triangle : public hittable
{
public:
    triangle(point3 pA, point3 pB, point3 pC, shared_ptr<material> mat) : pA(pA),
                                                                          pB(pB),
                                                                          pC(pC),
                                                                          mat_ptr(mat){};

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const;

public:
    point3 pA;
    point3 pB;
    point3 pC;
    shared_ptr<material> mat_ptr;
};

bool triangle::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
    vec3 v0v1 = pB - pA;
    vec3 v0v2 = pC - pA;
    vec3 pvec = cross(r.direction(), v0v2);
    float det = dot(v0v1, pvec);
    // // if the determinant is negative the triangle is backfacing
    // // if the determinant is close to 0, the ray misses the triangle
    if (det < t_min)
        return false;
    // // ray and triangle are parallel if det is close to 0
    if (fabs(det) < t_min)
        return false;

    float invDet = 1 / det;
    vec3 tvec = r.origin() - pA;
    float u = dot(tvec, pvec) * invDet;
    if (u < 0.0f || u > 1.0f)
        return false;

    vec3 qvec = cross(tvec, v0v1);
    float v = dot(r.direction(), qvec) * invDet;
    if (v < 0.0f || u + v > 1.0f)
        return false;

    float t = dot(v0v2, qvec) * invDet;

    rec.t = t;
    rec.p = vec3(u, v, t);
    rec.mat_ptr = mat_ptr;

    vec3 n0 = vec3(0, 0, 0);
    vec3 n1 = vec3(0, 0, 0);
    vec3 n2 = vec3(0, 0, 0);

    // Get the normal at the hit point by interpolating from the vertex normals.
    rec.normal = (1.0f - u - v) * n0 + u * n1 + v * n2;
    return true;
};

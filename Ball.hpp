#pragma once
#include "vector.hpp"

struct Ball {
    int id;
    Vector position;
    Vector velocity;
    double radius;
    double mass;
    double elasticity;

    Ball(int id, double x, double y, double r, double e)
        : id(id), position(x,y), velocity(0,0),
          radius(r), mass(r*r), elasticity(e) {}

    bool overlaps(const Ball& other) const {
        double dx = position.x - other.position.x;
        double dy = position.y - other.position.y;
        double rSum = radius + other.radius;
        return dx*dx + dy*dy <= rSum*rSum;
    }
};
#pragma once
#include "Vector.hpp"
#include "Constants.hpp"

struct Ball {
    inline static int nextID = 0;
    int id;
    Vector position;
    Vector velocity;
    const double radius;
    const double mass;

    Ball(double x, double y, double radius)
        : id(nextID++),
          position(x, y),
          velocity(0, 0),
          radius(radius),
          mass(radius * radius) {}

    bool overlaps(const Ball& other) const {
        double dx = position.x - other.position.x;
        double dy = position.y - other.position.y;
        double rSum = radius + other.radius;
        return dx * dx + dy * dy <= rSum * rSum;
    }
};
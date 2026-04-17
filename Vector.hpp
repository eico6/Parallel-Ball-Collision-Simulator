#pragma once
#include <cmath>

struct Vector {
    double x, y;

    Vector(double x=0, double y=0) : x(x), y(y) {}

    Vector add(double dx, double dy) const {
        return {x + dx, y + dy};
    }

    Vector add(const Vector& v) const {
        return {x + v.x, y + v.y};
    }

    Vector sub(const Vector& v) const {
        return {x - v.x, y - v.y};
    }

    Vector scale(double s) const {
        return {x * s, y * s};
    }

    Vector div(double d) const {
        return {x / d, y / d};
    }

    double dot(const Vector& v) const {
        return x*v.x + y*v.y;
    }

    bool isZero() const {
        return x == 0 && y == 0;
    }
};
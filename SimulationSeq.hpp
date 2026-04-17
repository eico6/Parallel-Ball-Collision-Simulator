#pragma once
#include "Ball.hpp"
#include "Grid.hpp"
#include "raylib.h"
#include <vector>
#include <cmath>

struct SimulationSeq {
    double gravity = 1000;
    Rectangle bounds;

    std::vector<Ball>& balls;
    Grid<std::vector<Ball>>& grid;

    SimulationSeq(Rectangle b,
                  std::vector<Ball>& balls,
                  Grid<std::vector<Ball>>& grid)
        : bounds(b), balls(balls), grid(grid) {}

    void update() {
        // move
        for (auto &b : balls) {
            b.velocity = b.velocity.add(0, gravity / 60.0);
            b.position = b.position.add(b.velocity.div(60.0));
        }

        // walls
        for (auto &b : balls) {
            resolveWall(b);
        }

        // collisions
        for (int i = 0; i < balls.size(); i++) {
            for (int j = i+1; j < balls.size(); j++) {
                auto &a = balls[i];
                auto &b = balls[j];

                if (a.overlaps(b)) {
                    resolveOverlap(a, b);
                    resolveCollision(a, b);
                }
            }
        }
    }

    void resolveWall(Ball &b) {
        if (b.position.x - b.radius < bounds.x) {
            b.position.x = bounds.x + b.radius;
            b.velocity.x *= -1;
        }

        if (b.position.x + b.radius > bounds.x + bounds.width) {
            b.position.x = bounds.x + bounds.width - b.radius;
            b.velocity.x *= -1;
        }

        if (b.position.y - b.radius < bounds.y) {
            b.position.y = bounds.y + b.radius;
            b.velocity.y *= -1;
        }

        if (b.position.y + b.radius > bounds.y + bounds.height) {
            b.position.y = bounds.y + bounds.height - b.radius;
            b.velocity.y *= -1;
        }
    }

    void resolveCollision(Ball &a, Ball &b) {
        Vector AB = a.position.sub(b.position);
        if (AB.isZero()) return;

        Vector vAB = a.velocity.sub(b.velocity);

        double scalar = 2.0/(a.mass + b.mass)
                        * b.mass
                        * vAB.dot(AB)
                        / AB.dot(AB);

        Vector impulse = AB.scale(scalar);

        a.velocity = a.velocity.sub(impulse);
        b.velocity = b.velocity.add(impulse);
    }

    void resolveOverlap(Ball &a, Ball &b) {
        Vector delta = a.position.sub(b.position);
        double distSq = delta.dot(delta);

        if (distSq == 0) return;

        double dist = std::sqrt(distSq);
        double overlap = a.radius + b.radius - dist;
        if (overlap <= 0) return;

        Vector normal = delta.div(dist);

        double totalMass = a.mass + b.mass;

        double moveA = overlap * b.mass / totalMass;
        double moveB = overlap * a.mass / totalMass;

        a.position = a.position.add(normal.scale(moveA));
        b.position = b.position.sub(normal.scale(moveB));
    }
};
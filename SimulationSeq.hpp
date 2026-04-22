#pragma once
#include "Ball.hpp"
#include "Grid.hpp"
#include "Simulation.hpp"
#include <vector>
#include <unordered_map>
#include <unordered_set>

class SimulationSeq : public Simulation {
public:
    const double gravity;
    const Rect bounds;
    std::vector<Ball>& balls;
    Grid<std::vector<Ball*>>& grid;

    SimulationSeq(double gravity, Rect bounds,
                  std::vector<Ball>& balls,
                  Grid<std::vector<Ball*>>& grid)
        : gravity(gravity), bounds(bounds), balls(balls), grid(grid)
    {
        for (Ball& b : balls)
            checkedPairs[&b] = {};
    }

    void update() {
        grid.clear();
        for (auto& kv : checkedPairs) kv.second.clear();

        // Apply gravity and integrate position
        for (Ball& b : balls) {
            b.velocity = b.velocity.add(0, gravity / FPS);
            b.position = b.position.add(Vector::div(b.velocity, FPS));
        }

        // Wall collisions
        for (Ball& b : balls)
            resolveWallCollision(b);

        // Insert into grid
        for (Ball& b : balls)
            addToGrid(b);

        // Grid-based O(n) collision detection
        On();
        // On2();
    }

    const Rect& getBounds() const override { return bounds; }
    std::vector<Ball>& getBalls() override { return balls; }
    const Grid<std::vector<Ball*>>& getGrid() const override { return grid; }

private:
    std::unordered_map<Ball*, std::unordered_set<Ball*>> checkedPairs;

    void On() {
        for (auto& kv : grid) {
            std::vector<Ball*>& cell = kv.second;
            for (int i = 0; i < (int)cell.size(); i++) {
                Ball* a = cell[i];
                for (int j = i + 1; j < (int)cell.size(); j++) {
                    Ball* b = cell[j];
                    // Skip already-checked pairs
                    if (checkedPairs[a].count(b))
                        continue;
                    if (a->overlaps(*b)) {
                        checkedPairs[a].insert(b);
                        checkedPairs[b].insert(a);
                        resolveOverlap(*a, *b);
                        resolveCollision(*a, *b);
                    }
                }
            }
        }
    }

    void On2() {
        for (int i = 0; i < (int)balls.size(); i++) {
            Ball& a = balls[i];
            for (int j = i + 1; j < (int)balls.size(); j++) {
                Ball& b = balls[j];
                if (a.overlaps(b)) {
                    resolveOverlap(a, b);
                    resolveCollision(a, b);
                }
            }
        }
    }

    void addToGrid(Ball& b) {
        Rect ballBounds = {
            b.position.x - b.radius,
            b.position.y - b.radius,
            2 * b.radius,
            2 * b.radius
        };
        for (const CellKey& cell : grid.getCells(ballBounds)) {
            if (grid.get(cell) == nullptr)
                grid.set(cell, {});
            grid.get(cell)->push_back(&b);
        }
    }

    void resolveWallCollision(Ball& b) {
        // Left wall
        if (b.position.x - b.radius < bounds.getX()) {
            b.position.setX(bounds.getX() + b.radius);
            b.velocity.setX(-b.velocity.getX());
        }
        // Right wall
        if (b.position.x + b.radius > bounds.getMaxX()) {
            b.position.setX(bounds.getMaxX() - b.radius);
            b.velocity.setX(-b.velocity.getX());
        }
        // Top wall (y min)
        if (b.position.y - b.radius < bounds.getY()) {
            b.position.setY(bounds.getY() + b.radius);
            b.velocity.setY(-b.velocity.getY());
        }
        // Bottom wall (y max)
        if (b.position.y + b.radius > bounds.getMaxY()) {
            b.position.setY(bounds.getMaxY() - b.radius);
            b.velocity.setY(-b.velocity.getY());
        }
    }

    void resolveCollision(Ball& a, Ball& b) {
        Vector AB = a.position.sub(b.position);
        if (AB.isZero()) return;

        Vector vAB = a.velocity.sub(b.velocity);

        double scalar = 2.0 / (a.mass + b.mass)
                        * b.mass
                        * vAB.dot(AB)
                        / AB.dot(AB);

        Vector impulse = AB.scale(scalar);

        a.velocity = a.velocity.sub(impulse);
        b.velocity = b.velocity.add(impulse);
    }

    void resolveOverlap(Ball& a, Ball& b) {
        Vector AB = a.position.sub(b.position);
        if (AB.isZero()) return;

        double dist = AB.len();
        double overlap = a.radius + b.radius - dist;
        if (overlap <= 0) return;

        Vector normal = AB.div(dist);
        normal = normal.scale(overlap / (a.mass + b.mass));

        a.position = a.position.add(normal.scale(b.mass));
        b.position = b.position.sub(normal.scale(a.mass));
    }
};

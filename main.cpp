#include "raylib.h"
#include "Ball.hpp"
#include "Grid.hpp"
#include "SimulationSeq.hpp"
#include "SimulationPar.hpp"
#include "View.hpp"

#include <vector>
#include <cstdlib>
#include <cmath>
#include <omp.h>

static const int ROWS = 12 * 2;
static const int COLS = 16 * 2;
static const int WIDTH  = 960;
static const int HEIGHT = 720;

static std::vector<Ball> spawnBalls(int n, double radius, const Rect& bounds) {
    std::vector<Ball> balls;
    balls.reserve(n);
    for (int i = 0; i < n; i++) {
        double x = bounds.getX() + (double)rand() / RAND_MAX * bounds.getWidth();
        double y = bounds.getY() + (double)rand() / RAND_MAX * bounds.getHeight();

        Ball b(x, y, radius, 1.0);
        b.velocity = {
            ((double)rand() / RAND_MAX - 0.5) * 1000,
            ((double)rand() / RAND_MAX - 0.5) * 1000
        };
        balls.push_back(b);
    }
    return balls;
}

/*
export OMP_NUM_THREADS=?
*/
int main() {
    double gravity = 0;

    Rect bounds = {0, 0, WIDTH, HEIGHT};

    
    int    n      = 1000;
    double radius = 1.0;
    printf("How many balls?\n");
    scanf("%d", &n);
    printf("What's the radius?\n");
    scanf("%lf", &radius);
    
    for (size_t i = 0; i < 100; i++)
    {
        printf("%d\n", omp_get_thread_num());
    }
    

    std::vector<Ball> balls = spawnBalls(n, radius, bounds);

    double cellWidth  = (double)WIDTH  / COLS;
    double cellHeight = (double)HEIGHT / ROWS;
    cellWidth = cellHeight = 4 * radius;

    Grid<std::vector<Ball*>> grid(cellWidth, cellHeight);
    for (const CellKey& cell : grid.getCells(bounds))
        grid.set(cell, {});

    SimulationPar simulation(gravity, bounds, balls, grid);
    View view(simulation);

    InitWindow(WIDTH, HEIGHT, "Ball Collision Simulator");
    SetTargetFPS(FPS);

    int   frames      = 0;
    int   fps         = 0;
    double lastFpsTime = GetTime();

    while (!WindowShouldClose()) {
        simulation.update();

        frames++;
        double now = GetTime();
        if (now - lastFpsTime >= 1.0) {
            fps        = frames;
            frames     = 0;
            lastFpsTime = now;
        }

        view.draw(fps);
    }

    CloseWindow();
    return 0;
}

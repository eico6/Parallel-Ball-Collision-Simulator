#pragma once
#include "raylib.h"
#include "Simulation.hpp"
#include "Constants.hpp"

static Color getColor(int id) {
    switch (id % 6) {
        case 0: return RED;
        case 1: return GREEN;
        case 2: return BLUE;
        case 3: return YELLOW;
        case 4: return ORANGE;
        case 5: return PURPLE;
    }
    return WHITE;
}

class View {
public:
    Simulation& sim;

    explicit View(Simulation& sim) : sim(sim) {}

    void draw(int fps) {
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw border rectangle
        Rect b = sim.getBounds();
        DrawRectangleLines((int)b.x, (int)b.y, (int)b.width, (int)b.height, WHITE);

        drawBalls();

        // Uncomment to draw grid:
        drawGrid();

        DrawText(TextFormat("Target FPS: %d", FPS), 10, 20, 20, WHITE);
        DrawText(TextFormat("FPS: %d", fps), 10, 40, 20, WHITE);
        DrawText(TextFormat("Balls: %d", (int)sim.getBalls().size()), 10, 60, 20, WHITE);

        EndDrawing();
    }

private:
    void drawBalls() {
        for (const Ball& ball : sim.getBalls()) {
            int x = (int)ball.position.x;
            int y = (int)ball.position.y;
            DrawCircle(x, y, (float)ball.radius, getColor(ball.id));
        }
    }

    void drawTrajectory() {
        for (const Ball& ball : sim.getBalls()) {
            int x = (int)ball.position.x;
            int y = (int)ball.position.y;
            DrawLine(x, y,
                     (int)(x + ball.velocity.x / 5),
                     (int)(y + ball.velocity.y / 5),
                     BLUE);
        }
    }

    void drawGrid() {
        for (const CellKey& cell : sim.getGrid().cells()) {
            double w = sim.getGrid().cellWidth;
            double h = sim.getGrid().cellHeight;
            int gx = (int)(cell.x * w);
            int gy = (int)(cell.y * h);
            DrawRectangleLines(gx, gy, (int)w, (int)h, GRAY);
        }
    }
};

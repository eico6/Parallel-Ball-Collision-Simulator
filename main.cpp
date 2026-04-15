#include <iostream>
#include <raylib.h>
#include "ball.h"

int main() {
    InitWindow(800, 600, "Bouncing Ball");

    Vector2 pos = {400, 300};
    Vector2 vel = {3, 2};
    float radius = 20;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // update
        pos.x += vel.x;
        pos.y += vel.y;

        if (pos.x < radius || pos.x > 800 - radius) vel.x *= -1;
        if (pos.y < radius || pos.y > 600 - radius) vel.y *= -1;

        // draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawCircleV(pos, radius, RED);
        EndDrawing();
    }

    CloseWindow();
}
#include "raylib.h"
#include "ParticleSystem.hpp"

int main() {
    const int screenWidth = 1600;
    const int screenHeight = 900;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "ray-bench | C++ Hardware & Graphics Benchmark");

    SetTargetFPS(60);

    ParticleSystem particleSystem(10000, screenWidth, screenHeight);
    RenderMode currentMode = RenderMode::Batched;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        int width = GetScreenWidth();
        int height = GetScreenHeight();

        if (IsKeyPressed(KEY_SPACE)) {
            currentMode = (currentMode == RenderMode::Naive) ? RenderMode::Batched : RenderMode::Naive;
        }

        particleSystem.Update(dt, width, height);

        BeginDrawing();
        ClearBackground(Color{ 8, 12, 20, 255 });

        particleSystem.Render(currentMode);

        DrawText("ray-bench | Particle Simulation Active", 20, 20, 18, Color{ 56, 189, 248, 255 });
        DrawText(TextFormat("Mode: %s (Press SPACE to toggle)", (currentMode == RenderMode::Naive) ? "Naive (Unbatched)" : "Batched (Vertex Stream)"), 20, 45, 14, RAYWHITE);
        DrawFPS(20, 70);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

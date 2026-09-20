#include "raylib.h"

int main() {
    const int screenWidth = 1600;
    const int screenHeight = 900;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "ray-bench | C++ Hardware & Graphics Benchmark");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Color{ 8, 12, 20, 255 });

        DrawText("ray-bench | Hardware Performance Benchmark", 40, 40, 24, Color{ 56, 189, 248, 255 });
        DrawText("Target Spec: NVIDIA GTX 1650 (4GB VRAM) | 8GB RAM", 40, 75, 16, Color{ 148, 163, 184, 255 });

        DrawText("Base window and rendering pipeline initialized.", 40, 130, 18, RAYWHITE);
        DrawText("Press ESC to exit.", 40, screenHeight - 60, 14, Color{ 100, 116, 139, 255 });

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

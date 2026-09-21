#include "raylib.h"
#include "ParticleSystem.hpp"
#include "BenchmarkHUD.hpp"

int main() {
    // 1600x900 default resolution, high DPI and resizable
    const int initialWidth = 1600;
    const int initialHeight = 900;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(initialWidth, initialHeight, "ray-bench | C++ Hardware & Graphics Performance Benchmark");

    // By default run uncapped to measure raw hardware throughput
    bool vsyncEnabled = false;
    SetTargetFPS(0);

    // Initial load: 10,000 particles
    ParticleSystem particleSystem(10000, initialWidth, initialHeight);
    BenchmarkHUD hud;

    // Start in Batched mode for optimal initial launch
    RenderMode currentMode = RenderMode::Batched;

    // Key repeat timer for UP / DOWN arrows
    float keyRepeatTimer = 0.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        // -------------------------------------------------------------
        // Input Handling
        // -------------------------------------------------------------
        // Toggle Rendering Mode
        if (IsKeyPressed(KEY_SPACE)) {
            currentMode = (currentMode == RenderMode::Naive) ? RenderMode::Batched : RenderMode::Naive;
        }

        // Particle Count Presets
        if (IsKeyPressed(KEY_ONE))   particleSystem.SetCount(1000);
        if (IsKeyPressed(KEY_TWO))   particleSystem.SetCount(10000);
        if (IsKeyPressed(KEY_THREE)) particleSystem.SetCount(25000);
        if (IsKeyPressed(KEY_FOUR))  particleSystem.SetCount(50000);
        if (IsKeyPressed(KEY_FIVE))  particleSystem.SetCount(100000);

        // Arrow Keys: Fine-grained increment / decrement
        if (IsKeyPressed(KEY_UP)) {
            particleSystem.AdjustCount(ParticleSystem::STEP_PARTICLES);
        } else if (IsKeyDown(KEY_UP)) {
            keyRepeatTimer += dt;
            if (keyRepeatTimer > 0.15f) {
                particleSystem.AdjustCount(ParticleSystem::STEP_PARTICLES);
                keyRepeatTimer = 0.10f;
            }
        }

        if (IsKeyPressed(KEY_DOWN)) {
            particleSystem.AdjustCount(-ParticleSystem::STEP_PARTICLES);
        } else if (IsKeyDown(KEY_DOWN)) {
            keyRepeatTimer += dt;
            if (keyRepeatTimer > 0.15f) {
                particleSystem.AdjustCount(-ParticleSystem::STEP_PARTICLES);
                keyRepeatTimer = 0.10f;
            }
        }

        if (IsKeyReleased(KEY_UP) && IsKeyReleased(KEY_DOWN)) {
            keyRepeatTimer = 0.0f;
        }

        // Reset particle simulation
        if (IsKeyPressed(KEY_R)) {
            particleSystem.Reset(screenWidth, screenHeight);
        }

        // Toggle HUD display
        if (IsKeyPressed(KEY_H)) {
            hud.ToggleVisibility();
        }

        // Toggle V-Sync / Framerate cap
        if (IsKeyPressed(KEY_V)) {
            vsyncEnabled = !vsyncEnabled;
            SetTargetFPS(vsyncEnabled ? 60 : 0);
        }

        // Fullscreen toggle
        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
        }

        // -------------------------------------------------------------
        // Simulation Update
        // -------------------------------------------------------------
        particleSystem.Update(dt, screenWidth, screenHeight);
        hud.Update(dt);

        // -------------------------------------------------------------
        // Render Pass
        // -------------------------------------------------------------
        BeginDrawing();
        ClearBackground(Color{ 8, 12, 20, 255 }); // Deep slate background

        // Render Particle System
        particleSystem.Render(currentMode);

        // Render Benchmark HUD Overlay
        hud.Render(
            currentMode,
            particleSystem.GetActiveCount(),
            particleSystem.GetMaxCapacity(),
            particleSystem.GetLastDrawCalls(),
            vsyncEnabled
        );

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

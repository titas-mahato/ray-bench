#include "BenchmarkHUD.hpp"
#include <algorithm>
#include <vector>
#include <cstdio>

BenchmarkHUD::BenchmarkHUD()
    : m_visible(true),
      m_avgFps(60.0f),
      m_onePercentLowFps(60.0f),
      m_currentFrameTimeMs(16.67f)
{
}

void BenchmarkHUD::Update(float dt) {
    if (dt <= 0.0f) dt = 0.0001f;
    m_currentFrameTimeMs = dt * 1000.0f;

    m_frameTimes.push_back(dt);
    if (m_frameTimes.size() > SAMPLE_WINDOW) {
        m_frameTimes.pop_front();
    }

    if (!m_frameTimes.empty()) {
        float totalTime = 0.0f;
        std::vector<float> sortedTimes(m_frameTimes.begin(), m_frameTimes.end());
        std::sort(sortedTimes.begin(), sortedTimes.end());

        for (float t : m_frameTimes) {
            totalTime += t;
        }

        m_avgFps = static_cast<float>(m_frameTimes.size()) / totalTime;

        // 99th percentile frame time (corresponds to 1% lowest framerate)
        size_t index99 = static_cast<size_t>(sortedTimes.size() * 0.99f);
        if (index99 >= sortedTimes.size()) index99 = sortedTimes.size() - 1;
        float frameTime99 = sortedTimes[index99];
        m_onePercentLowFps = (frameTime99 > 0.0f) ? (1.0f / frameTime99) : m_avgFps;
    }
}

void BenchmarkHUD::Render(RenderMode currentMode, int particleCount, int maxCapacity, int drawCalls, bool vsyncEnabled) {
    if (!m_visible) {
        // Draw minimal help prompt when HUD is collapsed
        DrawRectangle(10, 10, 160, 26, Color{ 15, 23, 42, 200 });
        DrawText("[H] Show Benchmark HUD", 16, 17, 12, RAYWHITE);
        return;
    }

    const int panelX = 20;
    const int panelY = 20;
    const int panelWidth = 470;
    const int panelHeight = 350;

    // Panel background & sleek border
    DrawRectangleRounded({ static_cast<float>(panelX), static_cast<float>(panelY), static_cast<float>(panelWidth), static_cast<float>(panelHeight) }, 0.04f, 4, Color{ 15, 23, 42, 225 });
    DrawRectangleRoundedLines({ static_cast<float>(panelX), static_cast<float>(panelY), static_cast<float>(panelWidth), static_cast<float>(panelHeight) }, 0.04f, 4, 1.5f, Color{ 51, 65, 85, 255 });

    int cursorY = panelY + 16;
    const int leftMargin = panelX + 18;

    // Benchmark title header
    DrawText("RAY-BENCH | HARDWARE PERFORMANCE MONITOR", leftMargin, cursorY, 14, Color{ 56, 189, 248, 255 });
    cursorY += 18;
    DrawText("Target Spec: NVIDIA RTX 3050 (6GB VRAM) | 16GB RAM", leftMargin, cursorY, 12, Color{ 148, 163, 184, 255 });
    cursorY += 22;

    // Divider
    DrawLine(leftMargin, cursorY, panelX + panelWidth - 18, cursorY, Color{ 51, 65, 85, 200 });
    cursorY += 12;

    // Mode Banner
    Rectangle modeBox = { static_cast<float>(leftMargin), static_cast<float>(cursorY), static_cast<float>(panelWidth - 36), 46.0f };
    if (currentMode == RenderMode::Naive) {
        DrawRectangleRounded(modeBox, 0.15f, 4, Color{ 127, 29, 29, 220 });
        DrawRectangleRoundedLines(modeBox, 0.15f, 4, 1.0f, Color{ 239, 68, 68, 255 });
        DrawText("[MODE A: NAIVE RENDERING]", leftMargin + 10, cursorY + 6, 15, Color{ 254, 202, 202, 255 });
        DrawText("1 Draw Call / Particle -> CPU Driver Stalled (Bottleneck)", leftMargin + 10, cursorY + 26, 11, Color{ 252, 165, 165, 255 });
    } else {
        DrawRectangleRounded(modeBox, 0.15f, 4, Color{ 20, 83, 45, 220 });
        DrawRectangleRoundedLines(modeBox, 0.15f, 4, 1.0f, Color{ 34, 197, 94, 255 });
        DrawText("[MODE B: BATCHED VERTEX STREAMING]", leftMargin + 10, cursorY + 6, 15, Color{ 187, 247, 208, 255 });
        DrawText("Dynamic Vertex Batching -> GPU Raster Saturation (Optimized)", leftMargin + 10, cursorY + 26, 11, Color{ 134, 239, 172, 255 });
    }
    cursorY += 56;

    // FPS & Latency Color Indicator
    Color perfColor;
    if (m_avgFps >= 55.0f) {
        perfColor = Color{ 34, 197, 94, 255 };   // Green
    } else if (m_avgFps >= 30.0f) {
        perfColor = Color{ 234, 179, 8, 255 };   // Yellow
    } else {
        perfColor = Color{ 239, 68, 68, 255 };   // Red
    }

    char buffer[128];

    // Real-time FPS & 1% Lows
    std::snprintf(buffer, sizeof(buffer), "Framerate:  %.1f FPS  (1%% Low: %.1f FPS)", m_avgFps, m_onePercentLowFps);
    DrawText(buffer, leftMargin, cursorY, 15, perfColor);
    cursorY += 22;

    // Frame Time Latency
    std::snprintf(buffer, sizeof(buffer), "Frame Time: %.2f ms  [Target: <16.6ms for 60 FPS]", m_currentFrameTimeMs);
    DrawText(buffer, leftMargin, cursorY, 13, (m_currentFrameTimeMs <= 16.67f) ? Color{ 226, 232, 240, 255 } : Color{ 251, 146, 60, 255 });
    cursorY += 20;

    // Entity Count
    std::snprintf(buffer, sizeof(buffer), "Particles:  %d / %d", particleCount, maxCapacity);
    DrawText(buffer, leftMargin, cursorY, 13, Color{ 226, 232, 240, 255 });
    cursorY += 20;

    // Draw Calls
    std::snprintf(buffer, sizeof(buffer), "Draw Calls: ~%d calls / frame", drawCalls);
    Color drawCallColor = (drawCalls > 1000) ? Color{ 248, 113, 113, 255 } : Color{ 74, 222, 128, 255 };
    DrawText(buffer, leftMargin, cursorY, 13, drawCallColor);
    cursorY += 20;

    // V-Sync State
    std::snprintf(buffer, sizeof(buffer), "Sync Mode:  %s", vsyncEnabled ? "V-Sync Enabled (60 FPS Cap)" : "Uncapped (Maximum Hardware Load)");
    DrawText(buffer, leftMargin, cursorY, 12, Color{ 148, 163, 184, 255 });
    cursorY += 24;

    // Divider
    DrawLine(leftMargin, cursorY, panelX + panelWidth - 18, cursorY, Color{ 51, 65, 85, 200 });
    cursorY += 10;

    // Controls Legend
    DrawText("HOTKEYS:", leftMargin, cursorY, 11, Color{ 94, 234, 212, 255 });
    cursorY += 16;
    DrawText("[SPACE] Toggle Mode A/B   [UP/DOWN] +/-25k Particles", leftMargin, cursorY, 11, Color{ 203, 213, 225, 255 });
    cursorY += 15;
    DrawText("[1-5] Presets (10k, 50k, 100k, 250k, 500k)   [V] V-Sync   [R] Reset", leftMargin, cursorY, 11, Color{ 203, 213, 225, 255 });
    cursorY += 15;
    DrawText("[H] Hide/Show HUD         [F11] Toggle Fullscreen", leftMargin, cursorY, 11, Color{ 148, 163, 184, 255 });
}

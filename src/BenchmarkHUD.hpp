#pragma once

#include "raylib.h"
#include "ParticleSystem.hpp"
#include <deque>

class BenchmarkHUD {
public:
    BenchmarkHUD();
    ~BenchmarkHUD() = default;

    void Update(float dt);
    void Render(RenderMode currentMode, int particleCount, int maxCapacity, int drawCalls, bool vsyncEnabled);

    void ToggleVisibility() { m_visible = !m_visible; }
    bool IsVisible() const { return m_visible; }

private:
    bool m_visible;
    std::deque<float> m_frameTimes;
    static constexpr size_t SAMPLE_WINDOW = 120;

    float m_avgFps;
    float m_onePercentLowFps;
    float m_currentFrameTimeMs;
};

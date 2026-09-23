#pragma once

#include "raylib.h"
#include <vector>
#include <cstdint>

enum class RenderMode {
    Naive,      // Mode A: Individual draw call per particle (Forced batch buffer flush)
    Batched     // Mode B: Streamed vertex batching (Single continuous quad pipeline)
};

struct Particle {
    float x, y;
    float vx, vy;
    float hue;
    Color color;
};

class ParticleSystem {
public:
    static constexpr int MIN_PARTICLES = 10000;
    static constexpr int MAX_PARTICLES = 500000;
    static constexpr int STEP_PARTICLES = 25000;

    ParticleSystem(int initialCount, int screenWidth, int screenHeight);
    ~ParticleSystem() = default;

    void Update(float dt, int screenWidth, int screenHeight);
    void Render(RenderMode mode);

    void SetCount(int count);
    void AdjustCount(int delta);
    void Reset(int screenWidth, int screenHeight);

    int GetActiveCount() const { return m_activeCount; }
    int GetMaxCapacity() const { return MAX_PARTICLES; }
    int GetLastDrawCalls() const { return m_lastDrawCalls; }
    float GetParticleSize() const { return m_particleSize; }

private:
    void InitParticle(size_t index, int screenWidth, int screenHeight);

    int m_activeCount;
    std::vector<Particle> m_particles;
    int m_lastDrawCalls;
    float m_particleSize;
};

#include "ParticleSystem.hpp"
#include "rlgl.h"
#include <algorithm>
#include <cstdlib>

ParticleSystem::ParticleSystem(int initialCount, int screenWidth, int screenHeight)
    : m_activeCount(std::clamp(initialCount, MIN_PARTICLES, MAX_PARTICLES)),
      m_lastDrawCalls(0),
      m_particleSize(3.0f)
{
    m_particles.resize(MAX_PARTICLES);
    for (size_t i = 0; i < MAX_PARTICLES; ++i) {
        InitParticle(i, screenWidth, screenHeight);
    }
}

void ParticleSystem::InitParticle(size_t index, int screenWidth, int screenHeight) {
    auto& p = m_particles[index];
    p.x = static_cast<float>(GetRandomValue(0, std::max(1, screenWidth - 10)));
    p.y = static_cast<float>(GetRandomValue(0, std::max(1, screenHeight - 10)));
    
    // Non-zero random velocities
    float vx = static_cast<float>(GetRandomValue(-150, 150));
    float vy = static_cast<float>(GetRandomValue(-150, 150));
    if (std::abs(vx) < 30.0f) vx = (vx < 0 ? -60.0f : 60.0f);
    if (std::abs(vy) < 30.0f) vy = (vy < 0 ? -60.0f : 60.0f);
    p.vx = vx;
    p.vy = vy;

    p.hue = static_cast<float>(GetRandomValue(0, 360));
    p.color = ColorFromHSV(p.hue, 0.85f, 1.0f);
}

void ParticleSystem::Update(float dt, int screenWidth, int screenHeight) {
    const float maxX = static_cast<float>(screenWidth) - m_particleSize;
    const float maxY = static_cast<float>(screenHeight) - m_particleSize;

    for (int i = 0; i < m_activeCount; ++i) {
        auto& p = m_particles[i];

        // Integrate position
        p.x += p.vx * dt;
        p.y += p.vy * dt;

        // Boundary collision reflection
        if (p.x <= 0.0f) {
            p.x = 0.0f;
            p.vx = -p.vx;
        } else if (p.x >= maxX) {
            p.x = maxX;
            p.vx = -p.vx;
        }

        if (p.y <= 0.0f) {
            p.y = 0.0f;
            p.vy = -p.vy;
        } else if (p.y >= maxY) {
            p.y = maxY;
            p.vy = -p.vy;
        }

        // Color cycling
        p.hue += 45.0f * dt;
        if (p.hue >= 360.0f) {
            p.hue -= 360.0f;
        }
        p.color = ColorFromHSV(p.hue, 0.85f, 1.0f);
    }
}

void ParticleSystem::Render(RenderMode mode) {
    if (mode == RenderMode::Naive) {
        // Mode A: Individual draw calls.
        // Forcing a render batch flush after every single particle forces a distinct
        // draw call (glDrawArrays / glDrawElements) and driver state validation round-trip.
        m_lastDrawCalls = 0;
        for (int i = 0; i < m_activeCount; ++i) {
            const auto& p = m_particles[i];
            DrawRectangleRec({ p.x, p.y, m_particleSize, m_particleSize }, p.color);
            rlDrawRenderBatchActive();
            m_lastDrawCalls++;
        }
    } else {
        // Mode B: Batched rendering via rlgl quad stream.
        // Contiguously buffers quad vertices directly into the GPU pipeline.
        // rlgl buffers up to 8,192 vertices before flushing, reducing 100,000 entities
        // down to ~49 draw calls total.
        m_lastDrawCalls = 0;
        rlBegin(RL_QUADS);
        for (int i = 0; i < m_activeCount; ++i) {
            const auto& p = m_particles[i];
            rlColor4ub(p.color.r, p.color.g, p.color.b, p.color.a);
            rlVertex2f(p.x, p.y);
            rlVertex2f(p.x, p.y + m_particleSize);
            rlVertex2f(p.x + m_particleSize, p.y + m_particleSize);
            rlVertex2f(p.x + m_particleSize, p.y);
        }
        rlEnd();
        m_lastDrawCalls = (m_activeCount * 4) / 8192 + 1;
    }
}

void ParticleSystem::SetCount(int count) {
    m_activeCount = std::clamp(count, MIN_PARTICLES, MAX_PARTICLES);
}

void ParticleSystem::AdjustCount(int delta) {
    SetCount(m_activeCount + delta);
}

void ParticleSystem::Reset(int screenWidth, int screenHeight) {
    for (size_t i = 0; i < MAX_PARTICLES; ++i) {
        InitParticle(i, screenWidth, screenHeight);
    }
}

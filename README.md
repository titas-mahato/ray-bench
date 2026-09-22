# ray-bench

> **Lightweight C++ GPU & Draw-Call Stress Benchmark for Budget Hardware (Raylib)**

`ray-bench` is an open-source, minimalist C++ graphics and hardware stress benchmark tailored for low-spec and budget gaming hardware—specifically calibrated for an **NVIDIA GeForce GTX 1650 (4GB VRAM)** and **8GB System RAM**.

Rather than testing complex geometry or compute shaders, `ray-bench` isolates the primary architectural bottleneck on budget hardware: **CPU Draw-Call Driver Dispatch vs. GPU Vertex Batching**. It simulates and renders up to **100,000 interactive 2D entities** in real-time, allowing instant switching between unbatched (Naive) and batched (Optimized) pipelines.

---

## ⚡ Quickstart (Build & Run in 2 Steps)

```powershell
# 1. Clone repository
git clone https://github.com/titas-mahato/ray-bench.git && cd ray-bench

# 2. Build and launch (Automatic Raylib fetch via CMake)
powershell -ExecutionPolicy Bypass -File .\build.ps1 -Run
```

*Alternatively, using standard CMake:*
```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
./build/bin/Release/ray-bench.exe
```

---

## 🎮 Benchmark Controls & Hotkeys

| Key | Action | Technical Impact |
| :--- | :--- | :--- |
| **`[SPACE]`** | **Toggle Rendering Mode** | Switches instantly between Mode A (Naive) and Mode B (Batched). |
| **`[1]` – `[5]`** | **Entity Count Presets** | `[1]` = 1,000 &bull; `[2]` = 10,000 &bull; `[3]` = 25,000 &bull; `[4]` = 50,000 &bull; `[5]` = 100,000 |
| **`[UP]` / `[DOWN]`** | **Fine Scale Entities** | Increases or decreases particle count by &plusmn;5,000 in real time. |
| **`[V]`** | **Toggle V-Sync / Framerate** | Alternates between V-Sync (60 FPS target cap) and Uncapped (raw hardware limit). |
| **`[R]`** | **Reset Simulation** | Re-randomizes all particle positions, velocities, and color vectors. |
| **`[H]`** | **Toggle HUD Overlay** | Hides or displays the performance metrics telemetry overlay. |
| **`[F11]`** | **Toggle Fullscreen** | Cycles borderless window / exclusive fullscreen mode. |

---

## 🛠 Project Architecture

```
ray-bench/
├── CMakeLists.txt              # Standard CMake build definition with Raylib FetchContent
├── build.ps1                   # One-click Windows PowerShell build & run utility
├── .gitignore                  # Git exclusions for build artifacts and IDE metadata
├── src/
│   ├── main.cpp                # Application entry point, main benchmark loop & inputs
│   ├── ParticleSystem.hpp      # Particle simulation header (Structs & RenderMode enums)
│   ├── ParticleSystem.cpp      # Implementation of Naive (flush) vs Batched (quad stream)
│   ├── BenchmarkHUD.hpp        # HUD overlay header (Metrics tracking & 1% low calculations)
│   └── BenchmarkHUD.cpp        # Real-time telemetry rendering (FPS, frame time, draw calls)
└── README.md                   # Technical documentation & low-spec optimization guide
```

---

## 📄 License
This project is open-source under the [MIT License](LICENSE). Built using [Raylib](https://www.raylib.com/).

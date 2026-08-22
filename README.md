# GNSS / RTK C++ Devcontainer

This repository is a portable starting point for a C++ GNSS/RTK codebase in VS Code. It gives you:

- a VS Code devcontainer based on Ubuntu 24.04
- a C++23 CMake + Ninja build with presets
- `GTSAM` installed in the container
- a small `gnss_rtk` library target you can expand into your own observation, navigation, and estimation stack

## Open In VS Code

1. Install Docker.
2. Open the folder in VS Code.
3. Run `Dev Containers: Reopen in Container`.

The container config lives in [`.devcontainer/devcontainer.json`](/C:/Users/andia/Documents/Codex/2026-08-22/ple/.devcontainer/devcontainer.json) and the image definition is in [`.devcontainer/Dockerfile`](/C:/Users/andia/Documents/Codex/2026-08-22/ple/.devcontainer/Dockerfile).

## Build

Inside the container:

```bash
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

## Layout

- [`CMakeLists.txt`](/C:/Users/andia/Documents/Codex/2026-08-22/ple/CMakeLists.txt): root build definition
- [`CMakePresets.json`](/C:/Users/andia/Documents/Codex/2026-08-22/ple/CMakePresets.json): reusable debug and release presets
- [`include/gnss_rtk/wgs84.hpp`](/C:/Users/andia/Documents/Codex/2026-08-22/ple/include/gnss_rtk/wgs84.hpp): starter public API
- [`src/wgs84.cpp`](/C:/Users/andia/Documents/Codex/2026-08-22/ple/src/wgs84.cpp): WGS84 conversions and GTSAM bridge
- [`examples/wgs84_demo.cpp`](/C:/Users/andia/Documents/Codex/2026-08-22/ple/examples/wgs84_demo.cpp): simple executable
- [`tests/wgs84_test.cpp`](/C:/Users/andia/Documents/Codex/2026-08-22/ple/tests/wgs84_test.cpp): round-trip sanity test

## Suggested Next Modules

As you grow this into a real GNSS/RTK library, the next useful directories are usually:

- `include/gnss_rtk/rinex/` and `src/rinex/` for observation and navigation parsing
- `include/gnss_rtk/time/` and `src/time/` for GPS, Galileo, and UTC time systems
- `include/gnss_rtk/models/` and `src/models/` for troposphere, ionosphere, clock, and measurement models
- `include/gnss_rtk/solver/` and `src/solver/` for least-squares, EKF, factor graph, and ambiguity handling
- `examples/` for repeatable experiments on pseudorange, carrier phase, PPP, and RTK

If you later want stricter dependency pinning across Linux and Windows, the next step would be switching the `GTSAM` dependency from Ubuntu packages to a project-local package manager such as `vcpkg`.

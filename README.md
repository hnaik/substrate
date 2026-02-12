# Substrate Execution Platform

![LGPL logo](https://www.gnu.org/graphics/lgplv3-147x51.png)

Substrate is an open-source execution platform for building and testing bespoke OMS/EMS systems. It combines a high‑performance C++ matching engine with a growing Python stack for simulation, replay, and orchestration.

## What’s in this repo

- C++ matching engine and limit order book
- Protocol definitions and message helpers
- Test suite for core matching/LOB behavior
- Data samples in extdata/ for replay/simulation workflows
- Python workspace (in progress) for fill simulation and replay

## Core goals

- Deterministic, testable execution simulation
- Modular components for OMS/EMS assembly
- Clear interfaces between market data, order flow, and risk controls

## Build (C++)

This project uses CMake and Ninja.

1. Configure:
   - mkdir -p build/gcc-debug
   - cmake -S . -B build/gcc-debug -G Ninja
2. Build:
   - cmake --build build/gcc-debug
3. Run tests:
   - ctest --test-dir build/gcc-debug

## Python (planned components)

The Python stack will add:

- Fill simulator
- Replay engine
- DAG builder for workflow assembly
- Algo/broker wheel
- Risk module
- Monitoring hooks

## Data

Data inputs live in extdata/ and include Databento DBN zst files plus metadata/manifest files.

## License

LGPL v3.0 or later. See COPYING and COPYING.LESSER for details.

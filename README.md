# Custom C++ Vector (C++20)

A from-scratch implementation of a `std::vector`-like container written in modern C++ (C++20).

This project was built as a learning exercise to deeply understand:
- dynamic memory management
- object lifetimes
- move semantics
- exception safety
- container invariants
- how `std::vector` works under the hood

The implementation is template-based and header-only, with correctness verified using GoogleTest.

---

## Features

- Dynamic resizing with a growth strategy
- Manual memory management (`operator new`, `std::construct_at`, `std::destroy_at`)
- Rule of 5 (copy/move constructors and assignments)
- `push_back` and `emplace_back` (perfect forwarding)
- `reserve`, `resize`, `clear`, `shrink_to_fit`
- Bounds-checked access via `at()`
- Iterators (`begin`, `end`)
- `initializer_list` and fill constructors
- Exception-safe reallocation paths
- Unit-tested with GoogleTest

---

## Project Structure

```
.
├── include/
│   └── dev_vector.hpp      # vector implementation (header-only)
├── tests/
│   └── vector_tests.cpp    # GoogleTest unit tests
├── CMakeLists.txt
└── README.md
```

---

## Build & Run Tests

### Requirements
- C++20 compatible compiler
- CMake
- GoogleTest (via Homebrew or system install)

### Build

```
mkdir build
cd build
cmake ..
cmake --build .
```

### Run tests

```
./vector_tests
```

---

## Motivation

This project is intentionally not meant to replace `std::vector`.

The goal is to:
- internalize container invariants
- understand allocation vs construction
- reason about exception safety and move semantics
- gain confidence reading and writing low-level C++ code

Documentation and deeper explanations will be expanded over time.

---

## Notes

- The implementation follows standard container semantics where applicable.
- Performance optimizations are secondary to correctness and clarity.
- This project favors explicitness over cleverness.

---

## License

MIT (or add later)

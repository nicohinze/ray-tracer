# ray-tracer

[![Pipeline status](https://github.com/nicohinze/ray-tracer/actions/workflows/ci.yml/badge.svg)](https://github.com/nicohinze/ray-tracer/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://raw.githubusercontent.com/nicohinze/ray-tracer/refs/heads/master/LICENSE)

A simple CPU-based ray tracer in C++. It is mostly a toy project to learn about ray tracing.

This project is heavily inspired by:

- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
- [Ray Tracing: The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
- [Understandable RayTracing in 256 lines of bare C++](https://github.com/ssloy/tinyraytracer)

## Build

```bash
cmake -S . -B build -G Ninja
cmake --build build
```

## Run

![Usage](docs/usage.gif)

## Example images

![Simple scene](examples/simple.jpg)
![Complex scene](examples/complex.jpg)
![Two big spheres](examples/two_spheres.jpg)
![Earth texture](examples/earth.jpg)
![Noise texture](examples/noise.jpg)
![Quads](examples/quads.jpg)
![Light scene](examples/light.jpg)
![Empty Cornell box](examples/cornell_box_empty.jpg)
![Standard Cornell box](examples/cornell_box_standard.jpg)
![Cornell box with smoke](examples/cornell_box_with_smoke.jpg)
![Final scene](examples/final.jpg)

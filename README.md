# Parallel Algorithms Lecture - Parallel Knapsack Solver

Framework for implementing the parallel Knapsack algorithms for the lecture's implementation challenge

## Setup

`git submodule update --init` to download the test instances locally

## Building

To build the project run the following commands:

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

and then run with `./build/ParAlgFramework test_instances.txt`.

# Individual Route Planning Tool

Command-line route planner (C++20, CMake) for driving and driving + walking trips over a real road network, built for **DA** (Desenho de Algoritmos) @ FEUP, 2nd year. Group project.

## About

The tool loads an urban map as a weighted graph (locations as nodes, roads as edges with separate **driving** and **walking** times) and uses **Dijkstra's algorithm** (with a mutable priority queue) to answer routing requests:

- **Independent driving route** – the fastest route between two points plus an **alternative route** that shares no intermediate nodes with the first.
- **Restricted driving route** – the fastest route that **avoids given nodes and/or road segments**, and can be forced to **pass through a specific node**.
- **Driving + walking route** – drive to a **parking node**, then walk to the destination, minimising total time under a **maximum walking time**. Ties are broken by preferring more walking.
  - If no valid route exists, the tool says why (walking limit too low, restrictions too tight, or both) and offers **approximate alternatives**: the two best routes without the walking limit, and optionally without any restrictions.

Results are printed to the terminal and written to `output.txt`.

## Project structure

```
.
├── main.cpp                    # Entry point: graph and input-mode menus
├── Dijkstra.h                  # Dijkstra (driving/walking, with avoid nodes/segments) + path reconstruction
├── data_structures/
│   ├── Graph.h                 # Graph template (vertices, edges, parking flag, driving/walking weights)
│   └── MutablePriorityQueue.h  # Min-heap with decrease-key used by Dijkstra
├── data_handler/               # CSV loading, path building/merging, output formatting helpers
├── input_handler/              # Interactive input and input.txt parsing
├── output_handler/             # Formats and writes results (terminal + output.txt)
├── driving/                    # Independent and restricted driving routes
├── drivingwalking/             # Driving + walking routes and approximations
└── cmake-build-debug/          # Datasets (Locations*.csv, Distances*.csv) and sample input.txt / output.txt
```

## Data

Two datasets are included in `cmake-build-debug/`:

| Dataset | Files | Size |
|---|---|---|
| Full (Porto) | `Locations.csv`, `Distances.csv` | ~1250 locations, ~1880 roads |
| Simplified | `LocationsSimplified.csv`, `DistancesSimplified.csv` | 8 locations, 12 roads (good for testing) |

- `Locations*.csv` – `Location,Id,Code,Parking` (`Parking` is `1` if you can park there).
- `Distances*.csv` – `Location1,Location2,Driving,Walking` (times in minutes; `X` means the road can't be driven, only walked).

## Requirements

- A C++20 compiler (GCC 10+, Clang 13+, or Apple Clang from Xcode Command Line Tools)
- CMake ≥ 3.30 (optional – you can compile with the compiler directly)

## Installation

```bash
git clone https://github.com/Atfas/FEUP-2Y2S-DA.git
cd FEUP-2Y2S-DA
```

## Build

**With CMake:**

```bash
cmake -S . -B build
cmake --build build
cp cmake-build-debug/*.csv cmake-build-debug/input.txt build/
```

**Without CMake:**

```bash
g++ -std=c++20 -O2 main.cpp data_handler/*.cpp input_handler/*.cpp output_handler/*.cpp driving/*.cpp drivingwalking/*.cpp -o cmake-build-debug/route_planner
```

> The program reads `Locations*.csv`, `Distances*.csv` and `input.txt` from the **current working directory**, so always run it from the folder that holds those files.

## Run

```bash
cd build            # or cmake-build-debug if you compiled with g++
./DA_Project_1      # or ./route_planner
```

You'll be asked:

1. **Graph type** – `csv` (full map) or `simplified`.
2. **Input mode** – `input` (type the request interactively) or `file` (read `input.txt`).

## Generating routes

### From a file (`input.txt`)

Edit `input.txt` in the run directory with one of these formats, then choose `file`.

**Independent driving** (exactly 3 lines):
```
Mode:driving
Source:5
Destination:4
```

**Restricted driving** (any field may be left empty):
```
Mode:driving
Source:5
Destination:4
AvoidNodes:2,3
AvoidSegments:(1,2),(4,7)
IncludeNode:7
```

**Driving + walking:**
```
Mode:driving-walking
Source:8
Destination:5
MaxWalkTime:18
AvoidNodes:
AvoidSegments:
```

### Interactively

Choose `input`, then type `driving` (and `independent` / `restricted`) or `driving-walking` and fill in each field when asked. Use the same formats as above (e.g. `2,3` for nodes, `(1,2),(4,7)` for segments).

### Output

The result is printed and saved to `output.txt`, e.g.:

```
Source:5
Destination:4
RestrictedDrivingRoute:5,3,7,8,4 (52)
```

The number in brackets is the total route time. `none` means no route was found.

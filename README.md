# Vehicle Routing Problem Using Simulated Annealing

A C++ implementation of the capacitated Vehicle Routing Problem (VRP) solved with a Simulated Annealing heuristic. This project was prepared for a university Heuristic Algorithms course.

## Overview

The solver reads node coordinates and demands from `VRP(nodes).csv`, constructs an initial feasible solution, and improves it using Simulated Annealing. It exports structured data for the HTML report (JSON/CSV) so charts and route maps can render in the browser.

The project includes:

- a modular object-oriented C++ codebase
- JSON exports for the final solution and annealing history
- CSV annealing history for spreadsheets
- a web report with Chart.js and modular canvas visualizations

## Problem Setting

- 1 depot and 200 customers
- vehicle capacity = `100`
- every route starts and ends at the depot
- every customer must be visited exactly once
- objective: minimize fixed truck cost and transportation cost

## Project Structure

```text
VRP/
|-- include/
|   |-- Node.h
|   |-- ProgressPoint.h
|   |-- Route.h
|   |-- SimulatedAnnealingSolver.h
|   |-- Solution.h
|   |-- SolutionVisualizer.h
|   `-- VRPProblem.h
|-- src/
|   |-- Route.cpp
|   |-- SimulatedAnnealingSolver.cpp
|   |-- Solution.cpp
|   |-- SolutionVisualizer.cpp
|   `-- VRPProblem.cpp
|-- docs/
|   `-- report/
|       |-- index.html
|       |-- styles.css
|       |-- REPORT_TEMPLATE.md
|       `-- assets/
|-- tools/
|   |-- generate_report_assets.py
|   `-- report_assets/
|       |-- __init__.py
|       |-- __main__.py
|       |-- cli.py
|       |-- eulerian_route.py
|       |-- generator.py
|       |-- models.py
|       |-- node_loader.py
|       |-- paths.py
|       |-- progress_io.py
|       |-- svg_coordinate_mapper.py
|       `-- svg_route_parser.py
|-- main.cpp
|-- README.md
`-- VRP(nodes).csv
```

## Main Components

- `VRPProblem`: loads the CSV dataset and builds the distance matrix
- `Route`: stores one vehicle route
- `Solution`: stores all routes and the total solution cost
- `SimulatedAnnealingSolver`: generates the initial solution and performs the heuristic search
- `SolutionVisualizer`: exports JSON/CSV output files for reporting

## Output

The solver writes:

- `annealing_progress.csv` and `annealing_progress.json` (temperature and best cost per step)
- `solution.json` (all node coordinates/demands and the final route sequences)

Copy those files into `docs/report/assets/` (for example with a `--prefix` per experiment), or regenerate the bundled Experiment A JSON files from existing report assets using:

```powershell
python tools/generate_report_assets.py
```

The HTML report expects JSON under `docs/report/assets/` and is easiest to view over a local HTTP server so `fetch()` can load the files.

## Running The Project

### VS Code + PowerShell + MSVC

This project was tested from the VS Code PowerShell terminal using the Visual Studio compiler environment.

Compile:

```powershell
& cmd /c '"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" && cl /EHsc /I include main.cpp src\Route.cpp src\Solution.cpp src\VRPProblem.cpp src\SimulatedAnnealingSolver.cpp src\SolutionVisualizer.cpp /Fe:vrp.exe'
```

Run:

```powershell
.\vrp.exe
```

### Running Experiments

The executable supports experiment parameters from the command line:

```powershell
.\vrp.exe --tmax 100000 --alpha 0.995 --iter 1000 --seed 42 --prefix exp_a_ --quiet
```

Available options:

- `--tmax`
- `--tmin`
- `--alpha`
- `--iter`
- `--seed`
- `--prefix`
- `--quiet`

## Report

The report materials are stored in `docs/report/`:

- `index.html`: browser-ready report page
- `styles.css`: report styling
- `REPORT_TEMPLATE.md`: written report structure
- `assets/`: generated figures and data used by the report

Open `docs/report/index.html` over HTTP (for example `npx --yes serve docs/report`) so chart and map data load correctly.

## Experimental Notes

The current web report includes temperature tuning experiments using several Simulated Annealing settings. The displayed results were produced by running the solver multiple times with the same random seed and comparing:

- total cost
- total distance
- number of vehicles used
- runtime

## Complexity

- greedy initial solution: approximately `O(n^2)`
- simulated annealing search: approximately `O(L * ITER * n)`

Where:

- `n` is the number of customers
- `L` is the number of cooling levels
- `ITER` is the number of iterations per temperature

## Future Improvements

- test more temperature schedules and multiple random seeds
- add stronger neighborhood operators such as 2-opt
- generate route snapshots for animation
- compare against a greedy-only baseline

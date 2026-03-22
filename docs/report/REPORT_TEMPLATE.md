# Vehicle Routing Problem Using Simulated Annealing

## 1. Introduction

This report presents a solution to the Vehicle Routing Problem (VRP) using the Simulated Annealing metaheuristic. The objective is to construct feasible routes that satisfy customer demand while minimizing total transportation cost.

## 2. Problem Description

- one depot and 200 customers
- each customer has a location and demand
- each vehicle has a maximum capacity of 100
- each route starts and ends at the depot
- each customer must be visited exactly once

## 3. Dataset Description

The dataset is stored in `VRP(nodes).csv` and includes:

- node ID
- x coordinate
- y coordinate
- demand

Node `0` represents the depot.

## 4. Objective Function

The objective is to minimize:

- fixed cost per truck
- transportation cost based on route distance and load

## 5. Simulated Annealing Method

1. Generate an initial feasible solution using a greedy nearest-neighbor strategy.
2. Generate neighboring solutions using:
   - customer swap between routes
   - customer relocation between routes
3. Accept improving solutions directly.
4. Sometimes accept worse solutions based on temperature.
5. Cool the temperature until the stopping condition is reached.

## 6. Parameter Settings

Document the values used for:

- `T_MAX`
- `T_MIN`
- `ALPHA`
- `ITER`
- random seed

## 7. Complexity Analysis

- greedy initialization: approximately `O(n^2)`
- simulated annealing phase: approximately `O(L * ITER * n)`

Where:

- `n` is the number of customers
- `L` is the number of cooling levels
- `ITER` is the number of iterations per temperature

## 8. Experimental Results

Use a table like this:

| Experiment | T_MAX | ALPHA | ITER | Final Cost | Distance | Vehicles | Runtime |
|---|---:|---:|---:|---:|---:|---:|---:|
| A | 100000 | 0.995 | 1000 | ... | ... | ... | ... |
| B | ... | ... | ... | ... | ... | ... | ... |

## 9. Graphs And Visual Results

Include:

- route graph SVG
- annealing progress SVG
- discussion of how temperature affected convergence

## 10. Discussion

Discuss:

- which setting gave the best final cost
- the tradeoff between runtime and solution quality
- strengths and limitations of Simulated Annealing
- possible improvements

## 11. Conclusion

Summarize:

- whether feasible routes were obtained
- how parameter tuning affected the result
- what could be improved in future work

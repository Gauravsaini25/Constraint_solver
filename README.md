# 2D Geometric Constraint Solver

A modular, iterative-relaxation-based 2D geometric constraint solver written in modern C++17. This is the kind of engine that sits at the core of parametric CAD tools like Autodesk Inventor, Fusion 360, and SolidWorks Sketcher.

## Architecture

```
┌──────────────────────────────────────────────────────┐
│                    Solver Engine                      │
│  ┌─────────┐  ┌──────────────────────────────────┐   │
│  │ Point[] │  │ Constraint[]                     │   │
│  │ (Vec2)  │  │  ├─ DistanceConstraint           │   │
│  │         │  │  ├─ FixedPointConstraint          │   │
│  │         │  │  ├─ HorizontalConstraint          │   │
│  │         │  │  ├─ VerticalConstraint            │   │
│  │         │  │  ├─ AngleConstraint               │   │
│  │         │  │  └─ CoincidentConstraint          │   │
│  └─────────┘  └──────────────────────────────────┘   │
│                                                      │
│  Gauss-Seidel Iterative Relaxation                   │
│  • RMS error convergence with configurable damping   │
│  • Per-constraint stiffness control                  │
│  • Error history for convergence analysis            │
└──────────────────────────────────────────────────────┘
```

## How It Works

**Iterative Relaxation (Gauss-Seidel style):**

1. For each iteration, visit every constraint in sequence
2. Each constraint computes its error (how far from satisfaction)
3. Each constraint nudges the involved points toward satisfaction
4. Points marked as "fixed" are never moved (act as anchors)
5. Repeat until RMS error < tolerance, or max iterations reached

Key difference from Jacobi iteration: each constraint sees the *updated* positions from previous constraints in the same iteration, leading to faster convergence.

## Constraints

| Constraint | Description | Parameters |
|---|---|---|
| `DistanceConstraint` | Fixed distance between two points | target distance |
| `FixedPointConstraint` | Locks a point at a coordinate | target position |
| `HorizontalConstraint` | Same y-coordinate for two points | — |
| `VerticalConstraint` | Same x-coordinate for two points | — |
| `AngleConstraint` | Angle at a vertex between three points | target angle (rad) |
| `CoincidentConstraint` | Two points overlap | — |

## Building

### Prerequisites
- C++17 compiler (GCC, Clang, or MSVC)
- CMake 3.16+

### Build
```bash
cmake -S . -B build
cmake --build build
```

### Run
```bash
./build/constraint_solver
```

## Project Structure

```
├── CMakeLists.txt
├── include/
│   ├── vec2.h                          # 2D vector math
│   ├── point.h                         # Point primitive
│   ├── constraint.h                    # Abstract constraint base
│   ├── solver.h                        # Solver engine
│   ├── solver_config.h                 # Config + result types
│   └── constraints/
│       ├── distance_constraint.h
│       ├── fixed_point_constraint.h
│       ├── horizontal_constraint.h
│       ├── vertical_constraint.h
│       ├── angle_constraint.h
│       └── coincident_constraint.h
├── src/
│   ├── vec2.cpp
│   ├── point.cpp
│   ├── solver.cpp
│   ├── main.cpp
│   └── constraints/
│       ├── distance_constraint.cpp
│       ├── fixed_point_constraint.cpp
│       ├── horizontal_constraint.cpp
│       ├── vertical_constraint.cpp
│       ├── angle_constraint.cpp
│       └── coincident_constraint.cpp
└── README.md
```

## Design Decisions

- **`Vec2` as value type**: Cheap to copy, supports all arithmetic operators, `constexpr` where possible
- **`shared_ptr<Point>`**: Multiple constraints reference the same point — shared ownership is natural
- **`unique_ptr<Constraint>`**: Solver exclusively owns constraints; polymorphic dispatch via vtable
- **Per-constraint stiffness + global damping**: Prevents oscillation in competing constraints
- **RMS error**: Normalized metric — threshold is meaningful regardless of constraint count
- **Fixed points**: Boolean flag on Point — constraints check this and skip movement

## Extending to Production Level

To evolve this into something comparable to Autodesk's solvers:

1. **Newton-Raphson solver**: Replace iterative relaxation with Jacobian-based solving for quadratic convergence
2. **Constraint graph analysis**: Detect under/over-constrained subsystems, find rigid clusters
3. **Line/Circle/Arc primitives**: Add geometric entities beyond points
4. **Parametric dimensions**: Link constraint values to expressions
5. **Undo/Redo**: Snapshot and restore point positions
6. **DOF analysis**: Report remaining degrees of freedom per point
7. **Prioritized constraints**: Weight certain constraints higher during solving

## License

MIT

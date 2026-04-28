cmake_minimum_required(VERSION 3.16)
project(ConstraintSolver2D
    VERSION 1.0.0
    DESCRIPTION "2D Geometric Constraint Solver using iterative relaxation"
    LANGUAGES CXX
)

# ──────────────────────────────────────────────
# Global compiler settings
# ──────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Strict warnings — treat this like production code
if(MSVC)
    add_compile_options(/W4 /WX /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wshadow -Wconversion -Wsign-conversion)
endif()

# ──────────────────────────────────────────────
# Core solver library
# ──────────────────────────────────────────────
set(SOLVER_SOURCES
    src/vec2.cpp
    src/point.cpp
    src/solver.cpp
    src/constraints/distance_constraint.cpp
    src/constraints/fixed_point_constraint.cpp
    src/constraints/horizontal_constraint.cpp
    src/constraints/vertical_constraint.cpp
    src/constraints/angle_constraint.cpp
    src/constraints/coincident_constraint.cpp
)

add_library(solver_core STATIC ${SOLVER_SOURCES})
target_include_directories(solver_core PUBLIC ${CMAKE_SOURCE_DIR}/include)

# ──────────────────────────────────────────────
# Main executable
# ──────────────────────────────────────────────
add_executable(constraint_solver src/main.cpp)
target_link_libraries(constraint_solver PRIVATE solver_core)

# ──────────────────────────────────────────────
# Install rules (optional)
# ──────────────────────────────────────────────
install(TARGETS constraint_solver DESTINATION bin)

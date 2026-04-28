#pragma once
/// @file solver.h
/// @brief Core solver engine that manages points and constraints.
///
/// The Solver orchestrates iterative relaxation: it repeatedly applies
/// all constraints, measuring convergence via RMS error, until the
/// system is satisfied or max iterations are reached.
///
/// Usage:
///   Solver solver;
///   solver.add_point(p1);
///   solver.add_constraint(std::make_unique<DistanceConstraint>(...));
///   SolveResult result = solver.solve();

#include "constraint.h"
#include "solver_config.h"

#include <functional>
#include <memory>
#include <vector>

namespace cs {

/// @brief The iterative constraint solver engine.
///
/// Manages a collection of points and constraints. The solve() method
/// performs Gauss-Seidel style iterative relaxation until convergence.
class Solver {
public:
    explicit Solver(SolverConfig config = {});

    // ── Point management ──────────────────────────────────────────

    /// Add a point to the solver's managed set.
    void add_point(std::shared_ptr<Point> point);

    /// Remove a point (also removes any constraints referencing it).
    void remove_point(uint32_t point_id);

    /// Get all managed points.
    [[nodiscard]] const std::vector<std::shared_ptr<Point>>& points() const { return points_; }

    // ── Constraint management ─────────────────────────────────────

    /// Add a constraint. Ownership is transferred to the solver.
    void add_constraint(std::unique_ptr<Constraint> constraint);

    /// Remove a constraint by index.
    void remove_constraint(size_t index);

    /// Get all constraints (read-only).
    [[nodiscard]] const std::vector<std::unique_ptr<Constraint>>& constraints() const {
        return constraints_;
    }

    // ── Solving ───────────────────────────────────────────────────

    /// Run the solver. Returns convergence result and diagnostics.
    [[nodiscard]] SolveResult solve();

    /// Compute the current total RMS error across all constraints.
    [[nodiscard]] double compute_total_error() const;

    /// Reset all points to their initial positions.
    void reset_points();

    // ── Configuration ─────────────────────────────────────────────

    void set_config(const SolverConfig& config) { config_ = config; }
    [[nodiscard]] const SolverConfig& config() const { return config_; }

    // ── Diagnostics ───────────────────────────────────────────────

    /// Print the current state of all points and constraints.
    void print_state(std::ostream& os) const;

    /// Print per-constraint error breakdown.
    void print_error_breakdown(std::ostream& os) const;

    /// Set a callback that fires after each iteration (for live visualization).
    using IterationCallback = std::function<void(uint32_t iteration, double error)>;
    void set_iteration_callback(IterationCallback callback);

private:
    std::vector<std::shared_ptr<Point>> points_;
    std::vector<std::unique_ptr<Constraint>> constraints_;
    SolverConfig config_;
    IterationCallback iteration_callback_;
};

} // namespace cs

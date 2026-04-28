#pragma once
/// @file solver_config.h
/// @brief Configuration and result types for the constraint solver.

#include <cstdint>
#include <vector>
#include <string>

namespace cs {

/// @brief Solver configuration parameters.
///
/// These control the convergence behavior of the iterative solver.
/// Tuning these is critical for balancing speed vs. stability.
struct SolverConfig {
    /// Maximum number of solver iterations before giving up.
    uint32_t max_iterations = 1000;

    /// Convergence threshold: solver stops when RMS error drops below this.
    /// A typical CAD system uses 1e-6 to 1e-9 depending on the unit system.
    double tolerance = 1e-6;

    /// Global damping factor multiplied with per-constraint stiffness.
    /// Range: (0.0, 1.0]. Lower values improve stability for competing
    /// constraints at the cost of convergence speed.
    double damping_factor = 0.5;

    /// Whether to record error history for diagnostics / plotting.
    bool record_error_history = true;
};

/// @brief Result of a solver run.
struct SolveResult {
    /// True if the solver converged (RMS error < tolerance).
    bool converged = false;

    /// Number of iterations actually performed.
    uint32_t iterations_used = 0;

    /// Final RMS error across all constraints.
    double final_error = 0.0;

    /// Error at each iteration (only populated if config.record_error_history).
    std::vector<double> error_history;

    /// Human-readable summary.
    [[nodiscard]] std::string to_string() const;
};

} // namespace cs

#pragma once
/// @file constraint.h
/// @brief Abstract base class for all geometric constraints.
///
/// Each constraint knows how to:
/// 1. Compute its current error (how far from satisfaction)
/// 2. Apply a correction (nudge points toward satisfaction)
/// 3. Report which points it involves (for dependency analysis)
///
/// Constraints are owned by the Solver via `std::unique_ptr<Constraint>`.

#include "point.h"

#include <memory>
#include <string>
#include <vector>

namespace cs {

/// @brief Abstract geometric constraint.
///
/// The constraint solver works by iteratively calling `apply()` on each
/// constraint. Each constraint adjusts the positions of its involved points
/// to reduce its error. The `stiffness` parameter controls how aggressively
/// the correction is applied (0.0 = no correction, 1.0 = full correction).
///
/// Subclasses must implement: apply(), error(), name(), involved_points().
class Constraint {
public:
    virtual ~Constraint() = default;

    // Prevent copying — constraints are managed by unique_ptr
    Constraint(const Constraint&) = delete;
    Constraint& operator=(const Constraint&) = delete;
    Constraint(Constraint&&) = default;
    Constraint& operator=(Constraint&&) = default;

    /// @brief Apply the constraint, adjusting point positions.
    /// @return The residual error after applying the correction.
    ///
    /// This is the core method called by the solver each iteration.
    /// Implementations should:
    /// 1. Compute the current error
    /// 2. Compute correction vectors for each involved point
    /// 3. Scale corrections by `stiffness`
    /// 4. Call `point->move_by(correction)` — this respects the fixed flag
    /// 5. Return the remaining error
    virtual double apply() = 0;

    /// @brief Compute the current error without modifying any points.
    /// @return Scalar error value (≥ 0). Zero means fully satisfied.
    [[nodiscard]] virtual double error() const = 0;

    /// @brief Human-readable constraint name for diagnostics.
    [[nodiscard]] virtual std::string name() const = 0;

    /// @brief Get all points involved in this constraint.
    /// Used for dependency analysis, visualization, and diagnostics.
    [[nodiscard]] virtual std::vector<std::shared_ptr<Point>> involved_points() const = 0;

    /// Stiffness controls how aggressively corrections are applied.
    /// Range: [0.0, 1.0]. Lower values improve stability at the cost of convergence speed.
    double stiffness = 1.0;

protected:
    Constraint() = default;
    explicit Constraint(double s) : stiffness(s) {}
};

} // namespace cs

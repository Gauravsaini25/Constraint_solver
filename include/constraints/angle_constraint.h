#pragma once
/// @file angle_constraint.h
/// @brief Constrains the angle at a vertex formed by three points.
///
/// Given three points A, B, C, this constraint maintains the angle at B
/// (the vertex) between rays BA and BC at a target value. This is the
/// most mathematically complex constraint in the solver.
///
/// Angle is measured in radians, in the range [0, π].

#include "constraint.h"

namespace cs {

class AngleConstraint : public Constraint {
public:
    /// @param p1 First arm point (A)
    /// @param vertex Vertex point (B) — the angle is measured here
    /// @param p3 Second arm point (C)
    /// @param target_angle_rad Desired angle in radians [0, π]
    /// @param stiffness Correction strength [0.0, 1.0]
    AngleConstraint(std::shared_ptr<Point> p1,
                    std::shared_ptr<Point> vertex,
                    std::shared_ptr<Point> p3,
                    double target_angle_rad,
                    double s = 1.0);

    double apply() override;
    [[nodiscard]] double error() const override;
    [[nodiscard]] std::string name() const override;
    [[nodiscard]] std::vector<std::shared_ptr<Point>> involved_points() const override;

private:
    std::shared_ptr<Point> p1_;      // Arm point A
    std::shared_ptr<Point> vertex_;  // Vertex B
    std::shared_ptr<Point> p3_;      // Arm point C
    double target_angle_rad_;

    /// Compute the current angle at the vertex.
    [[nodiscard]] double current_angle() const;
};

} // namespace cs

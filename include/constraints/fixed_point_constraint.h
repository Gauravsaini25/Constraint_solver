#pragma once
/// @file fixed_point_constraint.h
/// @brief Locks a point at a specific position.
///
/// This constraint acts as a hard anchor — it forces a point to a target
/// coordinate. Useful for grounding a sketch (e.g., fixing the origin point).

#include "constraint.h"

namespace cs {

class FixedPointConstraint : public Constraint {
public:
    /// @param point The point to fix
    /// @param target The target position to lock the point to
    /// @param stiffness Correction strength [0.0, 1.0]
    FixedPointConstraint(std::shared_ptr<Point> point,
                         Vec2 target,
                         double s = 1.0);

    double apply() override;
    [[nodiscard]] double error() const override;
    [[nodiscard]] std::string name() const override;
    [[nodiscard]] std::vector<std::shared_ptr<Point>> involved_points() const override;

private:
    std::shared_ptr<Point> point_;
    Vec2 target_;
};

} // namespace cs

#pragma once
/// @file coincident_constraint.h
/// @brief Forces two points to occupy the same position.
///
/// This is a special case of DistanceConstraint with distance = 0,
/// but implemented separately for clarity and because the correction
/// strategy is simpler (midpoint averaging).

#include "constraint.h"

namespace cs {

class CoincidentConstraint : public Constraint {
public:
    /// @param p1 First point
    /// @param p2 Second point
    /// @param stiffness Correction strength [0.0, 1.0]
    CoincidentConstraint(std::shared_ptr<Point> p1,
                         std::shared_ptr<Point> p2,
                         double s = 1.0);

    double apply() override;
    [[nodiscard]] double error() const override;
    [[nodiscard]] std::string name() const override;
    [[nodiscard]] std::vector<std::shared_ptr<Point>> involved_points() const override;

private:
    std::shared_ptr<Point> p1_;
    std::shared_ptr<Point> p2_;
};

} // namespace cs

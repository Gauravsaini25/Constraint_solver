#pragma once
/// @file horizontal_constraint.h
/// @brief Forces two points to share the same y-coordinate.
///
/// This is equivalent to constraining the two points to lie on a horizontal
/// line. The correction averages their y-coordinates.

#include "constraint.h"

namespace cs {

class HorizontalConstraint : public Constraint {
public:
    /// @param p1 First point
    /// @param p2 Second point
    /// @param stiffness Correction strength [0.0, 1.0]
    HorizontalConstraint(std::shared_ptr<Point> p1,
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

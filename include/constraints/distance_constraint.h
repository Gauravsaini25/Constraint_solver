#pragma once
/// @file distance_constraint.h
/// @brief Maintains a fixed distance between two points.
///
/// This is the most fundamental parametric constraint. It ensures that
/// two points remain exactly `target_distance` apart by moving them
/// along the line connecting them.

#include "constraint.h"

namespace cs {

class DistanceConstraint : public Constraint {
public:
    /// @param p1 First point
    /// @param p2 Second point
    /// @param target_distance Desired distance between p1 and p2
    /// @param stiffness Correction strength [0.0, 1.0]
    DistanceConstraint(std::shared_ptr<Point> p1,
                       std::shared_ptr<Point> p2,
                       double target_distance,
                       double s = 1.0);

    double apply() override;
    [[nodiscard]] double error() const override;
    [[nodiscard]] std::string name() const override;
    [[nodiscard]] std::vector<std::shared_ptr<Point>> involved_points() const override;

private:
    std::shared_ptr<Point> p1_;
    std::shared_ptr<Point> p2_;
    double target_distance_;
};

} // namespace cs

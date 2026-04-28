#include "constraints/distance_constraint.h"

#include <cmath>
#include <sstream>

namespace cs {

DistanceConstraint::DistanceConstraint(std::shared_ptr<Point> p1,
                                       std::shared_ptr<Point> p2,
                                       double target_distance,
                                       double s)
    : Constraint(s), p1_(std::move(p1)), p2_(std::move(p2)),
      target_distance_(target_distance) {}

double DistanceConstraint::apply() {
    Vec2 delta = p2_->position() - p1_->position();
    double current_dist = delta.length();

    // Degenerate case: points are coincident but we need a non-zero distance.
    // Nudge in an arbitrary direction to break the degeneracy.
    if (current_dist < kEpsilon) {
        if (target_distance_ < kEpsilon) {
            return 0.0; // Both want to be at the same place — satisfied.
        }
        delta = Vec2(1.0, 0.0); // Arbitrary direction
        current_dist = 1.0;
    }

    double err = current_dist - target_distance_;
    Vec2 direction = delta / current_dist;

    // Correction: move each point by half the error along the connecting line.
    // If one point is fixed, the other absorbs the full correction.
    Vec2 correction = direction * (err * stiffness * 0.5);

    bool p1_fixed = p1_->is_fixed();
    bool p2_fixed = p2_->is_fixed();

    if (p1_fixed && p2_fixed) {
        // Both fixed — can't do anything
        return std::abs(err);
    } else if (p1_fixed) {
        // Only p2 moves — absorb full correction
        p2_->move_by(-correction * 2.0);
    } else if (p2_fixed) {
        // Only p1 moves — absorb full correction
        p1_->move_by(correction * 2.0);
    } else {
        // Both move equally
        p1_->move_by(correction);
        p2_->move_by(-correction);
    }

    return std::abs(err);
}

double DistanceConstraint::error() const {
    return std::abs(distance(p1_->position(), p2_->position()) - target_distance_);
}

std::string DistanceConstraint::name() const {
    std::ostringstream oss;
    oss << "Distance(P" << p1_->id() << " <-> P" << p2_->id()
        << ", d=" << target_distance_ << ")";
    return oss.str();
}

std::vector<std::shared_ptr<Point>> DistanceConstraint::involved_points() const {
    return {p1_, p2_};
}

} // namespace cs

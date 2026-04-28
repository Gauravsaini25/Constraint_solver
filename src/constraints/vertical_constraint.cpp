#include "constraints/vertical_constraint.h"

#include <cmath>
#include <sstream>

namespace cs {

VerticalConstraint::VerticalConstraint(std::shared_ptr<Point> p1,
                                       std::shared_ptr<Point> p2,
                                       double s)
    : Constraint(s), p1_(std::move(p1)), p2_(std::move(p2)) {}

double VerticalConstraint::apply() {
    double x1 = p1_->position().x;
    double x2 = p2_->position().x;
    double err = x2 - x1;

    if (std::abs(err) < kEpsilon) {
        return 0.0;
    }

    bool p1_fixed = p1_->is_fixed();
    bool p2_fixed = p2_->is_fixed();

    if (p1_fixed && p2_fixed) {
        return std::abs(err);
    } else if (p1_fixed) {
        p2_->move_by(Vec2(-err * stiffness, 0.0));
    } else if (p2_fixed) {
        p1_->move_by(Vec2(err * stiffness, 0.0));
    } else {
        double half_correction = err * stiffness * 0.5;
        p1_->move_by(Vec2(half_correction, 0.0));
        p2_->move_by(Vec2(-half_correction, 0.0));
    }

    return std::abs(err);
}

double VerticalConstraint::error() const {
    return std::abs(p1_->position().x - p2_->position().x);
}

std::string VerticalConstraint::name() const {
    std::ostringstream oss;
    oss << "Vertical(P" << p1_->id() << ", P" << p2_->id() << ")";
    return oss.str();
}

std::vector<std::shared_ptr<Point>> VerticalConstraint::involved_points() const {
    return {p1_, p2_};
}

} // namespace cs

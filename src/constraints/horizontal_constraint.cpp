#include "constraints/horizontal_constraint.h"

#include <cmath>
#include <sstream>

namespace cs {

HorizontalConstraint::HorizontalConstraint(std::shared_ptr<Point> p1,
                                           std::shared_ptr<Point> p2,
                                           double s)
    : Constraint(s), p1_(std::move(p1)), p2_(std::move(p2)) {}

double HorizontalConstraint::apply() {
    double y1 = p1_->position().y;
    double y2 = p2_->position().y;
    double err = y2 - y1;

    if (std::abs(err) < kEpsilon) {
        return 0.0;
    }

    bool p1_fixed = p1_->is_fixed();
    bool p2_fixed = p2_->is_fixed();

    if (p1_fixed && p2_fixed) {
        return std::abs(err);
    } else if (p1_fixed) {
        // Move p2's y to match p1's y
        p2_->move_by(Vec2(0.0, -err * stiffness));
    } else if (p2_fixed) {
        // Move p1's y to match p2's y
        p1_->move_by(Vec2(0.0, err * stiffness));
    } else {
        // Average: move each by half
        double half_correction = err * stiffness * 0.5;
        p1_->move_by(Vec2(0.0, half_correction));
        p2_->move_by(Vec2(0.0, -half_correction));
    }

    return std::abs(err);
}

double HorizontalConstraint::error() const {
    return std::abs(p1_->position().y - p2_->position().y);
}

std::string HorizontalConstraint::name() const {
    std::ostringstream oss;
    oss << "Horizontal(P" << p1_->id() << ", P" << p2_->id() << ")";
    return oss.str();
}

std::vector<std::shared_ptr<Point>> HorizontalConstraint::involved_points() const {
    return {p1_, p2_};
}

} // namespace cs

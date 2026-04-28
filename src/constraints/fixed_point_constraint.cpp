#include "constraints/fixed_point_constraint.h"

#include <sstream>

namespace cs {

FixedPointConstraint::FixedPointConstraint(std::shared_ptr<Point> point,
                                           Vec2 target,
                                           double s)
    : Constraint(s), point_(std::move(point)), target_(target) {}

double FixedPointConstraint::apply() {
    Vec2 delta = target_ - point_->position();
    Vec2 correction = delta * stiffness;
    point_->move_by(correction);
    return delta.length();
}

double FixedPointConstraint::error() const {
    return distance(point_->position(), target_);
}

std::string FixedPointConstraint::name() const {
    std::ostringstream oss;
    oss << "Fixed(P" << point_->id() << " -> " << target_ << ")";
    return oss.str();
}

std::vector<std::shared_ptr<Point>> FixedPointConstraint::involved_points() const {
    return {point_};
}

} // namespace cs

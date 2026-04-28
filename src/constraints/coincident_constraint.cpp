#include "constraints/coincident_constraint.h"

#include <sstream>

namespace cs {

CoincidentConstraint::CoincidentConstraint(std::shared_ptr<Point> p1,
                                           std::shared_ptr<Point> p2,
                                           double s)
    : Constraint(s), p1_(std::move(p1)), p2_(std::move(p2)) {}

double CoincidentConstraint::apply() {
    Vec2 pos1 = p1_->position();
    Vec2 pos2 = p2_->position();
    Vec2 delta = pos2 - pos1;
    double err = delta.length();

    if (err < kEpsilon) {
        return 0.0;
    }

    bool p1_fixed = p1_->is_fixed();
    bool p2_fixed = p2_->is_fixed();

    if (p1_fixed && p2_fixed) {
        return err;
    } else if (p1_fixed) {
        // Move p2 to p1
        p2_->move_by(-delta * stiffness);
    } else if (p2_fixed) {
        // Move p1 to p2
        p1_->move_by(delta * stiffness);
    } else {
        // Move both to midpoint
        Vec2 midpoint = (pos1 + pos2) * 0.5;
        p1_->move_by((midpoint - pos1) * stiffness);
        p2_->move_by((midpoint - pos2) * stiffness);
    }

    return err;
}

double CoincidentConstraint::error() const {
    return distance(p1_->position(), p2_->position());
}

std::string CoincidentConstraint::name() const {
    std::ostringstream oss;
    oss << "Coincident(P" << p1_->id() << ", P" << p2_->id() << ")";
    return oss.str();
}

std::vector<std::shared_ptr<Point>> CoincidentConstraint::involved_points() const {
    return {p1_, p2_};
}

} // namespace cs

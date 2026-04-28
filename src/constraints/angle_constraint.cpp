#define _USE_MATH_DEFINES
#include "constraints/angle_constraint.h"

#include <cmath>
#include <sstream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace cs {

AngleConstraint::AngleConstraint(std::shared_ptr<Point> p1,
                                 std::shared_ptr<Point> vertex,
                                 std::shared_ptr<Point> p3,
                                 double target_angle_rad,
                                 double s)
    : Constraint(s), p1_(std::move(p1)), vertex_(std::move(vertex)),
      p3_(std::move(p3)), target_angle_rad_(target_angle_rad) {}

double AngleConstraint::current_angle() const {
    Vec2 v_pos = vertex_->position();
    Vec2 arm_a = p1_->position() - v_pos;
    Vec2 arm_c = p3_->position() - v_pos;
    return arm_a.angle_to(arm_c);
}

double AngleConstraint::apply() {
    Vec2 v_pos = vertex_->position();
    Vec2 arm_a = p1_->position() - v_pos;
    Vec2 arm_c = p3_->position() - v_pos;

    double len_a = arm_a.length();
    double len_c = arm_c.length();

    // Can't define an angle if either arm has zero length
    if (len_a < kEpsilon || len_c < kEpsilon) {
        return 0.0;
    }

    double current = arm_a.angle_to(arm_c);
    double angle_error = current - target_angle_rad_;

    if (std::abs(angle_error) < kEpsilon) {
        return 0.0;
    }

    // Strategy: rotate p1 and p3 around the vertex to correct the angle.
    // Each point absorbs half the angular correction (unless one is fixed).
    // We use the signed angle (via cross product) to determine rotation direction.
    double sign = (arm_a.cross(arm_c) >= 0.0) ? 1.0 : -1.0;
    double half_correction = angle_error * stiffness * 0.5;

    bool p1_fixed = p1_->is_fixed();
    bool p3_fixed = p3_->is_fixed();
    bool v_fixed = vertex_->is_fixed();

    if (p1_fixed && p3_fixed) {
        // Can only move the vertex — complex, apply a perpendicular nudge
        if (!v_fixed) {
            // Approximate: move vertex perpendicular to the bisector
            Vec2 bisector = (arm_a.normalized() + arm_c.normalized()).normalized();
            Vec2 perp = bisector.perpendicular();
            double nudge_magnitude = angle_error * stiffness * 0.3;
            vertex_->move_by(perp * nudge_magnitude * sign);
        }
        return std::abs(angle_error);
    }

    if (!p1_fixed) {
        // Rotate p1 around vertex by +half_correction
        double rotation = (p3_fixed) ? angle_error * stiffness : half_correction;
        rotation *= sign;
        Vec2 new_arm_a = arm_a.rotated(-rotation);
        Vec2 new_p1 = v_pos + new_arm_a;
        p1_->move_by(new_p1 - p1_->position());
    }

    if (!p3_fixed) {
        // Rotate p3 around vertex by -half_correction
        double rotation = (p1_fixed) ? angle_error * stiffness : half_correction;
        rotation *= sign;
        Vec2 new_arm_c = arm_c.rotated(rotation);
        Vec2 new_p3 = v_pos + new_arm_c;
        p3_->move_by(new_p3 - p3_->position());
    }

    return std::abs(angle_error);
}

double AngleConstraint::error() const {
    return std::abs(current_angle() - target_angle_rad_);
}

std::string AngleConstraint::name() const {
    std::ostringstream oss;
    oss << "Angle(P" << p1_->id() << "-P" << vertex_->id()
        << "-P" << p3_->id() << ", θ="
        << (target_angle_rad_ * 180.0 / M_PI) << "°)";
    return oss.str();
}

std::vector<std::shared_ptr<Point>> AngleConstraint::involved_points() const {
    return {p1_, vertex_, p3_};
}

} // namespace cs

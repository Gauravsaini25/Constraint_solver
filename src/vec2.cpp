#include "vec2.h"

#include <cmath>
#include <iomanip>
#include <sstream>

namespace cs {

double Vec2::length() const {
    return std::sqrt(x * x + y * y);
}

Vec2 Vec2::normalized() const {
    double len = length();
    if (len < kEpsilon) {
        return {0.0, 0.0};
    }
    return {x / len, y / len};
}

double Vec2::angle() const {
    return std::atan2(y, x);
}

double Vec2::angle_to(const Vec2& other) const {
    double len_product = length() * other.length();
    if (len_product < kEpsilon) {
        return 0.0;
    }
    // Clamp to [-1, 1] to handle floating-point drift
    double cos_angle = dot(other) / len_product;
    cos_angle = std::fmax(-1.0, std::fmin(1.0, cos_angle));
    return std::acos(cos_angle);
}

Vec2 Vec2::rotated(double angle_rad) const {
    double c = std::cos(angle_rad);
    double s = std::sin(angle_rad);
    return {x * c - y * s, x * s + y * c};
}

std::string Vec2::to_string() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(4) << "(" << x << ", " << y << ")";
    return oss.str();
}

std::ostream& operator<<(std::ostream& os, const Vec2& v) {
    os << v.to_string();
    return os;
}

double distance(const Vec2& a, const Vec2& b) {
    return (a - b).length();
}

} // namespace cs

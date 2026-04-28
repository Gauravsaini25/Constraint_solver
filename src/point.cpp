#include "point.h"

#include <sstream>
#include <iomanip>

namespace cs {

std::atomic<uint32_t> Point::next_id_{0};

Point::Point(double x, double y)
    : id_(next_id_++), position_(x, y), initial_position_(x, y) {}

Point::Point(Vec2 position)
    : id_(next_id_++), position_(position), initial_position_(position) {}

void Point::set_position(Vec2 pos) {
    position_ = pos;
}

void Point::move_by(const Vec2& delta) {
    if (!fixed_) {
        position_ += delta;
    }
}

void Point::reset() {
    position_ = initial_position_;
}

double Point::distance_to(const Point& other) const {
    return distance(position_, other.position_);
}

std::string Point::to_string() const {
    std::ostringstream oss;
    oss << "Point[" << id_ << "]" << position_
        << (fixed_ ? " (FIXED)" : "");
    return oss.str();
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << p.to_string();
    return os;
}

} // namespace cs

#pragma once
/// @file point.h
/// @brief Geometric point primitive for the constraint solver.
///
/// Points are the fundamental primitives that constraints operate on.
/// Each point has a unique ID, a position, and a fixed flag that prevents
/// constraint-driven movement (useful for anchors / ground points).

#include "vec2.h"

#include <cstdint>
#include <atomic>
#include <string>

namespace cs {

/// @brief A 2D point in the constraint system.
///
/// Points are managed via `std::shared_ptr` because multiple constraints
/// reference the same point. The solver holds the authoritative collection,
/// and constraints hold shared references.
class Point {
public:
    // ── Constructors ──────────────────────────────────────────────

    /// Create a point at the given coordinates.
    explicit Point(double x, double y);

    /// Create a point from a Vec2 position.
    explicit Point(Vec2 position);

    // ── Accessors ─────────────────────────────────────────────────

    [[nodiscard]] uint32_t id() const { return id_; }
    [[nodiscard]] Vec2 position() const { return position_; }
    [[nodiscard]] Vec2 initial_position() const { return initial_position_; }
    [[nodiscard]] bool is_fixed() const { return fixed_; }

    // ── Mutators ──────────────────────────────────────────────────

    /// Set the position directly (bypasses fixed check — use for initialization).
    void set_position(Vec2 pos);

    /// Move the point by a delta vector. No-op if the point is fixed.
    void move_by(const Vec2& delta);

    /// Fix this point in place — constraints will not move it.
    void set_fixed(bool fixed) { fixed_ = fixed; }

    /// Reset position to initial position.
    void reset();

    // ── Geometry ──────────────────────────────────────────────────

    /// Distance to another point.
    [[nodiscard]] double distance_to(const Point& other) const;

    // ── Diagnostics ───────────────────────────────────────────────

    /// Human-readable string: "Point[id](x, y)"
    [[nodiscard]] std::string to_string() const;

    friend std::ostream& operator<<(std::ostream& os, const Point& p);

private:
    uint32_t id_;
    Vec2 position_;
    Vec2 initial_position_;
    bool fixed_ = false;

    /// Thread-safe monotonic ID counter.
    static std::atomic<uint32_t> next_id_;
};

} // namespace cs

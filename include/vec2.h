#pragma once
/// @file vec2.h
/// @brief 2D vector math utilities for the constraint solver.
///
/// Vec2 is a lightweight value type representing a 2D vector or point.
/// All operators are provided for ergonomic math, and common geometric
/// utilities (length, normalization, dot/cross products) are included.

#include <cmath>
#include <iostream>
#include <string>

namespace cs {

/// Floating-point comparison tolerance used throughout the solver.
inline constexpr double kEpsilon = 1e-9;

/// @brief 2D vector / point.
///
/// Designed as a value type — cheap to copy, supports all arithmetic operators.
/// Used both as positional coordinates and as direction/displacement vectors.
struct Vec2 {
    double x = 0.0;
    double y = 0.0;

    // ── Constructors ──────────────────────────────────────────────
    constexpr Vec2() = default;
    constexpr Vec2(double x_val, double y_val) : x(x_val), y(y_val) {}

    // ── Arithmetic operators ──────────────────────────────────────
    constexpr Vec2 operator+(const Vec2& rhs) const { return {x + rhs.x, y + rhs.y}; }
    constexpr Vec2 operator-(const Vec2& rhs) const { return {x - rhs.x, y - rhs.y}; }
    constexpr Vec2 operator*(double scalar) const { return {x * scalar, y * scalar}; }
    constexpr Vec2 operator/(double scalar) const { return {x / scalar, y / scalar}; }
    constexpr Vec2 operator-() const { return {-x, -y}; }

    Vec2& operator+=(const Vec2& rhs) { x += rhs.x; y += rhs.y; return *this; }
    Vec2& operator-=(const Vec2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
    Vec2& operator*=(double scalar) { x *= scalar; y *= scalar; return *this; }
    Vec2& operator/=(double scalar) { x /= scalar; y /= scalar; return *this; }

    bool operator==(const Vec2& rhs) const {
        return std::abs(x - rhs.x) < kEpsilon && std::abs(y - rhs.y) < kEpsilon;
    }
    bool operator!=(const Vec2& rhs) const { return !(*this == rhs); }

    // ── Geometric operations ──────────────────────────────────────

    /// Euclidean length (L2 norm).
    [[nodiscard]] double length() const;

    /// Squared length — avoids sqrt, useful for comparisons.
    [[nodiscard]] constexpr double length_squared() const { return x * x + y * y; }

    /// Returns unit vector in the same direction. Returns zero vector if length < epsilon.
    [[nodiscard]] Vec2 normalized() const;

    /// Dot product.
    [[nodiscard]] constexpr double dot(const Vec2& rhs) const { return x * rhs.x + y * rhs.y; }

    /// 2D cross product (z-component of the 3D cross product).
    /// Useful for determining winding order / signed area.
    [[nodiscard]] constexpr double cross(const Vec2& rhs) const { return x * rhs.y - y * rhs.x; }

    /// Returns the perpendicular vector (rotated 90° counter-clockwise).
    [[nodiscard]] constexpr Vec2 perpendicular() const { return {-y, x}; }

    /// Angle of this vector from the positive x-axis, in radians [-π, π].
    [[nodiscard]] double angle() const;

    /// Angle between this vector and another, in radians [0, π].
    [[nodiscard]] double angle_to(const Vec2& other) const;

    /// Linearly interpolate between this and another vector.
    [[nodiscard]] constexpr Vec2 lerp(const Vec2& target, double t) const {
        return {x + (target.x - x) * t, y + (target.y - y) * t};
    }

    /// Rotate this vector by an angle (radians) around the origin.
    [[nodiscard]] Vec2 rotated(double angle_rad) const;

    /// Human-readable string representation.
    [[nodiscard]] std::string to_string() const;

    // ── Stream output ─────────────────────────────────────────────
    friend std::ostream& operator<<(std::ostream& os, const Vec2& v);
};

// ── Free function: scalar * Vec2 ──────────────────────────────────
constexpr Vec2 operator*(double scalar, const Vec2& v) { return {v.x * scalar, v.y * scalar}; }

// ── Free geometric functions ──────────────────────────────────────

/// Euclidean distance between two points.
[[nodiscard]] double distance(const Vec2& a, const Vec2& b);

/// Squared distance between two points.
[[nodiscard]] constexpr double distance_squared(const Vec2& a, const Vec2& b) {
    return (a - b).length_squared();
}

/// Dot product (free function form).
[[nodiscard]] constexpr double dot(const Vec2& a, const Vec2& b) { return a.dot(b); }

/// Cross product (free function form).
[[nodiscard]] constexpr double cross(const Vec2& a, const Vec2& b) { return a.cross(b); }

} // namespace cs

/// @file main.cpp
/// @brief Demo driver for the 2D Geometric Constraint Solver.

#define _USE_MATH_DEFINES
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "solver.h"
#include "constraints/distance_constraint.h"
#include "constraints/fixed_point_constraint.h"
#include "constraints/horizontal_constraint.h"
#include "constraints/vertical_constraint.h"
#include "constraints/angle_constraint.h"
#include "constraints/coincident_constraint.h"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <memory>

using namespace cs;

// ── Helper ────────────────────────────────────────────────────────

static void print_header(const std::string& title) {
    std::cout << "\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(60, '=') << "\n";
}

static void print_points(const std::string& label,
                          const std::vector<std::shared_ptr<Point>>& pts) {
    std::cout << label << ":\n";
    for (const auto& p : pts) {
        std::cout << "  " << *p << "\n";
    }
}

static void verify_distance(const std::string& label,
                              const Point& a, const Point& b,
                              double expected) {
    double actual = a.distance_to(b);
    bool ok = std::abs(actual - expected) < 1e-4;
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "  " << label << ": "
              << actual << " (expected " << expected << ") "
              << (ok ? "[PASS]" : "[FAIL]") << "\n";
}

// ── Scenario 1: Triangle ─────────────────────────────────────────

static void scenario_triangle() {
    print_header("Scenario 1: Equilateral Triangle (side = 5)");

    // Start with points in a rough triangle shape
    auto p1 = std::make_shared<Point>(0.0, 0.0);
    auto p2 = std::make_shared<Point>(6.0, 0.5);
    auto p3 = std::make_shared<Point>(2.0, 4.0);

    Solver solver;
    solver.add_point(p1);
    solver.add_point(p2);
    solver.add_point(p3);

    // Fix p1 at origin to anchor the system
    p1->set_fixed(true);

    solver.add_constraint(std::make_unique<DistanceConstraint>(p1, p2, 5.0));
    solver.add_constraint(std::make_unique<DistanceConstraint>(p2, p3, 5.0));
    solver.add_constraint(std::make_unique<DistanceConstraint>(p3, p1, 5.0));

    print_points("Initial positions", solver.points());
    solver.print_state(std::cout);

    SolveResult result = solver.solve();
    std::cout << result.to_string();

    print_points("Final positions", solver.points());
    std::cout << "\nVerification:\n";
    verify_distance("P1-P2", *p1, *p2, 5.0);
    verify_distance("P2-P3", *p2, *p3, 5.0);
    verify_distance("P3-P1", *p3, *p1, 5.0);
}

// ── Scenario 2: Rectangle ────────────────────────────────────────

static void scenario_rectangle() {
    print_header("Scenario 2: Rectangle (4x3)");

    // Start with a rough quadrilateral
    auto p1 = std::make_shared<Point>(0.0, 0.0);
    auto p2 = std::make_shared<Point>(4.5, 0.3);
    auto p3 = std::make_shared<Point>(3.8, 3.2);
    auto p4 = std::make_shared<Point>(0.2, 2.8);

    p1->set_fixed(true);

    Solver solver;
    solver.add_point(p1);
    solver.add_point(p2);
    solver.add_point(p3);
    solver.add_point(p4);

    // Horizontal edges
    solver.add_constraint(std::make_unique<HorizontalConstraint>(p1, p2));
    solver.add_constraint(std::make_unique<HorizontalConstraint>(p4, p3));
    // Vertical edges
    solver.add_constraint(std::make_unique<VerticalConstraint>(p1, p4));
    solver.add_constraint(std::make_unique<VerticalConstraint>(p2, p3));
    // Distance constraints for sides
    solver.add_constraint(std::make_unique<DistanceConstraint>(p1, p2, 4.0));
    solver.add_constraint(std::make_unique<DistanceConstraint>(p2, p3, 3.0));
    solver.add_constraint(std::make_unique<DistanceConstraint>(p3, p4, 4.0));
    solver.add_constraint(std::make_unique<DistanceConstraint>(p4, p1, 3.0));

    print_points("Initial positions", solver.points());

    SolveResult result = solver.solve();
    std::cout << result.to_string();

    print_points("Final positions", solver.points());
    solver.print_error_breakdown(std::cout);
}

// ── Scenario 3: Fixed Anchor + Pendulum ──────────────────────────

static void scenario_pendulum() {
    print_header("Scenario 3: Fixed Anchor + Pendulum Arm (length = 7)");

    auto anchor = std::make_shared<Point>(0.0, 10.0);
    auto tip = std::make_shared<Point>(3.0, 5.0);

    anchor->set_fixed(true);

    Solver solver;
    solver.add_point(anchor);
    solver.add_point(tip);

    solver.add_constraint(
        std::make_unique<FixedPointConstraint>(anchor, Vec2(0.0, 10.0)));
    solver.add_constraint(
        std::make_unique<DistanceConstraint>(anchor, tip, 7.0));

    print_points("Initial positions", solver.points());

    SolveResult result = solver.solve();
    std::cout << result.to_string();

    print_points("Final positions", solver.points());
    verify_distance("Anchor-Tip", *anchor, *tip, 7.0);
}

// ── Scenario 4: Angle Constraint ─────────────────────────────────

static void scenario_angle() {
    print_header("Scenario 4: 90-degree Angle Constraint");

    auto p1 = std::make_shared<Point>(3.0, 0.0);
    auto vertex = std::make_shared<Point>(0.0, 0.0);
    auto p3 = std::make_shared<Point>(0.0, 4.0);

    vertex->set_fixed(true);

    Solver solver;
    solver.add_point(p1);
    solver.add_point(vertex);
    solver.add_point(p3);

    double target_angle = M_PI / 2.0; // 90 degrees
    solver.add_constraint(
        std::make_unique<AngleConstraint>(p1, vertex, p3, target_angle));
    // Also fix distances so the triangle holds shape
    solver.add_constraint(
        std::make_unique<DistanceConstraint>(vertex, p1, 3.0));
    solver.add_constraint(
        std::make_unique<DistanceConstraint>(vertex, p3, 4.0));

    print_points("Initial positions", solver.points());

    SolveResult result = solver.solve();
    std::cout << result.to_string();

    print_points("Final positions", solver.points());

    // Verify angle
    Vec2 arm_a = p1->position() - vertex->position();
    Vec2 arm_c = p3->position() - vertex->position();
    double actual_angle = arm_a.angle_to(arm_c) * 180.0 / M_PI;
    bool ok = std::abs(actual_angle - 90.0) < 0.01;
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "\n  Angle at vertex: " << actual_angle
              << "° (expected 90°) " << (ok ? "[PASS]" : "[FAIL]") << "\n";
}

// ── Scenario 5: Coincident ───────────────────────────────────────

static void scenario_coincident() {
    print_header("Scenario 5: Coincident Constraint (merge two points)");

    auto p1 = std::make_shared<Point>(1.0, 2.0);
    auto p2 = std::make_shared<Point>(5.0, 8.0);

    Solver solver;
    solver.add_point(p1);
    solver.add_point(p2);

    solver.add_constraint(std::make_unique<CoincidentConstraint>(p1, p2));

    print_points("Initial positions", solver.points());

    SolveResult result = solver.solve();
    std::cout << result.to_string();

    print_points("Final positions", solver.points());
    verify_distance("P1-P2 (should be 0)", *p1, *p2, 0.0);
}

// ── Scenario 6: Over-constrained ─────────────────────────────────

static void scenario_overconstrained() {
    print_header("Scenario 6: Over-constrained System (conflicting distances)");

    auto p1 = std::make_shared<Point>(0.0, 0.0);
    auto p2 = std::make_shared<Point>(5.0, 0.0);

    p1->set_fixed(true);
    p2->set_fixed(true);

    Solver solver;
    solver.add_point(p1);
    solver.add_point(p2);

    // Conflict: p1 and p2 are 5 apart, but we want them 3 apart
    solver.add_constraint(std::make_unique<DistanceConstraint>(p1, p2, 3.0));

    SolveResult result = solver.solve();
    std::cout << result.to_string();
    std::cout << "\n  (Expected: solver does NOT converge because both points are fixed\n"
              << "   at distance 5, but we require distance 3 — conflict!)\n";

    solver.print_error_breakdown(std::cout);
}

// ── Main ──────────────────────────────────────────────────────────

int main() {
    std::cout << R"(
  ╔═══════════════════════════════════════════════════╗
  ║   2D Geometric Constraint Solver v1.0             ║
  ║   Iterative Relaxation (Gauss-Seidel)             ║
  ╚═══════════════════════════════════════════════════╝
)" << std::endl;

    scenario_triangle();
    scenario_rectangle();
    scenario_pendulum();
    scenario_angle();
    scenario_coincident();
    scenario_overconstrained();

    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "  All scenarios completed.\n";
    std::cout << std::string(60, '=') << "\n";

    return 0;
}

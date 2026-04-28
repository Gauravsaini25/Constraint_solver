#include "solver.h"
#include "solver_config.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace cs {

std::string SolveResult::to_string() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(10);
    oss << "=== SOLVER RESULT ===\n";
    oss << "Converged:   " << (converged ? "YES" : "NO") << "\n";
    oss << "Iterations:  " << iterations_used << "\n";
    oss << "Final Error: " << final_error << "\n";
    return oss.str();
}

Solver::Solver(SolverConfig config) : config_(config) {}

void Solver::add_point(std::shared_ptr<Point> point) {
    points_.push_back(std::move(point));
}

void Solver::remove_point(uint32_t point_id) {
    constraints_.erase(
        std::remove_if(constraints_.begin(), constraints_.end(),
            [point_id](const std::unique_ptr<Constraint>& c) {
                for (const auto& p : c->involved_points()) {
                    if (p->id() == point_id) return true;
                }
                return false;
            }),
        constraints_.end());
    points_.erase(
        std::remove_if(points_.begin(), points_.end(),
            [point_id](const std::shared_ptr<Point>& p) {
                return p->id() == point_id;
            }),
        points_.end());
}

void Solver::add_constraint(std::unique_ptr<Constraint> constraint) {
    constraints_.push_back(std::move(constraint));
}

void Solver::remove_constraint(size_t index) {
    if (index < constraints_.size()) {
        constraints_.erase(constraints_.begin() + static_cast<ptrdiff_t>(index));
    }
}

SolveResult Solver::solve() {
    SolveResult result;
    if (constraints_.empty()) { result.converged = true; return result; }

    for (uint32_t iter = 0; iter < config_.max_iterations; ++iter) {
        double total_sq = 0.0;
        for (auto& c : constraints_) {
            double orig = c->stiffness;
            c->stiffness *= config_.damping_factor;
            double r = c->apply();
            total_sq += r * r;
            c->stiffness = orig;
        }
        double rms = std::sqrt(total_sq / static_cast<double>(constraints_.size()));
        if (config_.record_error_history) result.error_history.push_back(rms);
        if (iteration_callback_) iteration_callback_(iter, rms);
        if (rms < config_.tolerance) {
            result.converged = true;
            result.iterations_used = iter + 1;
            result.final_error = rms;
            return result;
        }
    }
    result.converged = false;
    result.iterations_used = config_.max_iterations;
    result.final_error = compute_total_error();
    return result;
}

double Solver::compute_total_error() const {
    if (constraints_.empty()) return 0.0;
    double total = 0.0;
    for (const auto& c : constraints_) { double e = c->error(); total += e * e; }
    return std::sqrt(total / static_cast<double>(constraints_.size()));
}

void Solver::reset_points() {
    for (auto& p : points_) p->reset();
}

void Solver::print_state(std::ostream& os) const {
    os << "\n=== SYSTEM STATE ===\nPoints:\n";
    for (const auto& p : points_) os << "  " << p->to_string() << "\n";
    os << "Constraints:\n";
    for (const auto& c : constraints_) os << "  " << c->name() << "\n";
}

void Solver::print_error_breakdown(std::ostream& os) const {
    os << std::fixed << std::setprecision(10);
    os << "\n=== CONSTRAINT ERROR BREAKDOWN ===\n";
    for (const auto& c : constraints_)
        os << "  " << c->name() << "  err=" << c->error() << "\n";
    os << "RMS Error: " << compute_total_error() << "\n";
}

void Solver::set_iteration_callback(IterationCallback callback) {
    iteration_callback_ = std::move(callback);
}

} // namespace cs

#ifndef CURVE_H
#define CURVE_H

#pragma once
#include <vector>
#include <map>
#include <string>

namespace terraingenerator {
    template< std::floating_point T>
    class Curve {
    private:
        std::vector<T> coefficients_{};
        std::vector<T> points_x_;
        std::map<T, T> points_;
        bool seted_coefficients_ = true;

        std::vector<T> get_points_x(std::vector<T>& points_y) {
            std::vector<T> points_x{};
            for (auto point: points_) {
                points_x.push_back(point.first);
                points_y.push_back(point.second);
            }
            return points_x;
        }
        void set_coefficients() {
            int n = static_cast<int>(points_.size());
            coefficients_.clear();
            points_x_ = get_points_x(coefficients_);
            for (int j = 1; j < n; ++j) {
                for (int i = n - 1; i >= j; --i) {
                    coefficients_[i] = (coefficients_[i] - coefficients_[i - 1]) / (points_x_[i] - points_x_[i - j]);
                }
            }
            seted_coefficients_ = true;
        }

        void add_edge_points() {
            points_.insert({0, 0});
            points_.insert({1, 1});
        }

    public:
        Curve() {
            add_edge_points();
            set_coefficients();
        }

        void add_point(T x, T y) {
            if (points_.contains(x)) {
                points_[x] = y;
            } else {
                points_.insert({x, y});
            }
            seted_coefficients_ = false;
        }

        T calculate(T x) {
            if (!seted_coefficients_) {
                set_coefficients();
            }
            size_t n = coefficients_.size();
            T result = coefficients_[n - 1];
            for (size_t i = 1; i <= n - 1; ++i) {
                result = result * (x - points_x_[n - i]) + coefficients_[n - i];
            }
            return result;
        }

        void clear() {
            coefficients_.clear();
            add_edge_points();
            set_coefficients();
        }

        operator std::string() const {
            std::string result;
            for (auto point: points_) {
                result += std::to_string(point.first) + " " + std::to_string(point.second) + "\n";
            }
            return result;
        }
    };
}
#endif // CURVE_H

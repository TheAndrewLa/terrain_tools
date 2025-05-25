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
            size_t n = points_.size();
            std::vector<T> points_y{};
            points_x_ = get_points_x(points_y);
            coefficients_ = points_y;
            for (size_t j = 1; j < n; ++j) {
                for (size_t i = n - 1; i >= j; --i) {
                    coefficients_[i] = (coefficients_[i] - coefficients_[i - 1]) /
                                       (points_x_[i] - points_x_[i - j]);
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
            points_[x] = y;
            seted_coefficients_ = false;
        }

        T calculate(T x) {
            if (!seted_coefficients_) {
                set_coefficients();
            }
            size_t n = coefficients_.size();
            T result = coefficients_[n - 1];
            for (size_t i = 1; i < n; ++i) {
                result = result * (x - points_x_[n - i - 1]) + coefficients_[n - i - 1];
            }
            if (result < 0) {
                return 0;
            }
            if (result > 1) {
                return 1;
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

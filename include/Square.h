#pragma once

#include <array>
#include <cmath>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include "./Figure.h"

template <Scalar T>
class Square : public Figure<T> {
private:
    static constexpr std::size_t vertex_count = 4;
    std::array<std::unique_ptr<Point<T>>, vertex_count> points_;

    void ensure_storage() {
        for (auto& point : points_) {
            if (!point) {
                point = std::make_unique<Point<T>>();
            }
        }
    }

    void copy_from(const Square& other) {
        this->name_fig = other.name_fig;
        for (std::size_t i = 0; i < vertex_count; ++i) {
            if (other.points_[i]) {
                if (!points_[i]) {
                    points_[i] = std::make_unique<Point<T>>();
                }
                *points_[i] = *other.points_[i];
            } else {
                points_[i].reset();
            }
        }
    }

    static double polygon_area(const std::array<std::unique_ptr<Point<T>>, vertex_count>& pts) {
        double area = 0.0;
        for (std::size_t i = 0; i < vertex_count; ++i) {
            const auto& current = *pts[i];
            const auto& next = *pts[(i + 1) % vertex_count];
            area += static_cast<double>(current.x) * static_cast<double>(next.y)
                  - static_cast<double>(next.x) * static_cast<double>(current.y);
        }
        return std::abs(area) / 2.0;
    }

public:
    Square() : Figure<T>("Square") {
        ensure_storage();
    }

    explicit Square(std::string name) : Figure<T>(std::move(name)) {
        ensure_storage();
    }

    Square(std::initializer_list<Point<T>> pointList) : Figure<T>("Square") {
        if (pointList.size() != vertex_count) {
            throw std::invalid_argument("Square requires exactly 4 points");
        }
        ensure_storage();
        std::size_t index = 0;
        for (const auto& point : pointList) {
            *points_[index++] = point;
        }
    }

    Square(const Square& other) : Figure<T>(other) {
        ensure_storage();
        copy_from(other);
    }

    Square(Square&& other) noexcept = default;

    Square& operator=(const Square& other) {
        if (this != &other) {
            Figure<T>::operator=(other);
            copy_from(other);
        }
        return *this;
    }

    Square& operator=(Square&& other) noexcept = default;

    ~Square() override = default;

    Point<T> center_figure() const override {
        T x_sum{};
        T y_sum{};
        for (const auto& point : points_) {
            x_sum += point->x;
            y_sum += point->y;
        }
        return Point<T>(x_sum / static_cast<T>(vertex_count), y_sum / static_cast<T>(vertex_count));
    }

    void write_figure(std::ostream& os) const override {
        for (std::size_t i = 0; i < vertex_count; ++i) {
            os << "Точка " << static_cast<char>('A' + static_cast<char>(i)) << ": " << *points_[i] << '\n';
        }
        os << "Центр: " << center_figure() << '\n';
        os << "Площадь квадрата: " << area_figure();
    }

    void read_figure(std::istream& is) override {
        ensure_storage();
        for (std::size_t i = 0; i < vertex_count; ++i) {
            Point<T> inputPoint;
            is >> inputPoint;
            if (!is) {
                break;
            }
            *points_[i] = inputPoint;
        }
    }

    double area_figure() const override {
        return polygon_area(points_);
    }

    std::unique_ptr<Figure<T>> clone() const override {
        return std::make_unique<Square>(*this);
    }

    bool operator==(const Square& other) const {
        if (this->name_fig != other.name_fig) {
            return false;
        }
        for (std::size_t i = 0; i < vertex_count; ++i) {
            if (*points_[i] != *other.points_[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Square& other) const {
        return !(*this == other);
    }
};
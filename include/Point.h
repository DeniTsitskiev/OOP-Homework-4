#pragma once

#include <concepts>
#include <iostream>
#include <type_traits>

template <typename T>
concept Scalar = std::is_scalar_v<T>;

template <Scalar T>
struct Point {
    T x{};
    T y{};

    Point() = default;
    Point(T xValue, T yValue) : x(xValue), y(yValue) {}
};

template <Scalar T>
bool operator==(const Point<T>& lhs, const Point<T>& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

template <Scalar T>
bool operator!=(const Point<T>& lhs, const Point<T>& rhs) {
    return !(lhs == rhs);
}

template <Scalar T>
std::ostream& operator<<(std::ostream& os, const Point<T>& p) {
    os << '(' << p.x << ", " << p.y << ')';
    return os;
}

template <Scalar T>
std::istream& operator>>(std::istream& is, Point<T>& p) {
    char ch{};
    is >> std::ws >> ch;
    if (ch != '(') {
        is.setstate(std::ios::failbit);
        return is;
    }

    is >> p.x;
    is >> std::ws >> ch;
    if (ch != ',') {
        is.setstate(std::ios::failbit);
        return is;
    }

    is >> p.y;
    is >> std::ws >> ch;
    if (ch != ')') {
        is.setstate(std::ios::failbit);
    }
    return is;
}
#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "./Point.h"

template <Scalar T>
class Figure {
protected:
    std::string name_fig;

public:
    Figure() : name_fig("Figure") {}
    explicit Figure(std::string name) : name_fig(std::move(name)) {}
    Figure(const Figure&) = default;
    Figure(Figure&&) noexcept = default;
    Figure& operator=(const Figure&) = default;
    Figure& operator=(Figure&&) noexcept = default;
    virtual ~Figure() = default;

    virtual Point<T> center_figure() const = 0;
    virtual void write_figure(std::ostream& os) const = 0;
    virtual void read_figure(std::istream& is) = 0;
    virtual double area_figure() const = 0;
    virtual std::unique_ptr<Figure<T>> clone() const = 0;

    operator double() const { return area_figure(); }

    const std::string& name() const noexcept { return name_fig; }
};

template <Scalar T>
std::ostream& operator<<(std::ostream& os, const Figure<T>& figure) {
    figure.write_figure(os);
    return os;
}

template <Scalar T>
std::istream& operator>>(std::istream& is, Figure<T>& figure) {
    figure.read_figure(is);
    return is;
}
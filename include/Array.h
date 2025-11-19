#pragma once

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

template <class T>
class Array;

namespace detail {

template <class T>
inline constexpr bool dependent_false_v = false;

template <class T>
concept Dereferenceable = requires(T value) {
    *value;
};

template <class T>
decltype(auto) to_reference(T& value) {
    if constexpr (std::is_pointer_v<std::remove_reference_t<T>>) {
        return *value;
    } else if constexpr (Dereferenceable<T>) {
        return *value;
    } else {
        return (value);
    }
}

template <class T>
double figure_area(const T& value) {
    const auto& ref = to_reference(const_cast<T&>(value));
    if constexpr (requires { static_cast<double>(ref); }) {
        return static_cast<double>(ref);
    } else if constexpr (requires { ref.area_figure(); }) {
        return static_cast<double>(ref.area_figure());
    } else {
        static_assert(dependent_false_v<T>, "Type does not provide area");
    }
}

template <class T>
void stream_out(std::ostream& os, const T& value) {
    const auto& ref = to_reference(const_cast<T&>(value));
    if constexpr (requires { os << ref; }) {
        os << ref;
    } else {
        static_assert(dependent_false_v<T>, "Type is not streamable");
    }
}

} // namespace detail

template <class T>
class Array {
private:
    std::shared_ptr<T[]> data_;
    std::size_t size_{0};
    std::size_t capacity_{0};

    static std::shared_ptr<T[]> allocate(std::size_t count) {
        if (count == 0) {
            return {};
        }
        return std::shared_ptr<T[]>(new T[count]);
    }

    void reallocate(std::size_t new_capacity) {
        if (new_capacity == capacity_) {
            return;
        }
        auto new_data = allocate(new_capacity);
        for (std::size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[i]);
        }
        data_ = std::move(new_data);
        capacity_ = new_capacity;
    }

    void ensure_capacity(std::size_t desired) {
        if (desired <= capacity_) {
            return;
        }
        std::size_t new_capacity = capacity_ == 0 ? 1 : capacity_;
        while (new_capacity < desired) {
            new_capacity *= 2;
        }
        reallocate(new_capacity);
    }

public:
    Array() = default;

    explicit Array(std::size_t capacity)
        : data_(allocate(capacity)), size_(0), capacity_(capacity) {}

    Array(std::initializer_list<T> init)
        : data_(allocate(init.size())), size_(init.size()), capacity_(init.size()) {
        std::size_t index = 0;
        for (const auto& value : init) {
            data_[index++] = value;
        }
    }

    Array(const Array& other)
        : data_(allocate(other.capacity_)), size_(other.size_), capacity_(other.capacity_) {
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    Array(Array&& other) noexcept = default;

    Array& operator=(const Array& other) {
        if (this != &other) {
            Array temp(other);
            swap(*this, temp);
        }
        return *this;
    }

    Array& operator=(Array&& other) noexcept = default;

    friend void swap(Array& lhs, Array& rhs) noexcept {
        using std::swap;
        swap(lhs.data_, rhs.data_);
        swap(lhs.size_, rhs.size_);
        swap(lhs.capacity_, rhs.capacity_);
    }

    bool operator==(const Array& other) const {
        if (size_ != other.size_) {
            return false;
        }
        for (std::size_t i = 0; i < size_; ++i) {
            if (!(data_[i] == other.data_[i])) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Array& other) const {
        return !(*this == other);
    }

    std::size_t getSize() const noexcept {
        return size_;
    }

    std::size_t getCapacity() const noexcept {
        return capacity_;
    }

    bool empty() const noexcept {
        return size_ == 0;
    }

    void addFigure(const T& value) {
        ensure_capacity(size_ + 1);
        data_[size_++] = value;
    }

    void addFigure(T&& value) {
        ensure_capacity(size_ + 1);
        data_[size_++] = std::move(value);
    }

    void deleteFigure(std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        for (std::size_t i = index; i + 1 < size_; ++i) {
            data_[i] = std::move(data_[i + 1]);
        }
        --size_;
    }

    T& getFigure(std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    const T& getFigure(std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    T* begin() noexcept {
        return data_.get();
    }

    const T* begin() const noexcept {
        return data_.get();
    }

    T* end() noexcept {
        return data_.get() + size_;
    }

    const T* end() const noexcept {
        return data_.get() + size_;
    }

    double totalArea() const {
        double total = 0.0;
        for (std::size_t i = 0; i < size_; ++i) {
            total += detail::figure_area(data_[i]);
        }
        return total;
    }

    void printAll() const {
        for (std::size_t i = 0; i < size_; ++i) {
            detail::stream_out(std::cout, data_[i]);
            std::cout << '\n';
        }
    }
};

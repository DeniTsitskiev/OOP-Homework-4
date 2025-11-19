#include <gtest/gtest.h>
#include "Figure.h"
#include "Octagon.h"
#include "Point.h"
#include "Square.h"
#include "Triangle.h"

// Тесты для Point
TEST(PointTest, DefaultConstructor) {
    Point<double> p;
    EXPECT_DOUBLE_EQ(p.x, 0);
    EXPECT_DOUBLE_EQ(p.y, 0);
}

TEST(PointTest, ParameterizedConstructor) {
    Point<double> p(3.5, 2.8);
    EXPECT_DOUBLE_EQ(p.x, 3.5);
    EXPECT_DOUBLE_EQ(p.y, 2.8);
}

TEST(PointTest, InputOutputOperator) {
    Point<double> p1(1.5, 2.5);
    std::stringstream ss;
    ss << p1;
    EXPECT_EQ(ss.str(), "(1.5, 2.5)");
    
    Point<double> p2;
    ss >> p2;
    EXPECT_DOUBLE_EQ(p2.x, 1.5);
    EXPECT_DOUBLE_EQ(p2.y, 2.5);
}

// Тесты для Square
TEST(SquareTest, DefaultConstructor) {
    Square<double> square;
    EXPECT_NO_THROW(square.area_figure());
}

TEST(SquareTest, InitializerListConstructor) {
    Square<double> square({Point<double>(0,0), Point<double>(1,0), Point<double>(1,1), Point<double>(0,1)});
    EXPECT_DOUBLE_EQ(square.area_figure(), 1.0);
}

TEST(SquareTest, CopyConstructor) {
    Square<double> square1({Point<double>(0,0), Point<double>(2,0), Point<double>(2,2), Point<double>(0,2)});
    Square<double> square2 = square1;
    EXPECT_TRUE(square1 == square2);
}

TEST(SquareTest, MoveConstructor) {
    Square<double> square1({Point<double>(0,0), Point<double>(1,0), Point<double>(1,1), Point<double>(0,1)});
    Square<double> square2 = std::move(square1);
    EXPECT_DOUBLE_EQ(square2.area_figure(), 1.0);
}

TEST(SquareTest, AreaCalculation) {
    Square<double> square({Point<double>(0,0), Point<double>(3,0), Point<double>(3,3), Point<double>(0,3)});
    EXPECT_DOUBLE_EQ(square.area_figure(), 9.0);
}

TEST(SquareTest, CenterCalculation) {
    Square<double> square({Point<double>(1,1), Point<double>(3,1), Point<double>(3,3), Point<double>(1,3)});
    Point<double> center = square.center_figure();
    EXPECT_DOUBLE_EQ(center.x, 2.0);
    EXPECT_DOUBLE_EQ(center.y, 2.0);
}

// Тесты для Triangle
TEST(TriangleTest, DefaultConstructor) {
    Triangle<double> triangle;
    EXPECT_NO_THROW(triangle.area_figure());
}

TEST(TriangleTest, EquilateralTriangleArea) {
    Triangle<double> triangle({Point<double>(0,0), Point<double>(2,0), Point<double>(1,1.732)});
    EXPECT_NEAR(triangle.area_figure(), 1.732, 0.01);
}

TEST(TriangleTest, CopyAssignment) {
    Triangle<double> triangle1({Point<double>(0,0), Point<double>(3,0), Point<double>(1.5,2.598)});
    Triangle<double> triangle2;
    triangle2 = triangle1;
    EXPECT_TRUE(triangle1 == triangle2);
}

// Тесты для Octagon
TEST(OctagonTest, DefaultConstructor) {
    Octagon<double> octagon;
    EXPECT_NO_THROW(octagon.area_figure());
}

TEST(OctagonTest, RegularOctagonArea) {
    Octagon<double> octagon({
        Point<double>(1,0), Point<double>(0.707,0.707), Point<double>(0,1), Point<double>(-0.707,0.707),
        Point<double>(-1,0), Point<double>(-0.707,-0.707), Point<double>(0,-1), Point<double>(0.707,-0.707)
    });
    EXPECT_NEAR(octagon.area_figure(), 2.828, 0.01);
}

TEST(OctagonTest, CloneMethod) {
    Octagon<double> octagon1({
        Point<double>(1,0), Point<double>(0.707,0.707), Point<double>(0,1), Point<double>(-0.707,0.707),
        Point<double>(-1,0), Point<double>(-0.707,-0.707), Point<double>(0,-1), Point<double>(0.707,-0.707)
    });
    auto cloned = octagon1.clone();
    EXPECT_NEAR(cloned->area_figure(), 2.828, 0.01);
}
#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>

#include "Array.h"
#include "Octagon.h"
#include "Point.h"
#include "Square.h"
#include "Triangle.h"

TEST(ArrayTest, DefaultConstructor) {
    Array<Square<double>> array;
    EXPECT_EQ(array.getSize(), 0);
    EXPECT_EQ(array.getCapacity(), 0);
}

TEST(ArrayTest, AddFigures) {
    Array<std::shared_ptr<Figure<double>>> array;
    array.addFigure(std::make_shared<Square<double>>(std::initializer_list<Point<double>>{
        Point<double>(0,0), Point<double>(1,0), Point<double>(1,1), Point<double>(0,1)}));
    array.addFigure(std::make_shared<Triangle<double>>(std::initializer_list<Point<double>>{
        Point<double>(0,0), Point<double>(2,0), Point<double>(1,1.732)}));
    
    EXPECT_EQ(array.getSize(), 2);
}

TEST(ArrayTest, TotalAreaCalculation) {
    Array<std::shared_ptr<Figure<double>>> array;
    array.addFigure(std::make_shared<Square<double>>(std::initializer_list<Point<double>>{
        Point<double>(0,0), Point<double>(2,0), Point<double>(2,2), Point<double>(0,2)}));
    array.addFigure(std::make_shared<Triangle<double>>(std::initializer_list<Point<double>>{
        Point<double>(0,0), Point<double>(3,0), Point<double>(1.5,2.598)}));
    
    EXPECT_NEAR(array.totalArea(), 7.897, 0.01);
}

TEST(ArrayTest, DeleteFigure) {
    Array<Square<double>> array;
    array.addFigure(Square<double>());
    array.addFigure(Square<double>({Point<double>(0,0), Point<double>(1,0), Point<double>(1,1), Point<double>(0,1)}));
    array.addFigure(Square<double>({Point<double>(1,1), Point<double>(2,1), Point<double>(2,2), Point<double>(1,2)}));
    
    EXPECT_EQ(array.getSize(), 3);
    array.deleteFigure(1);
    EXPECT_EQ(array.getSize(), 2);
}

TEST(ArrayTest, CopyConstructor) {
    Array<std::shared_ptr<Figure<double>>> array1;
    array1.addFigure(std::make_shared<Square<double>>(std::initializer_list<Point<double>>{
        Point<double>(0,0), Point<double>(1,0), Point<double>(1,1), Point<double>(0,1)}));
    
    Array<std::shared_ptr<Figure<double>>> array2 = array1;
    EXPECT_EQ(array1.getSize(), array2.getSize());
    EXPECT_NEAR(array1.totalArea(), array2.totalArea(), 0.001);
}

TEST(ArrayTest, AssignmentOperator) {
    Array<Square<double>> array1;
    array1.addFigure(Square<double>({Point<double>(0,0), Point<double>(2,0), Point<double>(2,2), Point<double>(0,2)}));
    
    Array<Square<double>> array2;
    array2 = array1;
    EXPECT_EQ(array1.getSize(), array2.getSize());
    EXPECT_NEAR(array1.totalArea(), array2.totalArea(), 0.001);
}

TEST(ArrayTest, OutOfRangeAccess) {
    Array<Square<double>> array;
    EXPECT_THROW(array.getFigure(0), std::out_of_range);
    EXPECT_THROW(array.deleteFigure(0), std::out_of_range);
}
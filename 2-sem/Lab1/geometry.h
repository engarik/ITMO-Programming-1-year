#include <cstdlib>
#include <iostream>
#include <cassert>

#ifndef LAB1_GEOMETRY_H
#define LAB1_GEOMETRY_H

using namespace std;

const double EPS = 1e-5;

class Point {
public:
    // default constructor
    explicit Point(int x = 0, int y = 0) : x(x), y(y) {}

    // copy constructor
    Point(const Point &other) = default;

    // destructor
    ~Point() = default;

    // copy assign operator
    Point &operator=(const Point &other) = default;

    // perimeter or area can't be computed

    int getX() const;

    int getY() const;

    friend ostream &operator<<(ostream &stream, const Point &point);

private:
    int x, y;
};

ostream &operator<<(ostream &stream, const Point &point);

double getDistanceBetweenPoints(const Point &p1, const Point &p2);

int productSign(const Point &a, const Point &b, const Point &c);

int productSign(const Point &a, const Point &b, const Point &c, const Point &d);

double angle(const Point &a, const Point &b, const Point &c);

class PolygonalChain {
public:
    // default constructor
    explicit PolygonalChain(size_t numberOfPoints, const Point * points)
            : numberOfPoints(numberOfPoints), points(new Point[numberOfPoints])
    {
        for (size_t i = 0; i < numberOfPoints; ++i) {
            this->points[i] = points[i];
        }
    }

    // copy constructor
    PolygonalChain(const PolygonalChain &other)
            : numberOfPoints(other.numberOfPoints), points(new Point[numberOfPoints])
    {
        for (size_t i = 0; i < numberOfPoints; ++i) {
            points[i] = other.points[i];
        }
    }

    // copy assign operator
    PolygonalChain &operator=(const PolygonalChain &other)
    {
        if (this != &other) {
            delete [] points;
            numberOfPoints = other.numberOfPoints;
            points = new Point[numberOfPoints];
            for (size_t i = 0; i < numberOfPoints; ++i) {
                points[i] = other.points[i];
            }
        }

        return *this;
    }

    // destructor
    ~PolygonalChain() { delete [] points; }

    // other methods
    size_t getNumberOfPoints() const;

    virtual double getPerimeter() const;

    const Point &getPoint(size_t point) const;

    friend ostream &operator<<(ostream &stream, const PolygonalChain &polygonalChain);

private:
    size_t numberOfPoints;
    Point * points;
};

ostream &operator<<(ostream &stream, const PolygonalChain &polygonalChain);

class ClosedPolygonalChain : public PolygonalChain {
public:
    explicit ClosedPolygonalChain(size_t numberOfPoints, const Point * points)
            : PolygonalChain(numberOfPoints, points)
    {}

    double getPerimeter() const override;

};

class Polygon : public ClosedPolygonalChain {
public:
    explicit Polygon(size_t numberOfPoints, const Point * points)
            : ClosedPolygonalChain(numberOfPoints, points)
    {
        assert(isConvex());
    }

    virtual double getArea() const;

private:
    bool isConvex() const;
};

class Triangle : public Polygon {
public:
    explicit Triangle(const Point * points)
            : Polygon(3, points)
    {}

    bool hasRightAngle() const;

    double getArea() const override;
};

class Trapezoid : public Polygon {
public:
    explicit Trapezoid(const Point * points)
            : Polygon(4, points)
    {
        assert(isTrapezoid());
    }

    double getHeight() const;

    double getArea() const override;
private:
    bool isTrapezoid() const;
};

class RegularPolygon : public Polygon {
public:
    explicit RegularPolygon(const Point * points)
            : Polygon(4, points)
    {
        assert(isRegularPolygon());
    }

    double getArea() const override;

private:
    bool isRegularPolygon() const;
};

#endif

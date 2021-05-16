#include <iostream>
#include <cmath>

#include "geometry.h"

using namespace std;

int Point::getX() const { return x;}

int Point::getY() const { return y;}

size_t PolygonalChain::getNumberOfPoints() const { return numberOfPoints; }

const Point & PolygonalChain::getPoint(size_t point) const {
    assert(point >= 0 && point < numberOfPoints);
    return points[point];
}

double PolygonalChain::getPerimeter() const {
    double perimeter = 0;
    for (size_t i = 0; i < numberOfPoints - 1; i++) {
        perimeter += getDistanceBetweenPoints(points[i], points[i + 1]);
    }
    return perimeter;
}

double ClosedPolygonalChain::getPerimeter() const {
    double perimeter = 0;
    for (size_t i = 0; i < getNumberOfPoints(); i++) {
        perimeter += getDistanceBetweenPoints(getPoint(i), getPoint((i + 1) % getNumberOfPoints()));
    }
    return perimeter;
}

double Polygon::getArea() const {
    cout << "Polygons' area function called\n";
    double area = 0;

    for (size_t i = 0; i < getNumberOfPoints(); ++i) {
        area += getPoint(i).getX() * getPoint((i + 1) % getNumberOfPoints()).getY();
        area -= getPoint((i + 1) % getNumberOfPoints()).getX() * getPoint(i).getY();
    }

    return abs(area) * 0.5;
}

bool Polygon::isConvex() const {
    int prevSign = 0;
    for (size_t i = 1; i < getNumberOfPoints() + 1; ++i) {
        Point a = getPoint((i - 1) % getNumberOfPoints());
        Point b = getPoint(i % getNumberOfPoints());
        Point c = getPoint((i + 1) % getNumberOfPoints());

        int sign = productSign(a, b, c);

        if (sign != 0) {
            if (prevSign == 0) {
                prevSign = sign;
            } else if (prevSign != sign) {
                return false;
            }
        }
    }

    return true;
}

bool Triangle::hasRightAngle() const {
    double a1 = angle(getPoint(0), getPoint(1), getPoint(2));
    double a2 = angle(getPoint(1), getPoint(2), getPoint(0));
    double a3 = angle(getPoint(2), getPoint(0), getPoint(1));

    return (abs(a1 - 1.5708) <= EPS) || (abs(a2 - 1.5708) <= EPS) || (abs(a3 - 1.5708) <= EPS);
}

double Triangle::getArea() const {
    cout << "Triangles' area function called\n";
    double a = getDistanceBetweenPoints(getPoint(0), getPoint(1));
    double b = getDistanceBetweenPoints(getPoint(1), getPoint(2));
    double c = getDistanceBetweenPoints(getPoint(2), getPoint(0));
    double p = (a + b + c) / 2;

    return sqrt(p * (p - a) * (p - b) * (p - c));
}

double Trapezoid::getHeight() const {
    double a = getDistanceBetweenPoints(getPoint(1), getPoint(2));
    double b = getDistanceBetweenPoints(getPoint(3), getPoint(0));
    double c = getDistanceBetweenPoints(getPoint(0), getPoint(1));
    double d = getDistanceBetweenPoints(getPoint(2), getPoint(3));
    return sqrt(c * c - pow((pow((a - b), 2) + c * c - d * d) / (2 * (a - b)), 2));
}

bool Trapezoid::isTrapezoid() const {
    return productSign(getPoint(1), getPoint(2), getPoint(3), getPoint(0)) == 0;
}

double Trapezoid::getArea() const {
    cout << "Trapezoids' area function called\n";
    double a = getDistanceBetweenPoints(getPoint(1), getPoint(2));
    double b = getDistanceBetweenPoints(getPoint(3), getPoint(0));

    return (a + b) / 2 * getHeight();
}

bool RegularPolygon::isRegularPolygon() const {
    double a1 = angle(getPoint(0), getPoint(1), getPoint(2));
    double a2 = angle(getPoint(1), getPoint(2), getPoint(3));
    double a3 = angle(getPoint(2), getPoint(3), getPoint(0));
    double a4 = angle(getPoint(3), getPoint(0), getPoint(1));

    return (abs(a1 - 1.5708) <= EPS) && (abs(a2 - 1.5708) <= EPS) && (abs(a3 - 1.5708) <= EPS) && (abs(a4 - 1.5708) <= EPS);
}

double RegularPolygon::getArea() const {
    cout << "RegularPolygons' area function called\n";
    double a = getDistanceBetweenPoints(getPoint(0), getPoint(1));
    double b = getDistanceBetweenPoints(getPoint(1), getPoint(2));

    return a * b;
}

ostream &operator<<(ostream &stream, const Point &point) {
    stream << "(" << point.x << ":" << point.y << ")";
    return stream;
}

double getDistanceBetweenPoints(const Point &p1, const Point &p2) {
    return sqrt(pow(p1.getX() - p2.getX(), 2.0) + pow(p1.getY() - p2.getY(), 2.0));
}

ostream &operator<<(ostream &stream, const PolygonalChain &polygonalChain) {
    for (size_t i = 0; i < polygonalChain.numberOfPoints; i++) {
        stream << polygonalChain.points[i] << " ";
    }
    return stream;
}

int sign(double value) {
    if (value > 0) {
        return 1;
    } else if (value == 0) {
        return 0;
    } else {
        return -1;
    }
}

int productSign(const Point &a, const Point &b, const Point &c) {
    Point ab(b.getX() - a.getX(), b.getY() - a.getY());
    Point bc(c.getX() - b.getX(), c.getY() - b.getY());
    double product = ab.getX() * bc.getY() - ab.getY() * bc.getX();

    return sign(product);
}

int productSign(const Point &a, const Point &b, const Point &c, const Point &d) {
    Point ab(b.getX() - a.getX(), b.getY() - a.getY());
    Point cd(c.getX() - d.getX(), c.getY() - d.getY());
    double product = ab.getX() * cd.getY() - ab.getY() * cd.getX();

    return sign(product);
}

double angle(const Point &a, const Point &b, const Point &c) {
    Point ba(a.getX() - b.getX(), a.getY() - b.getY());
    Point bc(c.getX() - b.getX(), c.getY() - b.getY());
    double angle = (ba.getX() * bc.getX() + ba.getY() * bc.getY()) / (getDistanceBetweenPoints(a, b) * getDistanceBetweenPoints(b, c));

    return acos(angle);
}

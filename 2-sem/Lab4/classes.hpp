#include <cmath>

#ifndef LAB4_CLASSES_HPP
#define LAB4_CLASSES_HPP

class Point {
public:
    Point(double x = 0, double y = 0) : x(x), y(y) {}

    double getDist() const { return sqrt(x * x + y * y); }

    bool operator==(const Point& rhs) const {
        return getDist() == rhs.getDist();
    }

    bool operator!=(const Point& rhs) const {
        return getDist() != rhs.getDist();
    }

    bool operator<(const Point& rhs) const {
        return getDist() < rhs.getDist();
    }

    bool operator>(const Point& rhs) const {
        return getDist() > rhs.getDist();
    }

    bool operator<=(const Point& rhs) const {
        return getDist() <= rhs.getDist();
    }

    bool operator>=(const Point& rhs) const {
        return getDist() >= rhs.getDist();
    }
private:
    double x, y;
};

class Human {
public:
    explicit Human(unsigned height = 0, unsigned age = 0) : height(height), age(age) {}

    bool operator==(const Human& rhs) const {
        return height == rhs.height && age == rhs.age;
    }

    bool operator!=(const Human& rhs) const {
        return !((*this) == rhs);
    }

    bool operator<(const Human& rhs) const {
        return height * age < rhs.height * rhs.age;
    }

    bool operator>(const Human& rhs) const {
        return height * age > rhs.height * rhs.age;
    }

    bool operator<=(const Human& rhs) const {
        return height * age <= rhs.height * rhs.age;
    }

    bool operator>=(const Human& rhs) const {
        return height * age >= rhs.height * rhs.age;
    }
private:
    unsigned height, age;
};

#endif

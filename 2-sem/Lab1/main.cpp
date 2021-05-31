#include "geometry.h"
#include <iostream>
#include <cassert>

using namespace std;

void PointCheck() {
    const Point p1;
    assert(p1.getX() == 0 && p1.getY() == 0);
    const Point p2(3, 4);
    assert(p2.getX() == 3 && p2.getY() == 4);
}

void InheritancePolygonalChainCheck(const PolygonalChain *pc, const PolygonalChain *cpc) {
    assert(pc->getPerimeter() == 9);
    assert(cpc->getPerimeter() == 12);
}

void PolygonsCheck() {
    Point *a = new Point[3]{Point(0, 0), Point(3, 4), Point(3, 0)};
    Point *b = new Point[4]{Point(0, 0), Point(1, 4), Point(3, 4), Point(3, 0)};

    const Polygon p(3, a);
    const Triangle t(a);
    const Trapezoid tr(b);
    const PolygonalChain pc(3, a);
    assert(pc.getNumberOfPoints() == 3 && pc.getPoint(1).getX() == 3 && pc.getPoint(1).getY() == 4);
    assert(pc.getPerimeter() == 9);
    const ClosedPolygonalChain cpc(3, a);
    a[1] = Point();
    assert(cpc.getNumberOfPoints() == 3 && cpc.getPoint(1).getX() == 3 && cpc.getPoint(1).getY() == 4);
    assert(cpc.getPerimeter() == 12);

    InheritancePolygonalChainCheck(&pc, &cpc);

    assert(p.getArea() == 6);
    assert(abs(tr.getHeight() - 4) < EPS);
    assert(t.hasRightAngle());
    Triangle trSharpestAsMyHand(new Point[3]{Point(0, 0), Point(1, 1), Point(0, 100)});
    assert(!trSharpestAsMyHand.hasRightAngle());
    RegularPolygon rp(new Point[4]{Point(0, 0), Point(0, 2), Point(2, 2), Point(2, 0)});
    assert(abs(rp.getArea() - 4) < EPS && abs(rp.getPerimeter() - 8) < EPS);
    Trapezoid tra(b);
    tra = tr;
    assert(abs(tra.getHeight() - 4) < EPS);
    Trapezoid* trap = new Trapezoid(b);
    Trapezoid trCopy(*trap);
    delete trap;
    assert(abs(trCopy.getHeight() - 4) < EPS);
    Triangle floatArea(new Point[3]{Point(0, 0), Point(1, 1), Point(0, 1)});
    assert(abs(floatArea.getArea() - 0.5) < EPS);

    a[1] = Point(3, 4);

    cout << "\n---Dynamic polymorphism---\n" << endl;

    Polygon ** array = new Polygon*[4];
    array[0] = new Polygon(3, a);
    array[1] = new Triangle(a);
    array[2] = new Trapezoid(b);
    array[3] = new RegularPolygon(new Point[4]{Point(0, 0), Point(0, 2), Point(2, 2), Point(2, 0)});

    for (int i = 0; i < 4; i++) {
        cout << array[i]->getArea() << endl;
    }
}

int main() {
    PointCheck();
    PolygonsCheck();
}

#include "predicate.hpp"
#include "classes.hpp"
#include <sstream>
#include <cassert>
#include <vector>
#include <set>
#include <functional>

using namespace std;

void defaultCheck() {
    vector<int> a{1, 2, 3, 4};
    vector<int> b{1, 2, 3, 4, 2};
    vector<int> c{4, 3, 2, 1, 0};
    vector<int> d{1, 1};
    vector<int> e{1, 2, 3, 4, 3, 2, 1};
    vector<int> f{1, 2, 3, 4, 3, 2, 3};
    vector<int> e1{1, 2, 3, 3, 2, 1};
    vector<int> f1{1, 2, 3, 4, 2, 1};
    vector<pair<int ,int>> h{{1, 2}, {2, 3}, {3, 4}};


    vector<int> g{1};
    set<int> s{1, 2, 3};
    assert(allOf(a.begin(), a.end(), [](int x) { return x <= 5;}));
    assert(!allOf(a.begin(), a.end(), [](int x){return x <= 3;}));
    assert(anyOf(a.begin(), a.end(), [](int x){return x == 3;}));
    assert(!anyOf(a.begin(), a.end(), [](int x){return x == 5;}));
    assert(noneOf(a.begin(), a.end(), [](int x){return x > 5;}));
    assert(!noneOf(a.begin(), a.end(), [](int x){return x >= 3;}));
    assert(oneOf(b.begin(), b.end(), [](int x){return x == 3;}));
    assert(!oneOf(b.begin(), b.end(), [](int x){return x == 2;}));
    assert(isSorted(a.begin(), a.end(), [](int x, int y){return x < y;}));
    assert(!isSorted(b.begin(), b.end(), [](int x, int y){return x < y;}));
    assert(isSorted(a.begin(), a.end()));
    assert(!isSorted(b.begin(), b.end()));
    assert(isSorted(c.begin(), c.end(), [](int x, int y){return x > y;}));
    assert(isPartitioned(a.begin(), a.end(), [](int x){return x <= 3;}));
    assert(!isPartitioned(a.begin(), a.end(), [](int x){return x % 2 == 0;}));
    assert(isPartitioned(a.begin(), a.end(), [](int x){return x <= 3;}));
    assert(findNot(a.begin(), a.end(), 1) == ++a.begin());
    assert(findNot(d.begin(), d.end(), 1) == d.end());
    assert(findBackwards(b.begin(), b.end(), 2) == --b.end());
    assert(findBackwards(b.begin(), b.end(), 5) == b.end());
    assert(isPalindrome(e.begin(), e.end(), [](int x, int y){return x == y;}));
    assert(!isPalindrome(f.begin(), f.end(), [](int x, int y){return x == y;}));
    assert(isPalindrome(e1.begin(), e1.end(), [](int x, int y){return x == y;}));
    assert(!isPalindrome(f1.begin(), f1.end(), [](int x, int y){return x == y;}));
    assert(isPalindrome(s.begin(), s.end(), [](int x, int y){return x + y > 0;}));
    assert(isPalindrome(g.begin(), g.end(), [](int x, int y){return x == y;}));
    assert(isSorted(h.begin(), h.end(), [](pair<int, int> x, pair<int, int> y) {return x < y;}));
    assert(isSorted(h.begin(), h.end()));
}

void customCheck() {
    Point a;
    Point b(1, 2);
    Point c(2, -3);
    vector<Point> points = {a, b, c};

    assert(allOf(points.begin(), points.end(), [](Point x) { return x <= 100;}));
    assert(anyOf(points.begin(), points.end(), [](Point x){return x == 0;}));
    assert(noneOf(points.begin(), points.end(), [](Point x){return x < 0;}));
    assert(isSorted(points.begin(), points.end()));
    assert(findBackwards(points.begin(), points.end(), Point(2, -3)) == points.end() - 1);

    Human h1(180, 18);
    Human h2(190, 27);
    Human h3(165, 65);

    vector<Human> humans = {h1, h2, h3};
    vector<Human> humans1 = {h1, h1, h1};

    assert(anyOf(humans.begin(), humans.end(), [](Human x){return x == Human(180, 18);}));
    assert(noneOf(humans.begin(), humans.end(), [](Human x){return x == Human(0, 0);}));
    assert(isSorted(humans.begin(), humans.end()));

    assert(!oneOf(humans1.begin(), humans1.end(), [](Human x){return x == Human(180, 18);}));
    assert(allOf(humans1.begin(), humans1.end(), [](Human x){return x == Human(180, 18);}));
    assert(isPartitioned(humans1.begin(), humans1.end(), [](Human x){return x == Human(180, 18);}));
    assert(isPalindrome(humans1.begin(), humans1.end(), [](Human x, Human y){return x == y;}));
}

int main() {
    defaultCheck();
    customCheck();
    return 0;
}

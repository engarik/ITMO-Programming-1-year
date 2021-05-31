#include <iostream>

#ifndef LAB4_PREDICATE_HPP
#define LAB4_PREDICATE_HPP

template<class InputIterator, class Predicate>
bool allOf(InputIterator first, InputIterator last, Predicate condition) {
    while (first != last) {
        if (!condition(*first))
            return false;
        ++first;
    }
    return true;
}

template<class InputIterator, class Predicate>
bool anyOf(InputIterator first, InputIterator last, Predicate condition) {
    while (first != last) {
        if (condition(*first))
            return true;
        ++first;
    }
    return false;
}

template<class InputIterator, class Predicate>
bool noneOf(InputIterator first, InputIterator last, Predicate condition) {
    while (first != last) {
        if (condition(*first))
            return false;
        ++first;
    }
    return true;
}

template<class InputIterator, class Predicate>
bool oneOf(InputIterator first, InputIterator last, Predicate condition) {
    bool found = false;
    while (first != last) {
        if (condition(*first)) {
            if (found) return false;
            else found = true;
        }
        ++first;
    }
    return found;
}

template<class T>
struct less {
    bool operator()(const T &a, const T &b) const { return a < b; }
};

template<class ForwardIterator, class Predicate = less<typename ForwardIterator::value_type>>
bool isSorted(ForwardIterator first, ForwardIterator last, Predicate condition = Predicate()) {
    if (first != last) {
        ForwardIterator next(first);
        while (++next != last) {
            if (condition(*next, *first))
                return false;
            first = next;
        }
    }
    return true;
}

template<class InputIterator, class Predicate>
bool isPartitioned(InputIterator first, InputIterator last, Predicate condition) {
    while (first != last && condition(*first))
        ++first;
    while (first != last) {
        if (condition(*first))
            return false;
        ++first;
    }
    return true;
}

template<class InputIterator, class Class>
InputIterator findNot(InputIterator first, InputIterator last, Class element) {
    while (first != last) {
        if ((*first) != element)
            return first;
        ++first;
    }
    return last;
}

template<class BidirectionalIterator, class Class>
// Can be done using ForwardIterator, but it's not logical to do so...
BidirectionalIterator findBackwards(BidirectionalIterator first, BidirectionalIterator last, Class element) {
    BidirectionalIterator iter(last);
    while (iter != first) {
        if ((*iter) == element)
            return iter;
        iter--;
    }
    return last;
}

template<class BidirectionalIterator, class Predicate>
bool isPalindrome(BidirectionalIterator first, BidirectionalIterator last, Predicate condition) {
    --last;
    while (first != last) {
        if (++first == last)
            return condition(*(--first), *last);
        else --first;

        if (!condition(*first, *last))
            return false;
        ++first, --last;
    }
    return true;
}


#endif

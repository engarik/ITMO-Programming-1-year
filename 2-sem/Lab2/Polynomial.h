#include <cassert>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>

#ifndef LAB2_POLYNOMIAL_H
#define LAB2_POLYNOMIAL_H

using namespace std;

class Term {
public:

    // Default constructor
    explicit Term(int coeff = 0, int power = 0)
    : coeff(coeff), power(power) {}

    // Copy constructor
    Term(const Term& other) = default;

    // Destructor
    ~Term() = default;

    // Copy-assign operator
    Term& operator=(const Term& other) = default;

    // Some methods
    int getCoeff() const {
        return coeff;
    }

    int getPower() const {
        return power;
    }

    // as-class-instance-operators

    Term operator-() const;

    Term& operator+=(const Term& other);

    Term& operator*=(int number);

    Term& operator/=(int number);

    // as-function-operators

    friend ostream& operator<<(ostream& stream, const Term& term);

    friend istream& operator>>(istream& stream, Term& term);

    friend Term operator+(const Term& lhs, const Term& rhs);

    friend Term operator-(const Term& lhs, const Term& rhs);

    friend Term operator*(const Term& lhs, const Term& rhs);

    friend Term operator*(const Term& lhs, int rhs);

    friend Term operator/(const Term& lhs, int rhs);

private:
    int coeff, power;
};

// as-function-operators headers

ostream& operator<<(ostream& stream, const Term& term);

istream& operator>>(istream& stream, Term& term);

bool operator==(const Term& lhs, const Term& rhs);

bool operator!=(const Term& lhs, const Term& rhs);

bool operator<(const Term& rhs, const Term& lhs);

bool operator>(const Term& rhs, const Term& lhs);

bool operator<=(const Term& rhs, const Term& lhs);

bool operator>=(const Term& rhs, const Term& lhs);

Term operator+(const Term& lhs, const Term& rhs);

Term operator-(const Term& lhs, const Term& rhs);

Term operator*(const Term& lhs, int rhs);

Term operator/(const Term& lhs, int rhs);

Term operator*(const Term& lhs, const Term& rhs);

class Polynomial {
public:
    // Default constructor
    explicit Polynomial(int minPower = 0, int maxPower = 0, const int * coefficients = nullptr) {
        int termsNumber = maxPower - minPower + 1;
        if (termsNumber < 1) throw std::invalid_argument("You should pass at least 1 argument");

        if (coefficients != nullptr) {
            for (int i = 0; i < termsNumber; ++i) {
                if (coefficients[i] != 0) terms.emplace_back(coefficients[i], minPower + i);
            }
        } else {
            terms.emplace_back();
        }
    }

    // Copy constructor
    Polynomial(const Polynomial& other) = default;

    // Copy-assign operator
    Polynomial& operator=(const Polynomial& other) {
        if (this != &other) {
            terms = other.terms;
        }
        return *this;
    }

    // Some methods
    int getTermsNumber() const {
        return terms.size();
    }

    Term getTerm(int power) const {
        for (auto & term : terms) {
            if (term.getPower() == power) return term;
        }
        return Term(0, power);
    }

    int getTermPosition(int power) const {
        for (int i = 0; i < terms.size(); i++) {
            if (terms[i].getPower() == power) return i;
        }
        return -1;
    }

    bool isZero() const {
        return terms.size() == 1 && terms[0] == Term();
    }

    double get(int value) {
        double res = 0;
        for (auto & term : terms) {
            res += term.getCoeff() * pow(value,term.getPower());
        }
        return res;
    }

    // as-class-instance-operators

    Polynomial operator-() const;

    Polynomial& operator+=(const Polynomial& other);

    Polynomial& operator+=(const Term& other);

    Polynomial& operator-=(const Polynomial& other);

    Polynomial& operator*=(int number);

    Polynomial& operator/=(int number);

    int operator[](int termPower) const;

    // as-function-operators

    friend ostream& operator<<(ostream& stream, const Polynomial& polynomial);

    friend istream& operator>>(istream& stream, Polynomial& polynomial);

    friend Polynomial operator+(const Polynomial& lhs, const Polynomial& rhs);

    friend Polynomial operator+(const Polynomial& lhs, const Term& rhs);

    friend Polynomial operator-(const Polynomial& lhs, const Polynomial& rhs);

    friend Polynomial operator*(const Polynomial& lhs, const Polynomial& rhs);

    friend Polynomial operator*(const Polynomial& lhs, int rhs);

    friend Polynomial operator*(int lhs, const Polynomial& rhs);

    friend Polynomial operator/(const Polynomial& lhs, int rhs);

private:
    vector<Term> terms;
};

// as-function-operators headers

ostream& operator<<(ostream& stream, const Polynomial& polynomial);

istream& operator>>(istream& stream, Polynomial& polynomial);

bool operator==(const Polynomial& lhs, const Polynomial& rhs);

bool operator!=(const Polynomial& lhs, const Polynomial& rhs);

Polynomial operator+(const Polynomial& lhs, const Polynomial& rhs);

Polynomial operator+(const Polynomial& lhs, const Term& rhs);

Polynomial operator-(const Polynomial& lhs, const Polynomial& rhs);

Polynomial operator*(const Polynomial& lhs, int rhs);

Polynomial operator*(int lhs, const Polynomial& rhs);

Polynomial operator/(const Polynomial& lhs, int rhs);

Polynomial operator*(const Polynomial& lhs, const Polynomial& rhs);

#endif //LAB2_POLYNOMIAL_H

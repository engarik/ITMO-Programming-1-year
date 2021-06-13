#include "Polynomial.h"
#include <algorithm>

// Term

// as-class-instance-operators implementation

Term Term::operator-() const {
    return Term(coeff * -1, power);
}

Term& Term::operator+=(const Term& other) {
    *this = *this + other;

    return *this;
}

Term& Term::operator*=(int number) {
    *this = *this * number;

    return *this;
}

Term& Term::operator/=(int number) {
    *this = *this / number;

    return *this;
}

ostream& operator<<(ostream& stream, const Term& term) {
    if (term.coeff != 0) {
        if (term.coeff != 1 && term.coeff != -1) {
            if (term.power > 1 || term.power < 0) {
                stream << term.coeff << "x^" << term.power;
            } else if (term.power == 1) {
                stream << term.coeff << "x";
            } else {
                stream << term.coeff;
            }
        } else {
            if (term.power > 1 || term.power < 0) {
                stream << (term.coeff == -1 ? "-" : "") << "x^" << term.power;
            } else if (term.power == 1) {
                stream << (term.coeff == -1 ? "-" : "") << "x";
            } else {
                stream << term.coeff;
            }
        }
    } else if (term.power == 0) {
        stream << 0;
    }

    return stream;
}

istream& operator>>(istream& stream, Term& term) {
    int coeff, power;

    stream >> coeff >> power;

    term = Term(coeff, power);

    return stream;
}

bool operator==(const Term& lhs, const Term& rhs) {
    return (lhs.getPower() == rhs.getPower()) && (lhs.getCoeff() == rhs.getCoeff());
}

bool operator!=(const Term& lhs, const Term& rhs) {
    return !(lhs == rhs);
}

bool operator<(const Term& rhs, const Term& lhs) {
    return rhs.getPower() < lhs.getPower();
}

bool operator>(const Term& rhs, const Term& lhs) {
    return rhs.getPower() > lhs.getPower();
}

bool operator<=(const Term& rhs, const Term& lhs) {
    return rhs < lhs || rhs == lhs;
}

bool operator>=(const Term& rhs, const Term& lhs) {
    return rhs > lhs || rhs == lhs;
}

// as-function-operators implementation

Term operator+(const Term& lhs, const Term& rhs) {
    assert(lhs.power == rhs.power);

    return Term(lhs.coeff + rhs.coeff, lhs.power);
}

Term operator-(const Term& lhs, const Term& rhs) {
    return lhs + (-rhs);
}

Term operator*(const Term& lhs, const Term& rhs) {
    return Term(lhs.coeff * rhs.coeff, lhs.power + rhs.power);
}

Term operator*(const Term& lhs, int rhs) {
    return Term(lhs.coeff * rhs, lhs.power);
}

Term operator/(const Term& lhs, int rhs) {
    assert(rhs != 0);

    return Term(lhs.coeff / rhs, lhs.power);
}

// Polynomial

// as-class-instance-operators implementation

Polynomial Polynomial::operator-() const {
    Polynomial tmp = *this;

    for (auto & term : tmp.terms) {
        term = -term;
    }

    return tmp;
}

Polynomial& Polynomial::operator+=(const Polynomial& other) {
    *this = (*this + other);
    return *this;
}

Polynomial& Polynomial::operator+=(const Term& other) {
    *this = (*this + other);
    return *this;
}

Polynomial& Polynomial::operator-=(const Polynomial& other) {
    *this = *this - other;
    return *this;
}

Polynomial& Polynomial::operator*=(int number) {
    *this = *this * number;
    return *this;
}

Polynomial& Polynomial::operator/=(int number) {
    *this = *this / number;
    return *this;
}

int Polynomial::operator[](int termPower) const {
    return this->getTerm(termPower).getCoeff();
}

// as-function-operators implementation

string getSign(const Term &term, bool isFirst) {
    if (isFirst) {
        return "";
    } else {
        if (term.getCoeff() > 0) {
            return "+";
        } else return "";
    }
}

ostream& operator<<(ostream& stream, const Polynomial& polynomial) {
    for (int i = 0; i < polynomial.terms.size(); ++i) {
        const Term& cur = polynomial.terms[polynomial.terms.size() - i - 1];
        if (cur.getPower() != 0 || cur.getCoeff() != 0) {
            stream << getSign(cur, i == 0) << cur;
        } else if (polynomial.terms.size() == 1) {
            stream << cur;
        }
    }
    return stream;
}

istream& operator>>(istream& stream, Polynomial& polynomial) {
    int minPower, maxPower;
    stream >> minPower >> maxPower;
    assert(maxPower >= minPower);
    int * coeff = new int[maxPower - minPower + 1];

    for (int i = 0; i < maxPower - minPower + 1; ++i) {
        stream >> coeff[i];
    }

    polynomial = Polynomial(minPower, maxPower, coeff);

    delete [] coeff;

    return stream;
}

bool operator==(const Polynomial& lhs, const Polynomial& rhs) {
    for (int i = 0; i < max(lhs.getTermsNumber(), rhs.getTermsNumber()); ++i) {
        if (lhs.getTerm(i) != rhs.getTerm(i))
            return false;
    }
    return true;
}

bool operator!=(const Polynomial& lhs, const Polynomial& rhs) {
    return !(lhs == rhs);
}

Polynomial operator+(const Polynomial& lhs, const Polynomial& rhs) {
    Polynomial res = Polynomial();
    res.terms.clear();

    int lPos = 0, rPos = 0;

    while (lPos < lhs.terms.size() || rPos < rhs.terms.size()) {
        if (lhs.terms[lPos].getPower() == rhs.terms[rPos].getPower()) {
            res.terms.push_back(lhs.terms[lPos] + rhs.terms[rPos]);
            ++lPos; ++rPos;
        } else if (lhs.terms[lPos].getPower() > rhs.terms[rPos].getPower()) {
            res.terms.push_back(rhs.terms[rPos]);
            ++rPos;
        } else {
            res.terms.push_back(lhs.terms[lPos]);
            ++lPos;
        }
    }

    while (lPos < lhs.terms.size()) {
        res.terms.push_back(lhs.terms[lPos]);
        ++lPos;
    }

    while (rPos < rhs.terms.size()) {
        res.terms.push_back(rhs.terms[rPos]);
        ++rPos;
    }

    return res;
}

Polynomial operator+(const Polynomial& lhs, const Term& rhs) {
    Polynomial res = lhs;

    if (rhs.getCoeff() != 0) {
        int pos = res.getTermPosition(rhs.getPower());

        if (pos != -1) {
            res.terms[pos] += rhs;
        } else {
            res.terms.push_back(rhs);
            std::sort(res.terms.begin(), res.terms.end());
        }
    }

    return res;
}

Polynomial operator-(const Polynomial& lhs, const Polynomial& rhs) {
    return lhs + (-rhs);
}

Polynomial operator*(const Polynomial& lhs, const Polynomial& rhs) {
    Polynomial res;

    if (lhs.isZero() || rhs.isZero()) {
        return res;
    }

    for (auto lTerm : lhs.terms) {
        for (auto rTerm : rhs.terms) {
            res += lTerm * rTerm;
        }
    }

    std::sort(res.terms.begin(), res.terms.end());

    return res;
}

Polynomial operator*(const Polynomial& lhs, int rhs) {
    Polynomial res = lhs;
    for (auto & term : res.terms) {
        term *= rhs;
    }

    return res;
}

Polynomial operator*(int lhs, const Polynomial& rhs) {
    return rhs * lhs;
}

Polynomial operator/(const Polynomial& lhs, int rhs) {
    Polynomial res = lhs;
    for (auto & term : res.terms) {
        term /= rhs;
    }

    return res;
}


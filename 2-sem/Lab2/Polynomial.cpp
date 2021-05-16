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

    for (int i = 0; i < tmp.termsNumber; ++i) {
        tmp.terms[i] = -tmp.terms[i];
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
    for (int i = 0; i < polynomial.termsNumber; ++i) {
        const Term& cur = polynomial.terms[polynomial.termsNumber - i - 1];
        if (cur.getPower() != 0 || cur.getCoeff() != 0) {
            stream << getSign(cur, i == 0) << cur;
        } else if (polynomial.termsNumber == 1) {
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
    int min = std::min(lhs.minPower, rhs.minPower), max = std::max(lhs.maxPower, rhs.maxPower);

    Polynomial res = Polynomial(min, max);

    for (int i = min; i <= max;i++) {
        res.terms[i - min] = lhs.getTerm(i) + rhs.getTerm(i);
    }

    return res;
}

Polynomial operator+(const Polynomial& lhs, const Term& rhs) {
    Polynomial res = lhs;
    if (lhs.containsTerm(rhs.getPower())) {
        res.terms[rhs.getPower() - res.minPower] += rhs;
    } else {
        int oldTermsNumber = lhs.termsNumber;
        Term * tmp = new Term[oldTermsNumber];

        for (int i = 0; i < oldTermsNumber; ++i) {
            tmp[i] = lhs.terms[i];
        }

        int offset = 0, pos = 0;

        if (rhs.getPower() > lhs.maxPower) {
            if (lhs.isZero()) {
                res.minPower = rhs.getPower();
            }
            res.maxPower = rhs.getPower();
            pos = rhs.getPower() - res.minPower;
        } else if (rhs.getPower() < lhs.minPower) {
            offset = lhs.minPower - rhs.getPower();
            if (lhs.isZero()) {
                res.maxPower = rhs.getPower();
            }
            res.minPower = rhs.getPower();
        }

        res.updateTermsNumber();

        delete [] res.terms;
        res.terms = new Term[res.termsNumber];

        for (int i = 0; i < oldTermsNumber; ++i) {
            res.terms[i + offset] = tmp[i];
        }

        res.terms[pos] = rhs;

        delete [] tmp;
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

    for (int i = 0; i < lhs.termsNumber; ++i) {
        Term cur = lhs.terms[i];

        for (int j = 0; j < rhs.termsNumber; ++j) {
            res += rhs.terms[j] * cur;
        }
    }

    return res;
}

Polynomial operator*(const Polynomial& lhs, int rhs) {
    Polynomial res = lhs;
    for (int i = 0; i < lhs.termsNumber; ++i) {
        res.terms[i] *= rhs;
    }

    return res;
}

Polynomial operator*(int lhs, const Polynomial& rhs) {
    return rhs * lhs;
}

Polynomial operator/(const Polynomial& lhs, int rhs) {
    Polynomial res = lhs;
    for (int i = 0; i < lhs.termsNumber; ++i) {
        res.terms[i] /= rhs;
    }

    return res;
}


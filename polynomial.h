#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <cmath>
#include <string>

#include <QString>

using namespace std;

class Polynomial {
public:
    Polynomial();
    explicit Polynomial(const QString& text);
    explicit Polynomial(const string& text);
    Polynomial(const Polynomial& other);
    Polynomial(Polynomial&& other) noexcept;
    Polynomial& operator=(const Polynomial& other);
    Polynomial& operator=(Polynomial&& other) noexcept;
    ~Polynomial();

    bool isEmpty() const;
    bool parseFromString(const QString& text);
    QString toExpressionString() const;
    QString toSequenceString() const;
    Polynomial add(const Polynomial& other) const;
    Polynomial subtract(const Polynomial& other) const;
    long double evaluate(long double x) const;

private:
    struct Term {
        long long coefficient;
        long long exponent;
        Term* next;
    };

    Term* head;

    void clear();
    void copyFrom(const Polynomial& other);
    int countTerms() const;
    void insertTerm(long long coefficient, long long exponent);
    static long double power(long double base, long long exponent);
};

#endif // POLYNOMIAL_H

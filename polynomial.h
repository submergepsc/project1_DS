#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <map>

#include <QString>

class Polynomial {
public:
    Polynomial();

    bool isZero() const;
    bool parseFromString(const QString& text, QString* errorMessage);
    QString toExpressionString() const;
    QString toSequenceString() const;
    long double evaluate(long double x) const;

private:
    using TermMap = std::map<long long, long long>;

    TermMap terms;

    void reset();
    static long double fastPower(long double base, long long exponent);
};

#endif // POLYNOMIAL_H

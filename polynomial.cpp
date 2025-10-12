#include "polynomial.h"

#include <cmath>
#include <cstdlib>

#include <QStringList>

namespace {
QStringList tokenizeSequence(const QString& text)
{
    QString simplified = text;
    simplified.replace(',', ' ');
    simplified.replace('\n', ' ');
    simplified.replace('\t', ' ');
    return simplified.split(' ', Qt::SkipEmptyParts);
}
}

Polynomial::Polynomial() = default;

bool Polynomial::isZero() const
{
    return terms.empty();
}

bool Polynomial::parseSequence(const QString& text, QString* errorMessage)
{
    if (errorMessage) {
        errorMessage->clear();
    }

    reset();

    const QString trimmed = text.trimmed();
    if (trimmed.isEmpty()) {
        return true;
    }

    const QStringList tokens = tokenizeSequence(trimmed);
    if (tokens.size() % 2 != 0) {
        if (errorMessage) {
            *errorMessage = QStringLiteral("请输入成对的系数和指数。");
        }
        reset();
        return false;
    }

    for (int i = 0; i < tokens.size(); i += 2) {
        bool coefficientOk = false;
        const long long coefficient = tokens.at(i).toLongLong(&coefficientOk);
        if (!coefficientOk) {
            if (errorMessage) {
                *errorMessage = QStringLiteral("无法解析第 %1 个系数。").arg(i / 2 + 1);
            }
            reset();
            return false;
        }

        bool exponentOk = false;
        const long long exponent = tokens.at(i + 1).toLongLong(&exponentOk);
        if (!exponentOk) {
            if (errorMessage) {
                *errorMessage = QStringLiteral("无法解析第 %1 个指数。").arg(i / 2 + 1);
            }
            reset();
            return false;
        }

        if (coefficient == 0) {
            continue;
        }

        const auto iter = terms.find(exponent);
        if (iter == terms.end()) {
            terms.emplace(exponent, coefficient);
        } else {
            iter->second += coefficient;
            if (iter->second == 0) {
                terms.erase(iter);
            }
        }
    }

    for (auto it = terms.begin(); it != terms.end();) {
        if (it->second == 0) {
            it = terms.erase(it);
        } else {
            ++it;
        }
    }

    return true;
}

QString Polynomial::toExpressionString() const
{
    if (terms.empty()) {
        return QStringLiteral("0");
    }

    QStringList parts;
    parts.reserve(static_cast<int>(terms.size()));

    for (auto iter = terms.rbegin(); iter != terms.rend(); ++iter) {
        const long long exponent = iter->first;
        const long long coefficient = iter->second;

        QString part;
        if (coefficient < 0) {
            part.append('-');
        } else if (!parts.isEmpty()) {
            part.append('+');
        }

        const long long absCoefficient = std::llabs(coefficient);
        if (exponent == 0) {
            part.append(QString::number(absCoefficient));
        } else {
            if (absCoefficient != 1) {
                part.append(QString::number(absCoefficient));
            }
            part.append('x');
            if (exponent != 1) {
                part.append('^');
                part.append(QString::number(exponent));
            }
        }

        parts.append(part);
    }

    return parts.join(QString());
}

QString Polynomial::toSequenceString(SequenceOrder order) const
{
    if (terms.empty()) {
        return QStringLiteral("0");
    }

    QStringList parts;
    parts.reserve(static_cast<int>(terms.size()) * 2);

    if (order == SequenceOrder::Ascending) {
        for (const auto& term : terms) {
            parts.append(QString::number(term.second));
            parts.append(QString::number(term.first));
        }
    } else {
        for (auto iter = terms.rbegin(); iter != terms.rend(); ++iter) {
            parts.append(QString::number(iter->second));
            parts.append(QString::number(iter->first));
        }
    }

    return parts.join(' ');
}

long double Polynomial::evaluate(long double x) const
{
    long double result = 0.0L;
    for (const auto& term : terms) {
        const long long exponent = term.first;
        const long long coefficient = term.second;
        result += static_cast<long double>(coefficient) * fastPower(x, exponent);
    }
    return result;
}

Polynomial Polynomial::add(const Polynomial& left, const Polynomial& right)
{
    Polynomial result;
    result.terms = left.terms;

    for (const auto& term : right.terms) {
        result.terms[term.first] += term.second;
        if (result.terms[term.first] == 0) {
            result.terms.erase(term.first);
        }
    }

    return result;
}

void Polynomial::reset()
{
    terms.clear();
}

long double Polynomial::fastPower(long double base, long long exponent)
{
    if (exponent == 0) {
        return 1.0L;
    }

    long double result = 1.0L;
    long double current = base;
    long long power = exponent;

    while (power > 0) {
        if (power & 1LL) {
            result *= current;
        }
        current *= current;
        power >>= 1;
    }

    return result;
}

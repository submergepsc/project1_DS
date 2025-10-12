#include "polynomial.h"

#include <cmath>

#include <cstdlib>

#include <QStringList>

Polynomial::Polynomial() = default;

bool Polynomial::isZero() const {
    return terms.empty();
}

bool Polynomial::parseFromString(const QString& text, QString* errorMessage) {
    if (errorMessage) {
        errorMessage->clear();
    }

    reset();

    QString sanitized;
    sanitized.reserve(text.size());
    for (QChar ch : text) {
        if (!ch.isSpace()) {
            sanitized.append(ch);
        }
    }

    if (sanitized.isEmpty()) {
        return true;
    }

    bool trimmed = true;
    while (trimmed && sanitized.size() >= 2 && sanitized.front() == '(' && sanitized.back() == ')') {
        trimmed = false;
        int depth = 0;
        bool encloses = true;
        for (int i = 0; i < sanitized.size(); ++i) {
            const QChar ch = sanitized[i];
            if (ch == '(') {
                ++depth;
            } else if (ch == ')') {
                --depth;
                if (depth == 0 && i < sanitized.size() - 1) {
                    encloses = false;
                    break;
                }
                if (depth < 0) {
                    encloses = false;
                    break;
                }
            }
        }

        if (encloses && depth == 0) {
            sanitized = sanitized.mid(1, sanitized.size() - 2);
            trimmed = true;
        }
    }

    if (sanitized.isEmpty()) {
        return true;
    }

    const int length = sanitized.size();
    int index = 0;

    while (index < length) {
        int sign = 1;
        if (sanitized[index] == '+') {
            ++index;
        } else if (sanitized[index] == '-') {
            sign = -1;
            ++index;
        } else if (index != 0) {
            if (errorMessage) {
                *errorMessage = QStringLiteral("缺少运算符。");
            }
            reset();
            return false;
        }

        if (index >= length) {
            if (errorMessage) {
                *errorMessage = QStringLiteral("表达式以符号结尾。");
            }
            reset();
            return false;
        }

        QString coefficientPart;
        while (index < length && sanitized[index].isDigit()) {
            coefficientPart.append(sanitized[index]);
            ++index;
        }

        bool hasVariable = false;
        long long exponent = 0;
        if (index < length && (sanitized[index] == 'x' || sanitized[index] == 'X')) {
            hasVariable = true;
            ++index;
            exponent = 1;
            if (index < length && sanitized[index] == '^') {
                ++index;
                if (index >= length) {
                    if (errorMessage) {
                        *errorMessage = QStringLiteral("指数缺失。");
                    }
                    reset();
                    return false;
                }

                QString exponentPart;
                while (index < length && sanitized[index].isDigit()) {
                    exponentPart.append(sanitized[index]);
                    ++index;
                }

                if (exponentPart.isEmpty()) {
                    if (errorMessage) {
                        *errorMessage = QStringLiteral("指数缺失。");
                    }
                    reset();
                    return false;
                }

                bool exponentOk = false;
                exponent = exponentPart.toLongLong(&exponentOk);
                if (!exponentOk) {
                    if (errorMessage) {
                        *errorMessage = QStringLiteral("无法解析指数。");
                    }
                    reset();
                    return false;
                }
            }
        }

        long long coefficient = 0;
        if (coefficientPart.isEmpty()) {
            if (hasVariable) {
                coefficient = 1;
            } else {
                if (errorMessage) {
                    *errorMessage = QStringLiteral("常数项缺少系数。");
                }
                reset();
                return false;
            }
        } else {
            bool coefficientOk = false;
            coefficient = coefficientPart.toLongLong(&coefficientOk);
            if (!coefficientOk) {
                if (errorMessage) {
                    *errorMessage = QStringLiteral("无法解析系数。");
                }
                reset();
                return false;
            }
        }

        coefficient *= sign;
        terms[exponent] += coefficient;

        if (index < length && sanitized[index] != '+' && sanitized[index] != '-') {
            if (errorMessage) {
                *errorMessage = QStringLiteral("在第 %1 个字符附近检测到无法识别的内容。")
                                      .arg(index + 1);
            }
            reset();
            return false;
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

QString Polynomial::toExpressionString() const {
    if (terms.empty()) {
        return QStringLiteral("0");
    }

    QStringList parts;
    for (auto it = terms.rbegin(); it != terms.rend(); ++it) {
        const long long exponent = it->first;
        const long long coefficient = it->second;
        if (coefficient == 0) {
            continue;
        }

        QString term;
        const bool negative = coefficient < 0;
        const long long absoluteCoefficient = std::llabs(coefficient);

        if (parts.isEmpty()) {
            if (negative) {
                term.append('-');
            }
        } else {
            term.append(negative ? QStringLiteral(" - ") : QStringLiteral(" + "));
        }

        const bool showCoefficient = (exponent == 0) || (absoluteCoefficient != 1);
        if (showCoefficient) {
            term.append(QString::number(absoluteCoefficient));
        }

        if (exponent > 0) {
            if (showCoefficient) {
                term.append('x');
            } else {
                term.append('x');
            }

            if (exponent > 1) {
                term.append('^');
                term.append(QString::number(exponent));
            }
        }

        parts.append(term);
    }

    if (parts.isEmpty()) {
        return QStringLiteral("0");
    }

    return parts.join(QString());
}

QString Polynomial::toSequenceString() const {
    if (terms.empty()) {
        return QStringLiteral("[]");
    }

    QStringList items;
    for (auto it = terms.rbegin(); it != terms.rend(); ++it) {
        items.append(QStringLiteral("(%1, %2)").arg(QString::number(it->second), QString::number(it->first)));
    }

    return QStringLiteral("[ %1 ]").arg(items.join(QStringLiteral(", ")));
}

long double Polynomial::evaluate(long double x) const {
    long double total = 0.0L;
    for (const auto& term : terms) {
        const long double coefficient = static_cast<long double>(term.second);
        total += coefficient * fastPower(x, term.first);
    }
    return total;
}

void Polynomial::reset() {
    terms.clear();
}

long double Polynomial::fastPower(long double base, long long exponent) {
    if (exponent == 0) {
        return 1.0L;
    }

    long double result = 1.0L;
    long double factor = base;
    long long remaining = exponent;

    while (remaining > 0) {
        if ((remaining & 1LL) != 0) {
            result *= factor;
        }
        factor *= factor;
        remaining >>= 1;
    }

    return result;
}

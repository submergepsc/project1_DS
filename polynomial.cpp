#include <QTextStream>
#include "polynomial.h"

Polynomial::Polynomial() : head(nullptr) {}

Polynomial::Polynomial(const QString& text) : head(nullptr) {
    parseFromString(text);
}

Polynomial::Polynomial(const string& text) : head(nullptr) {
    parseFromString(QString::fromStdString(text));
}

Polynomial::Polynomial(const Polynomial& other) : head(nullptr) {
    copyFrom(other);
}

Polynomial::Polynomial(Polynomial&& other) noexcept : head(other.head) {
    other.head = nullptr;
}

Polynomial& Polynomial::operator=(const Polynomial& other) {
    if (this != &other) {
        clear();
        copyFrom(other);
    }
    return *this;
}

Polynomial& Polynomial::operator=(Polynomial&& other) noexcept {
    if (this != &other) {
        clear();
        head = other.head;
        other.head = nullptr;
    }
    return *this;
}

Polynomial::~Polynomial() {
    clear();
}

bool Polynomial::isEmpty() const {
    return head == nullptr;
}

bool Polynomial::parseFromString(const QString& text) {
    clear();

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

    int length = sanitized.size();
    int index = 0;
    bool parsedAny = false;

    while (index < length) {
        int sign = 1;
        if (sanitized[index] == '+') {
            ++index;
        } else if (sanitized[index] == '-') {
            sign = -1;
            ++index;
        } else if (parsedAny) {
            clear();
            return false;
        }

        if (index >= length) {
            clear();
            return false;
        }

        QString coefPart;
        while (index < length && sanitized[index].isDigit()) {
            coefPart.append(sanitized[index]);
            ++index;
        }

        bool hasVariable = false;
        if (index < length && (sanitized[index] == 'x' || sanitized[index] == 'X')) {
            hasVariable = true;
            ++index;
        }

        long long exponent = 0;
        if (hasVariable) {
            exponent = 1;
            if (index < length && sanitized[index] == '^') {
                ++index;
                QString exponentPart;
                while (index < length && sanitized[index].isDigit()) {
                    exponentPart.append(sanitized[index]);
                    ++index;
                }
                if (exponentPart.isEmpty()) {
                    clear();
                    return false;
                }
                bool okExp = false;
                long long parsedExp = exponentPart.toLongLong(&okExp);
                if (!okExp || parsedExp < 0) {
                    clear();
                    return false;
                }
                exponent = parsedExp;
            }
        } else if (coefPart.isEmpty()) {
            clear();
            return false;
        }

        long long coefficient = 0;
        if (coefPart.isEmpty()) {
            coefficient = sign;
        } else {
            bool okCoef = false;
            long long parsedCoef = coefPart.toLongLong(&okCoef);
            if (!okCoef) {
                clear();
                return false;
            }
            coefficient = parsedCoef * sign;
        }

        if (!hasVariable) {
            exponent = 0;
        }

        if (coefficient != 0) {
            insertTerm(coefficient, exponent);
        }

        parsedAny = true;

        if (index < length && sanitized[index] != '+' && sanitized[index] != '-') {
            clear();
            return false;
        }
    }

    return true;
}

QString Polynomial::toExpressionString() const {
    if (head == nullptr) {
        return QStringLiteral("0");
    }

    QString result;
    QTextStream stream(&result);
    Term* current = head;
    bool first = true;

    while (current != nullptr) {
        long long coefficient = current->coefficient;
        long long exponent = current->exponent;

        if (first) {
            if (coefficient < 0) {
                stream << '-';
            }
        } else {
            stream << (coefficient < 0 ? '-' : '+');
        }

        long long absCoef = coefficient < 0 ? -coefficient : coefficient;
        bool showCoefficient = !(absCoef == 1 && exponent != 0);

        if (showCoefficient) {
            stream << absCoef;
        }

        if (exponent != 0) {
            stream << 'x';
            if (exponent != 1) {
                stream << '^' << exponent;
            }
        }

        first = false;
        current = current->next;
    }

    return result;
}

QString Polynomial::toSequenceString() const {
    int termCount = countTerms();
    if (termCount == 0) {
        return QStringLiteral("0");
    }

    QString result = QString::number(termCount);
    Term* current = head;
    while (current != nullptr) {
        result.append(',');
        result.append(QString::number(current->coefficient));
        result.append(',');
        result.append(QString::number(current->exponent));
        current = current->next;
    }

    return result;
}

Polynomial Polynomial::add(const Polynomial& other) const {
    Polynomial output;
    Term* lhs = head;
    Term* rhs = other.head;

    while (lhs != nullptr || rhs != nullptr) {
        if (rhs == nullptr || (lhs != nullptr && lhs->exponent > rhs->exponent)) {
            output.insertTerm(lhs->coefficient, lhs->exponent);
            lhs = lhs->next;
        } else if (lhs == nullptr || rhs->exponent > lhs->exponent) {
            output.insertTerm(rhs->coefficient, rhs->exponent);
            rhs = rhs->next;
        } else {
            long long sum = lhs->coefficient + rhs->coefficient;
            if (sum != 0) {
                output.insertTerm(sum, lhs->exponent);
            }
            lhs = lhs->next;
            rhs = rhs->next;
        }
    }

    return output;
}

Polynomial Polynomial::subtract(const Polynomial& other) const {
    Polynomial output;
    Term* lhs = head;
    Term* rhs = other.head;

    while (lhs != nullptr || rhs != nullptr) {
        if (rhs == nullptr || (lhs != nullptr && lhs->exponent > rhs->exponent)) {
            output.insertTerm(lhs->coefficient, lhs->exponent);
            lhs = lhs->next;
        } else if (lhs == nullptr || rhs->exponent > lhs->exponent) {
            output.insertTerm(-rhs->coefficient, rhs->exponent);
            rhs = rhs->next;
        } else {
            long long diff = lhs->coefficient - rhs->coefficient;
            if (diff != 0) {
                output.insertTerm(diff, lhs->exponent);
            }
            lhs = lhs->next;
            rhs = rhs->next;
        }
    }

    return output;
}

long double Polynomial::evaluate(long double x) const {
    long double total = 0.0L;
    for (Term* current = head; current != nullptr; current = current->next) {
        long double coef = static_cast<long double>(current->coefficient);
        total += coef * power(x, current->exponent);
    }
    return total;
}

long double Polynomial::power(long double base, long long exponent) {
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

void Polynomial::clear() {
    Term* current = head;
    while (current != nullptr) {
        Term* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
}

void Polynomial::copyFrom(const Polynomial& other) {
    Term* current = other.head;
    while (current != nullptr) {
        insertTerm(current->coefficient, current->exponent);
        current = current->next;
    }
}

int Polynomial::countTerms() const {
    int count = 0;
    for (Term* current = head; current != nullptr; current = current->next) {
        ++count;
    }
    return count;
}

void Polynomial::insertTerm(long long coefficient, long long exponent) {
    if (coefficient == 0) {
        return;
    }

    Term* newTerm = new Term{coefficient, exponent, nullptr};

    if (head == nullptr || head->exponent < exponent) {
        newTerm->next = head;
        head = newTerm;
        return;
    }

    Term* current = head;
    Term* previous = nullptr;

    while (current != nullptr && current->exponent > exponent) {
        previous = current;
        current = current->next;
    }

    if (current != nullptr && current->exponent == exponent) {
        long long combined = current->coefficient + coefficient;
        if (combined == 0) {
            if (previous == nullptr) {
                head = current->next;
            } else {
                previous->next = current->next;
            }
            delete current;
            delete newTerm;
        } else {
            current->coefficient = combined;
            delete newTerm;
        }
        return;
    }

    if (previous == nullptr) {
        newTerm->next = head;
        head = newTerm;
    } else {
        newTerm->next = previous->next;
        previous->next = newTerm;
    }
}

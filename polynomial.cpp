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

    QString simplified = text.simplified();
    if (simplified.isEmpty()) {
        return true;
    }

    QStringList tokens = simplified.split(' ');
    if (tokens.isEmpty()) {
        return false;
    }

    bool okCount = false;
    int expectedTerms = tokens[0].toInt(&okCount);
    if (!okCount || expectedTerms < 0) {
        return false;
    }

    if (tokens.size() != 1 + expectedTerms * 2) {
        return false;
    }

    for (int i = 0; i < expectedTerms; ++i) {
        bool okCoef = false;
        bool okExp = false;
        long long coef = tokens[1 + i * 2].toLongLong(&okCoef);
        long long exp = tokens[2 + i * 2].toLongLong(&okExp);
        if (!okCoef || !okExp) {
            clear();
            return false;
        }
        if (coef != 0) {
            insertTerm(coef, exp);
        }
    }

    return true;
}

QString Polynomial::toSequenceString() const {
    QString result;
    QTextStream stream(&result);
    int count = countTerms();
    stream << count;
    for (Term* current = head; current != nullptr; current = current->next) {
        stream << ' ' << current->coefficient << ' ' << current->exponent;
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
        long double exp = static_cast<long double>(current->exponent);
        total += coef * pow(x, exp);
    }
    return total;
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

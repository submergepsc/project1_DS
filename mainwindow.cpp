#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QLabel>
#include <QPushButton>

using namespace std;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), hasA(false), hasB(false) {
    ui->setupUi(this);

    setMinimumSize(640, 480);

    connect(ui->buildAButton, &QPushButton::clicked, this, &MainWindow::buildPolynomialA);
    connect(ui->buildBButton, &QPushButton::clicked, this, &MainWindow::buildPolynomialB);
    connect(ui->showAButton, &QPushButton::clicked, this, &MainWindow::showPolynomialA);
    connect(ui->showBButton, &QPushButton::clicked, this, &MainWindow::showPolynomialB);
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::addPolynomials);
    connect(ui->subtractButton, &QPushButton::clicked, this, &MainWindow::subtractPolynomials);
    connect(ui->evalAButton, &QPushButton::clicked, this, &MainWindow::evaluatePolynomialA);
    connect(ui->evalBButton, &QPushButton::clicked, this, &MainWindow::evaluatePolynomialB);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::appendMessage(const QString& message) {
    ui->outputArea->append(message);
}

bool MainWindow::readValue(long double& value) {
    QString text = ui->valueInput->text().trimmed();
    if (text.isEmpty()) {
        appendMessage(tr("请先输入 x 的值。"));
        return false;
    }
    bool ok = false;
    long double parsed = static_cast<long double>(text.toDouble(&ok));
    if (!ok) {
        appendMessage(tr("无法解析 x 的值，请重新输入。"));
        return false;
    }
    value = parsed;
    return true;
}

void MainWindow::displayPolynomialDetails(const QString& name, const Polynomial& polynomial) {
    appendMessage(tr("%1: %2").arg(name, polynomial.toExpressionString()));
    appendMessage(tr("%1 的稀疏序列: %2").arg(name, polynomial.toSequenceString()));
    appendMessage(QString());
}

QString MainWindow::formatNumber(long double number) const {
    return QString::number(static_cast<double>(number), 'g', 15);
}

void MainWindow::buildPolynomialA() {
    QString text = ui->inputA->text().trimmed();
    if (polynomialA.parseFromString(text)) {
        hasA = true;
        appendMessage(tr("多项式 A 构建成功。"));
        if (polynomialA.isEmpty()) {
            appendMessage(tr("多项式 A 是零多项式。"));
        }
        displayPolynomialDetails(tr("多项式 A"), polynomialA);
    } else {
        hasA = false;
        appendMessage(tr("多项式 A 输入无效，请确认使用形如 (2-3x^4+4x^34) 的表达式并按降幂排列。"));
    }
}

void MainWindow::buildPolynomialB() {
    QString text = ui->inputB->text().trimmed();
    if (polynomialB.parseFromString(text)) {
        hasB = true;
        appendMessage(tr("多项式 B 构建成功。"));
        if (polynomialB.isEmpty()) {
            appendMessage(tr("多项式 B 是零多项式。"));
        }
        displayPolynomialDetails(tr("多项式 B"), polynomialB);
    } else {
        hasB = false;
        appendMessage(tr("多项式 B 输入无效，请确认使用形如 (2-3x^4+4x^34) 的表达式并按降幂排列。"));
    }
}

void MainWindow::showPolynomialA() {
    if (!hasA) {
        appendMessage(tr("请先建立多项式 A。"));
        return;
    }
    displayPolynomialDetails(tr("多项式 A"), polynomialA);
}

void MainWindow::showPolynomialB() {
    if (!hasB) {
        appendMessage(tr("请先建立多项式 B。"));
        return;
    }
    displayPolynomialDetails(tr("多项式 B"), polynomialB);
}

void MainWindow::addPolynomials() {
    if (!hasA || !hasB) {
        appendMessage(tr("请先建立多项式 A 和 B。"));
        return;
    }
    Polynomial result = polynomialA.add(polynomialB);
    displayPolynomialDetails(tr("A + B"), result);
}

void MainWindow::subtractPolynomials() {
    if (!hasA || !hasB) {
        appendMessage(tr("请先建立多项式 A 和 B。"));
        return;
    }
    Polynomial result = polynomialA.subtract(polynomialB);
    displayPolynomialDetails(tr("A - B"), result);
}

void MainWindow::evaluatePolynomialA() {
    if (!hasA) {
        appendMessage(tr("请先建立多项式 A。"));
        return;
    }
    long double value = 0.0L;
    if (!readValue(value)) {
        return;
    }
    long double result = polynomialA.evaluate(value);
    appendMessage(tr("A(%1) = %2").arg(formatNumber(value), formatNumber(result)));
    appendMessage(QString());
}

void MainWindow::evaluatePolynomialB() {
    if (!hasB) {
        appendMessage(tr("请先建立多项式 B。"));
        return;
    }
    long double value = 0.0L;
    if (!readValue(value)) {
        return;
    }
    long double result = polynomialB.evaluate(value);
    appendMessage(tr("B(%1) = %2").arg(formatNumber(value), formatNumber(result)));
    appendMessage(QString());
}

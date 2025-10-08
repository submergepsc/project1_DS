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

void MainWindow::buildPolynomialA() {
    QString text = ui->inputA->text();
    if (polynomialA.parseFromString(text)) {
        hasA = true;
        appendMessage(tr("多项式 A 构建成功: %1").arg(polynomialA.toExpressionString()));
    } else {
        hasA = false;
        appendMessage(tr("多项式 A 输入无效。"));
    }
}

void MainWindow::buildPolynomialB() {
    QString text = ui->inputB->text();
    if (polynomialB.parseFromString(text)) {
        hasB = true;
        appendMessage(tr("多项式 B 构建成功: %1").arg(polynomialB.toExpressionString()));
    } else {
        hasB = false;
        appendMessage(tr("多项式 B 输入无效。"));
    }
}

void MainWindow::showPolynomialA() {
    if (!hasA) {
        appendMessage(tr("请先建立多项式 A。"));
        return;
    }
    appendMessage(tr("多项式 A: %1").arg(polynomialA.toExpressionString()));
}

void MainWindow::showPolynomialB() {
    if (!hasB) {
        appendMessage(tr("请先建立多项式 B。"));
        return;
    }
    appendMessage(tr("多项式 B: %1").arg(polynomialB.toExpressionString()));
}

void MainWindow::addPolynomials() {
    if (!hasA || !hasB) {
        appendMessage(tr("请先建立多项式 A 和 B。"));
        return;
    }
    Polynomial result = polynomialA.add(polynomialB);
    appendMessage(tr("A + B: %1").arg(result.toExpressionString()));
}

void MainWindow::subtractPolynomials() {
    if (!hasA || !hasB) {
        appendMessage(tr("请先建立多项式 A 和 B。"));
        return;
    }
    Polynomial result = polynomialA.subtract(polynomialB);
    appendMessage(tr("A - B: %1").arg(result.toExpressionString()));
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
    appendMessage(tr("A(%1) = %2").arg(QString::number(static_cast<double>(value)),
                                     QString::number(static_cast<double>(result))));
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
    appendMessage(tr("B(%1) = %2").arg(QString::number(static_cast<double>(value)),
                                     QString::number(static_cast<double>(result))));
}

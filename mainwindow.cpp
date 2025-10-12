#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , hasPolynomial(false) {
    ui->setupUi(this);

    setMinimumSize(420, 360);

    connect(ui->parseButton, &QPushButton::clicked, this, &MainWindow::parsePolynomial);
    connect(ui->evaluateButton, &QPushButton::clicked, this, &MainWindow::evaluatePolynomial);
    connect(ui->polynomialInput, &QLineEdit::returnPressed, this, &MainWindow::parsePolynomial);
    connect(ui->valueInput, &QLineEdit::returnPressed, this, &MainWindow::evaluatePolynomial);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::appendMessage(const QString& message) {
    ui->outputView->appendPlainText(message);
}

void MainWindow::showPolynomialDetails() {
    appendMessage(tr("表达式: %1").arg(polynomial.toExpressionString()));
    appendMessage(tr("稀疏序列: %1").arg(polynomial.toSequenceString()));
    appendMessage(QString());
}

QString MainWindow::formatNumber(long double value) const {
    return QString::number(static_cast<double>(value), 'g', 15);
}

void MainWindow::parsePolynomial() {
    const QString text = ui->polynomialInput->text().trimmed();

    QString errorMessage;
    if (polynomial.parseFromString(text, &errorMessage)) {
        hasPolynomial = true;
        ui->outputView->clear();
        if (polynomial.isZero()) {
            appendMessage(tr("输入解析为零多项式。"));
        } else {
            appendMessage(tr("多项式解析成功。"));
        }
        showPolynomialDetails();
    } else {
        hasPolynomial = false;
        appendMessage(tr("解析失败: %1").arg(errorMessage));
    }
}

void MainWindow::evaluatePolynomial() {
    if (!hasPolynomial) {
        appendMessage(tr("请先解析多项式。"));
        return;
    }

    const QString valueText = ui->valueInput->text().trimmed();
    if (valueText.isEmpty()) {
        appendMessage(tr("请输入 x 的取值。"));
        return;
    }

    bool ok = false;
    const long double xValue = static_cast<long double>(valueText.toDouble(&ok));
    if (!ok) {
        appendMessage(tr("无法解析 x 的取值，请重新输入。"));
        return;
    }

    const long double result = polynomial.evaluate(xValue);
    appendMessage(tr("P(%1) = %2").arg(formatNumber(xValue), formatNumber(result)));
    appendMessage(QString());
}

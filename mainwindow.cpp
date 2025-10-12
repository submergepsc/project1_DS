#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , hasResult(false)
{
    ui->setupUi(this);

    setMinimumSize(520, 420);

    connect(ui->computeButton, &QPushButton::clicked, this, &MainWindow::generateResult);
    connect(ui->polyAInput, &QLineEdit::returnPressed, this, &MainWindow::generateResult);
    connect(ui->polyBInput, &QLineEdit::returnPressed, this, &MainWindow::generateResult);
    connect(ui->evaluateButton, &QPushButton::clicked, this, &MainWindow::evaluateResult);
    connect(ui->valueInput, &QLineEdit::returnPressed, this, &MainWindow::evaluateResult);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::appendMessage(const QString& message)
{
    ui->outputView->appendPlainText(message);
}

QString MainWindow::formatNumber(long double value) const
{
    return QString::number(static_cast<double>(value), 'g', 15);
}

void MainWindow::generateResult()
{
    QString errorMessage;
    Polynomial candidateA;
    Polynomial candidateB;

    if (!candidateA.parseSequence(ui->polyAInput->text(), &errorMessage)) {
        ui->outputView->clear();
        appendMessage(tr("解析多项式 A 失败: %1").arg(errorMessage));
        hasResult = false;
        return;
    }

    if (!candidateB.parseSequence(ui->polyBInput->text(), &errorMessage)) {
        ui->outputView->clear();
        appendMessage(tr("解析多项式 B 失败: %1").arg(errorMessage));
        hasResult = false;
        return;
    }

    polynomialA = candidateA;
    polynomialB = candidateB;
    polynomialC = Polynomial::add(polynomialA, polynomialB);

    ui->outputView->clear();

    appendMessage(tr("多项式 A (逆序输入): %1").arg(polynomialA.toSequenceString(Polynomial::SequenceOrder::Descending)));
    appendMessage(tr("多项式 B (逆序输入): %1").arg(polynomialB.toSequenceString(Polynomial::SequenceOrder::Descending)));
    appendMessage(tr("多项式 C(x) = %1").arg(polynomialC.toExpressionString()));
    appendMessage(tr("C 的顺序稀疏序列: %1").arg(polynomialC.toSequenceString(Polynomial::SequenceOrder::Ascending)));
    appendMessage(QString());

    hasResult = true;
}

void MainWindow::evaluateResult()
{
    if (!hasResult) {
        appendMessage(tr("请先生成多项式 C。"));
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

    const long double result = polynomialC.evaluate(xValue);
    appendMessage(tr("C(%1) = %2").arg(formatNumber(xValue), formatNumber(result)));
    appendMessage(QString());
}

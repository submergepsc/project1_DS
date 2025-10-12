#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

#include "polynomial.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void generateResult();
    void evaluateResult();

private:
    Ui::MainWindow* ui;
    Polynomial polynomialA;
    Polynomial polynomialB;
    Polynomial polynomialC;
    bool hasResult;

    void appendMessage(const QString& message);
    QString formatNumber(long double value) const;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "polynomial.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void buildPolynomialA();
    void buildPolynomialB();
    void showPolynomialA();
    void showPolynomialB();
    void addPolynomials();
    void subtractPolynomials();
    void evaluatePolynomialA();
    void evaluatePolynomialB();

private:
    Ui::MainWindow* ui;
    Polynomial polynomialA;
    Polynomial polynomialB;
    bool hasA;
    bool hasB;

    void appendMessage(const QString& message);
    bool readValue(long double& value);
};

#endif // MAINWINDOW_H

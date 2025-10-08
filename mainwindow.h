#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

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
    void displayPolynomialDetails(const QString& name, const Polynomial& polynomial);
    QString formatNumber(long double number) const;
};

#endif // MAINWINDOW_H

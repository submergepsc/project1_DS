#include <QApplication>

#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("简易多项式计算器"));
    QApplication::setApplicationVersion(QStringLiteral("2.0"));

    MainWindow window;
    window.show();

    return app.exec();
}

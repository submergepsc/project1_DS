#include <QApplication>
#include <QCoreApplication>
#include <QString>

#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("一元稀疏多项式简单计算器"));
    QCoreApplication::setApplicationVersion(QStringLiteral("1.0"));
    MainWindow window;
    window.show();
    return app.exec();
}

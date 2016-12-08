#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QFile styles(":/assets/styles.qss");
    styles.open(QFile::ReadOnly);
    QString styleString = QLatin1String(styles.readAll());

    QApplication a(argc, argv);
    a.setStyleSheet(styleString);
    MainWindow w;
    w.show();
    return a.exec();
}

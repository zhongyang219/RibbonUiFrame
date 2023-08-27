
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationVersion("1.0.0");
    a.setWindowIcon(QIcon(":/res/logo.ico"));

    MainWindow w;
    w.show();

    return a.exec();
}

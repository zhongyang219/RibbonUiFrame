
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "windows:darkmode=2");
    QApplication a(argc, argv);
    a.setApplicationVersion("1.0.0");
    MainWindow w;
    w.show();

    return a.exec();
}

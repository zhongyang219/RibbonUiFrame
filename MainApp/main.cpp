
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
//    qputenv("QT_QPA_PLATFORM", "windows:darkmode=2");

    //Linux下禁用Qt自带的高DPI缩放，而是使用StylePlugin提供的高DPI缩放方案，避免高DPI下模糊的问题
#if ((defined Q_OS_LINUX) && (QT_VERSION >= QT_VERSION_CHECK(5,6,0)))
    QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
#endif

    QApplication a(argc, argv);
    a.setApplicationVersion("1.0.0");

    MainWindow w;
    w.show();

    return a.exec();
}

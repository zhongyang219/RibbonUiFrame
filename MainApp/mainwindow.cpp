#include "mainwindow.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QApplication>

#define QSTR(str) QString::fromWCharArray(L ## str)

MainWindow::MainWindow(QWidget *parent) : RibbonFrameWindow(parent)
{
    QIcon appIcon(":/res/logo.png");
    setWindowIcon(appIcon);
    SetItemIcon("AppAbout", appIcon);
}


bool MainWindow::OnCommand(const QString &strCmd, bool checked)
{
    if (strCmd == "AppAbout")
    {
        QMessageBox::about(this, QSTR("关于 %1").arg(qApp->applicationName()), QSTR("%1 %2\r\n这是一个界面框架示例程序。\r\nCopyright(C) 2023 by ZhongYang").arg(qApp->applicationName()).arg(qApp->applicationVersion()));
        return true;
    }
    return RibbonFrameWindow::OnCommand(strCmd, checked);
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    RibbonFrameWindow::closeEvent(event);

    //QMessageBox box(QMessageBox::Question, QString(), QSTR("确实要退出吗？"), QMessageBox::Yes | QMessageBox::No, this);
    //if (box.exec() != QMessageBox::Yes)
    //    event->ignore();
}

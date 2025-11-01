#include "mainwindow.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QApplication>
#include <QScreen>
#include "toolkits/Config.h"

inline int DPI(int x)
{
    return QGuiApplication::primaryScreen()->logicalDotsPerInch() * x / 96;
}

MainWindow::MainWindow(QWidget *parent, const QStringList& cmdLine)
    : RibbonFrameWindow(parent, QString(), false, cmdLine)
{
    QIcon appIcon = windowIcon();
    if (appIcon.isNull())
    {
        appIcon = QIcon(":/res/logo.png");
        setWindowIcon(appIcon);
        SetItemIcon("AppAbout", appIcon);
    }

    //载入配置
    CConfig settings;
    int tabIndex = settings.GetValue("tabIndex").toInt();
    SetTabIndex(tabIndex);
    int windowWidth = settings.GetValue("windowWidth", DPI(800)).toInt();
    int windowHeight = settings.GetValue("windowHeight", DPI(600)).toInt();
    resize(QSize(windowWidth, windowHeight));

}

MainWindow::~MainWindow()
{
    //保存配置
    CConfig settings;
    settings.WriteValue("tabIndex", GetTabIndex());
    if (!isMaximized())
    {
        QSize windowSize = size();
        settings.WriteValue("windowWidth", windowSize.width());
        settings.WriteValue("windowHeight", windowSize.height());
    }
}

bool MainWindow::OnCommand(const QString &strCmd, bool checked)
{
    if (!RibbonFrameWindow::OnCommand(strCmd, checked))
    {
        if (strCmd == "AppAbout")
        {
            QMessageBox::about(this, QString(u8"关于 %1").arg(qApp->applicationName()), QString(u8"%1 %2\r\n这是一个界面框架示例程序。\r\nCopyright(C) 2023 by ZhongYang").arg(qApp->applicationName()).arg(qApp->applicationVersion()));
            return true;
        }
    }
    return false;
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    RibbonFrameWindow::closeEvent(event);

    //QMessageBox box(QMessageBox::Question, QString(), QSTR("确实要退出吗？"), QMessageBox::Yes | QMessageBox::No, this);
    //if (box.exec() != QMessageBox::Yes)
    //    event->ignore();
}

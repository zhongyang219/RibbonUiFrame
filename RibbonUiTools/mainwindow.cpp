#include "mainwindow.h"
#include <QTabBar>
#include <QTimer>
#include "ribbonuipredefine.h"

static MainWindow* pIns;

////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent)
    : RibbonFrameWindow(parent, "://res/MainFrame.xml", true)
{
    pIns = this;
    InitUi();
    SetDefaultWidget(&m_generateResIdDeninesWidget);
    resize(DPI(600), DPI(500));

    //获取TabWidget
    m_pTabWidget = findChild<QTabWidget*>("MainFrameTab");

    QTimer::singleShot(50, this, SLOT(HideTabArea()));

    m_generateResIdDeninesWidget.LoadConfig();
}

MainWindow::~MainWindow()
{
    m_generateResIdDeninesWidget.SaveConfig();
}

MainWindow *MainWindow::Instance()
{
    return pIns;
}

void MainWindow::HideTabArea()
{
    //将TabWidget的高度设置为标签的高度，即隐藏Tab功能区，只保留标签
    if (m_pTabWidget != nullptr)
        m_pTabWidget->setMaximumHeight(m_pTabWidget->tabBar()->height());
}


void *MainWindow::SendModuleMessage(const char *moduleName, const char *msgType, void *para1, void *para2)
{
    Q_UNUSED(para2)
    if (QString(msgType) == MODULE_MSG_StyleChanged)
    {
        //主题更改时重新设置TabWidget的高度
        HideTabArea();
    }
    return RibbonFrameWindow::SendModuleMessage(moduleName, msgType, para1);
}

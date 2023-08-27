#include "testmodule.h"
#include <QMainWindow>
#include <QApplication>
#include <QMessageBox>
#include <QToolButton>
#include <QComboBox>
#include "mainframeinterface.h"
#include <QScreen>
#include <QListWidget>
#include <QDateEdit>
#include <QAction>

#define QSTR(str) QString::fromWCharArray(L ## str)
//将一个像素值根据当前屏幕DPI设置进行等比放大
static int DPI(int x)
{
    return QGuiApplication::primaryScreen()->logicalDotsPerInch() * x / 96;
}

TestModule::TestModule()
{
}


void TestModule::InitInstance()
{
}

void TestModule::UnInitInstance()
{
}

void TestModule::UiInitComplete(IMainFrame *pMainFrame)
{
    m_pMainFrame = pMainFrame;

    QListWidget* pListWidget = qobject_cast<QListWidget*>((QWidget*)pMainFrame->GetWidget("ListWidget1"));
    if (pListWidget != nullptr)
    {
        pListWidget->setMaximumHeight(DPI(40));
        QIcon icon(":/res/Template.png");
        pListWidget->addItem(new QListWidgetItem(icon, QSTR("项目1")));
        pListWidget->addItem(new QListWidgetItem(icon, QSTR("项目2")));
        pListWidget->addItem(new QListWidgetItem(icon, QSTR("项目3")));
        pListWidget->addItem(new QListWidgetItem(icon, QSTR("项目4")));
    }
}

void* TestModule::GetMainWindow()
{
    return &m_mainWidget;
}

const char* TestModule::GetModuleName()
{
    return "TestModule";
}

void TestModule::OnCommand(const char* strCmd, bool checked)
{
    QString itemText = QString::fromUtf8(m_pMainFrame->GetItemText(strCmd));
    QString str = QSTR("你点击了按钮“%1”，id=%2，checked=%3").arg(itemText).arg(strCmd).arg(checked);
    QMessageBox::information(&m_mainWidget, QString(), str, QMessageBox::Ok);
}

IModule* CreateInstance()
{
    return new TestModule();
}

void* TestModule::CreateUserWidget(const char* strId, void* pParent)
{
    //初始化自定义控件
    if (QString::fromUtf8(strId) == "UserWidget1")
    {
        QDateEdit* pWidget = new QDateEdit((QWidget*)pParent);
        pWidget->setCalendarPopup(true);
        return pWidget;
    }
    return nullptr;
}

IModule::eMainWindowType TestModule::GetMainWindowType() const
{
    return IModule::MT_QWIDGET;
}

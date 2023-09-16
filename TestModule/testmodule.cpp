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
#include "ribbonuipredefine.h"

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
    pMainFrame->SetItemEnable("TestCommand13", false);
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
    //QMessageBox::information(&m_mainWidget, QString(), str, QMessageBox::Ok);
    m_pMainFrame->SetStatusBarText(str.toUtf8().constData(), 15000);
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

void TestModule::OnItemChanged(const char* strId, int index, const char* text)
{
    QString str = QSTR("触发控件消息：id=%1，index=%2，text=%3").arg(QString::fromUtf8(strId)).arg(index).arg(QString::fromUtf8(text));
    m_pMainFrame->SetStatusBarText(str.toUtf8().constData(), 15000);
}

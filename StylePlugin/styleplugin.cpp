﻿#include "styleplugin.h"
#include <QApplication>
#include <QMenu>
#include <QMessageBox>
#include <QToolButton>
#include "mainframeinterface.h"
#include "stylemanager.h"
#include <QSettings>
#include <QActionGroup>

#define QSTR(str) QString::fromWCharArray(L ## str)
#define SCOPE_NAME "Apps By ZhongYang"

StylePlugin::StylePlugin()
{
}


void StylePlugin::InitInstance()
{
    //载入设置
    QSettings settings(SCOPE_NAME, qApp->applicationName());
    m_curStyle = settings.value("style").toString();
}

void StylePlugin::UiInitComplete(IMainFrame *pMainFrame)
{
    m_pMainFrame = pMainFrame;
    QActionGroup* pGroup = new QActionGroup(this);
    if (pMainFrame != nullptr)
    {
        pGroup->addAction((QAction*)pMainFrame->GetAcion("DefaultStyle"));
        //查找“主题”菜单按钮
        QToolButton* pThemeBtn = qobject_cast<QToolButton*>((QWidget*)pMainFrame->GetWidget("Theme"));
        if (pThemeBtn != nullptr)
        {
            QMenu* pMenu = pThemeBtn->menu();
            if (pMenu != nullptr)
            {
                QMenu* pLightThemeMenu = pMenu->addMenu(QIcon(":/res/light_mode.png"), QSTR("浅色主题"));
                QMenu* pDarkThemeMenu = pMenu->addMenu(QIcon(":/res/dark_mode.png"), QSTR("深色主题"));
                QMenu* pOfficeThemeMenu = pMenu->addMenu(QIcon(":/res/office.png"), QSTR("Office主题"));
                //向“主题”菜单中添加Action
                Q_FOREACH(const auto& style, CStyleManager::Instance()->GetAllStyles())
                {
                    QAction* pAction = nullptr;
                    if (style.m_type == CStyleManager::CStyle::Dark)
                        pAction = AddThemeAction(style.m_strName, pDarkThemeMenu);
                    else if (style.m_type == CStyleManager::CStyle::Light)
                        pAction = AddThemeAction(style.m_strName, pLightThemeMenu);
                    else if (style.m_type == CStyleManager::CStyle::Office)
                        pAction = AddThemeAction(style.m_strName, pOfficeThemeMenu);

                    pGroup->addAction(pAction);
                }
            }
        }
    }

    //保存默认Palette
    m_defaultPalette = qApp->palette();

    //设置样式
    if (m_curStyle.isEmpty())
    {
        pMainFrame->SetItemChecked("DefaultStyle", true);
    }
    else
    {
        CStyleManager::Instance()->ApplyStyleSheet(m_curStyle);
        Q_FOREACH(QAction* pAction, m_themeActionList)
        {
            if (pAction != nullptr && pAction->text() == m_curStyle)
                pAction->setChecked(true);
        }
    }
}

void StylePlugin::UnInitInstance()
{
    //保存设置
    QSettings settings(SCOPE_NAME, qApp->applicationName());
    settings.setValue("style", m_curStyle);
}

IModule::eMainWindowType StylePlugin::GetMainWindowType() const
{
    return IModule::MT_QWIDGET;
}

void* StylePlugin::GetMainWindow()
{
    return nullptr;
}

const char *StylePlugin::GetModuleName()
{
    return "StylePlugin";
}

void StylePlugin::OnCommand(const char* strCmd, bool checked)
{
    Q_UNUSED(checked)
    if (QString(strCmd) == "DefaultStyle")
    {
        SetStyle(QString());
    }
}

void *StylePlugin::OnMessage(const char *msgType, void *para1, void *para2)
{
    Q_UNUSED(para1)
    if (QString(msgType) == "GetStyleType")
    {
        CStyleManager::CStyle* pStyle = CStyleManager::Instance()->GetStyle(m_curStyle);
        if (pStyle != nullptr)
            return (void*)pStyle->m_type;
    }
    else if (QString(msgType) == "GetStyleName")
    {
        static QByteArray styleName;
        styleName = m_curStyle.toUtf8();
        return (void*)styleName.constData();
    }
    return nullptr;
}

QAction *StylePlugin::AddThemeAction(const QString &name, QMenu *pMenu)
{
    QAction* pAction = pMenu->addAction(name);
    pAction->setCheckable(true);
    connect(pAction, &QAction::triggered, this, &StylePlugin::OnStyleActionTriggered);
    m_themeActionList.push_back(pAction);
    return pAction;
}

void StylePlugin::SetStyle(const QString &styleName)
{
    if (styleName != m_curStyle)
    {
        if (styleName.isEmpty())
        {
            qApp->setPalette(m_defaultPalette);
            qApp->setStyleSheet("");
        }
        else
        {
            CStyleManager::Instance()->ApplyStyleSheet(styleName);
        }
        m_curStyle = styleName;
    }
}

void StylePlugin::OnStyleActionTriggered(bool)
{
    QAction* pAction = qobject_cast<QAction*>(QObject::sender());
    if (pAction != nullptr)
    {
        SetStyle(pAction->text());
    }
}

IModule* CreateInstance()
{
    return new StylePlugin();

}

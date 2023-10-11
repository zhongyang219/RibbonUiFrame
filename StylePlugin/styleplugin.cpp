#include "styleplugin.h"
#include <QApplication>
#include <QMenu>
#include <QMessageBox>
#include <QToolButton>
#include "mainframeinterface.h"
#include "stylemanager.h"
#include <QSettings>
#include <QActionGroup>
#include <QStyleFactory>
#include "ribbonuipredefine.h"

#ifdef Q_OS_WIN
#define DEFAULT_STYLE_KEY "windowsvista"
#else
#define DEFAULT_STYLE_KEY "Fusion"
#endif

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
        //默认主题
        pGroup->addAction((QAction*)pMainFrame->GetAcion(CMD_DefaultStyle));
        //查找“主题”菜单按钮
        QToolButton* pThemeBtn = qobject_cast<QToolButton*>((QWidget*)pMainFrame->GetWidget("Theme"));
        if (pThemeBtn != nullptr)
        {
            QMenu* pMenu = pThemeBtn->menu();
            if (pMenu != nullptr)
            {
                //添加当前平台支持的主题
                QMenu* pPlatformSupportedTheme = pMenu->addMenu(QSTR("平台支持的主题"));
                for (const auto& key : QStyleFactory::keys())
                {
                    pGroup->addAction(AddThemeAction(key, pPlatformSupportedTheme));
                }

                //更多主题
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

    qApp->setPalette(m_defaultPalette);
    qApp->setStyle(QStyleFactory::create(DEFAULT_STYLE_KEY));
    //设置样式
    if (m_curStyle.isEmpty())
    {
        pMainFrame->SetItemChecked(CMD_DefaultStyle, true);
    }
    else
    {
        if (QStyleFactory::keys().contains(m_curStyle))
            qApp->setStyle(QStyleFactory::create(m_curStyle));
        else
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
    if (QString(strCmd) == CMD_DefaultStyle)
    {
        SetStyle(QString());
    }
}

void *StylePlugin::OnMessage(const char *msgType, void *para1, void *para2)
{
    Q_UNUSED(para1)
    QString strMsgType = msgType;
    if (strMsgType == "GetStyleType")
    {
        CStyleManager::CStyle* pStyle = CStyleManager::Instance()->GetStyle(m_curStyle);
        if (pStyle != nullptr)
            return (void*)pStyle->m_type;
    }
    else if (strMsgType == "GetStyleName")
    {
        static QByteArray styleName;
        styleName = m_curStyle.toUtf8();
        return (void*)styleName.constData();
    }
    else if (strMsgType == "IsDarkTheme")
    {
        CStyleManager::CStyle* pStyle = CStyleManager::Instance()->GetStyle(m_curStyle);
        return (void*)((pStyle != nullptr && pStyle->m_type == CStyleManager::CStyle::Dark) || m_curStyle == u8"Office2016深色");
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
            qApp->setStyle(QStyleFactory::create(DEFAULT_STYLE_KEY));
        }
        else if (QStyleFactory::keys().contains(styleName))
        {
            qApp->setPalette(m_defaultPalette);
            qApp->setStyleSheet("");
            qApp->setStyle(QStyleFactory::create(styleName));
        }
        else
        {
            qApp->setStyle(QStyleFactory::create(DEFAULT_STYLE_KEY));
            CStyleManager::Instance()->ApplyStyleSheet(styleName);
        }
        m_curStyle = styleName;
        QByteArray curStyleData = m_curStyle.toUtf8();
        m_pMainFrame->SendModuleMessage(nullptr, MODULE_MSG_StyleChanged, (void*)curStyleData.constData());
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

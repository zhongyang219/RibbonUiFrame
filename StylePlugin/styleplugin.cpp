﻿#include "styleplugin.h"
#include <QApplication>
#include <QMenu>
#include <QMessageBox>
#include <QToolButton>
#include "mainframeinterface.h"
#include "stylemanager.h"
#include <QSettings>
#include <QActionGroup>
#include <QStyleFactory>
#include <QColorDialog>
#include "ribbonuipredefine.h"
#include <QTimerEvent>

#ifdef Q_OS_WIN
#define DEFAULT_STYLE_KEY "windowsvista"
#include <dwmapi.h>
#pragma comment(lib,"Dwmapi.lib")
#else
#define DEFAULT_STYLE_KEY "Fusion"
#endif

#define DEFAULT_THEME_COLOR_MS_WORD "#2b579a"
#define DEFAULT_THEME_COLOR_MS_EXCEL "#2a724b"
#define DEFAULT_THEME_COLOR_POWERPOINT "#b7472a"
#define DEFAULT_THEME_COLOR_ONENOTE "#80397b"

static QIcon CreateSingleColorIcon(const QColor& color)
{
    QPixmap pixmap(DPI(16), DPI(16));
    pixmap.fill(color);
    return QIcon(pixmap);
}

static QColor GetWindowsThemeColor()
{
#ifdef Q_OS_WIN
    DWORD crColorization;
    BOOL fOpaqueBlend;
    QColor themeColor;
    HRESULT result = DwmGetColorizationColor(&crColorization, &fOpaqueBlend);
    if (result == S_OK)
    {
        BYTE r, g, b;
        r = (crColorization >> 16) % 256;
        g = (crColorization >> 8) % 256;
        b = crColorization % 256;
        themeColor.setRed(r);
        themeColor.setGreen(g);
        themeColor.setBlue(b);
    }
    return themeColor;
#else
    return QColor();
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
StylePlugin::StylePlugin()
{
}


void StylePlugin::InitInstance()
{
    m_timerId = startTimer(1000);

    //载入设置
    QSettings settings(SCOPE_NAME, qApp->applicationName());
    m_curStyle = settings.value("style", u8"Office2016彩色").toString();
    QString strThemeColor = settings.value("themeColor", DEFAULT_THEME_COLOR_MS_WORD).toString();
    m_themeColor.SetColor(QColor(strThemeColor));

    m_followSystemColorAction = new QAction(u8"跟随系统主题颜色");
    m_followSystemColorAction->setCheckable(true);
    m_followSystemColorAction->setChecked(settings.value("followSystemThemeColor").toInt());
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

                //主题颜色
                pMenu->addSeparator();
                QMenu* pThemeColorMenu = pMenu->addMenu(QSTR("主题颜色"));
                pThemeColorMenu->addAction(CreateSingleColorIcon(QColor(DEFAULT_THEME_COLOR_MS_WORD)), QSTR("Microsoft Word"), this, SLOT(OnThemeColorWord()));
                pThemeColorMenu->addAction(CreateSingleColorIcon(QColor(DEFAULT_THEME_COLOR_MS_EXCEL)), QSTR("Microsoft Excel"), this, SLOT(OnThemeColorExcel()));
                pThemeColorMenu->addAction(CreateSingleColorIcon(QColor(DEFAULT_THEME_COLOR_POWERPOINT)), QSTR("Microsoft PowerPoint"), this, SLOT(OnThemeColorPowerPoint()));
                pThemeColorMenu->addAction(CreateSingleColorIcon(QColor(DEFAULT_THEME_COLOR_ONENOTE)), QSTR("Microsoft OneNote"), this, SLOT(OnThemeColorOneNote()));
                pThemeColorMenu->addAction(QSTR("自定义主题色..."), this, SLOT(OnCustomThemeColor()));
                pThemeColorMenu->addAction(m_followSystemColorAction);
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
#ifdef Q_OS_WIN
            CStyleManager::Instance()->m_defaultStyle.ApplyStyleSheet();
#endif
    }
    else
    {
        if (QStyleFactory::keys().contains(m_curStyle))
            qApp->setStyle(QStyleFactory::create(m_curStyle));
        else
            CStyleManager::Instance()->ApplyStyleSheet(m_curStyle, nullptr, &m_themeColor);
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
    settings.setValue("themeColor", m_themeColor.OriginalColor().name());
    settings.setValue("followSystemThemeColor", static_cast<int>(m_followSystemColorAction->isChecked()));
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
    QString strMsgType = msgType;
    if (strMsgType == MODULE_MSG_GetStyleType)
    {
        CStyleManager::CStyle* pStyle = CStyleManager::Instance()->GetStyle(m_curStyle);
        if (pStyle != nullptr)
            return (void*)pStyle->m_type;
    }
    else if (strMsgType == MODULE_MSG_GetStyleName)
    {
        static QByteArray styleName;
        styleName = m_curStyle.toUtf8();
        return (void*)styleName.constData();
    }
    else if (strMsgType == MODULE_MSG_IsDarkTheme)
    {
        CStyleManager::CStyle* pStyle = CStyleManager::Instance()->GetStyle(m_curStyle);
        return (void*)((pStyle != nullptr && pStyle->m_type == CStyleManager::CStyle::Dark) || m_curStyle == u8"Office2016深色");
    }
    else if (strMsgType == MODULE_MSG_SetThemeColor)
    {
        QColor themeColor((const char*)para1);
        SetThemeColor(themeColor);
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
#ifdef Q_OS_WIN
            CStyleManager::Instance()->m_defaultStyle.ApplyStyleSheet();
#else
            qApp->setStyleSheet("");
#endif
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
            CStyleManager::Instance()->ApplyStyleSheet(styleName, nullptr, &m_themeColor);
        }
        m_curStyle = styleName;
        QByteArray curStyleData = m_curStyle.toUtf8();
        m_pMainFrame->SendModuleMessage(nullptr, MODULE_MSG_StyleChanged, (void*)curStyleData.constData());
    }
}

void StylePlugin::SetThemeColor(const QColor &color)
{
    if (m_themeColor.OriginalColor() != color)
    {
        m_themeColor.SetColor(color);
        CStyleManager::Instance()->ApplyStyleSheet(m_curStyle, nullptr, &m_themeColor);
    }
}

void StylePlugin::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == m_timerId)
    {
#ifdef Q_OS_WIN
        if (m_followSystemColorAction->isChecked())
        {
            QColor systemThemeColor = GetWindowsThemeColor();
            SetThemeColor(systemThemeColor);
        }
#endif
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

void StylePlugin::OnThemeColorWord()
{
    SetThemeColor(QColor(DEFAULT_THEME_COLOR_MS_WORD));
    m_followSystemColorAction->setChecked(false);
}

void StylePlugin::OnThemeColorExcel()
{
    SetThemeColor(QColor(DEFAULT_THEME_COLOR_MS_EXCEL));
    m_followSystemColorAction->setChecked(false);
}

void StylePlugin::OnThemeColorPowerPoint()
{
    SetThemeColor(QColor(DEFAULT_THEME_COLOR_POWERPOINT));
    m_followSystemColorAction->setChecked(false);
}

void StylePlugin::OnThemeColorOneNote()
{
    SetThemeColor(QColor(DEFAULT_THEME_COLOR_ONENOTE));
    m_followSystemColorAction->setChecked(false);
}

void StylePlugin::OnCustomThemeColor()
{
    QColorDialog colorDlg(m_themeColor.OriginalColor(), dynamic_cast<QWidget*>(m_pMainFrame));
    if (colorDlg.exec() == QDialog::Accepted)
    {
        SetThemeColor(colorDlg.currentColor());
        m_followSystemColorAction->setChecked(false);
    }
}

IModule* CreateInstance()
{
    return new StylePlugin();

}

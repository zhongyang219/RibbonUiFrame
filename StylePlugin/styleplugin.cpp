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
#include <QColorDialog>
#include "ribbonuipredefine.h"
#include <QTimerEvent>
#include <QProcess>

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

static bool IsWindowsDarkColorMode()
{
#ifdef Q_OS_WIN
    bool windows10_light_theme = false;
    HKEY hKey;
    DWORD dwThemeData(0);
    LONG lRes = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", 0, KEY_READ, &hKey);
    if (lRes == ERROR_SUCCESS)
    {
        DWORD dwBufferSize(sizeof(DWORD));
        DWORD dwResult(0);
        lRes = RegQueryValueExW(hKey, L"SystemUsesLightTheme", NULL, NULL, reinterpret_cast<LPBYTE>(&dwResult), &dwBufferSize);
        if (lRes == ERROR_SUCCESS)
            dwThemeData = dwResult;
        windows10_light_theme = (dwThemeData != 0);
    }
    RegCloseKey(hKey);
    return !windows10_light_theme;
#elif defined Q_OS_LINUX
    QProcess process;
    process.start("gsettings", QStringList() << "get" << "org.gnome.desktop.interface" << "gtk-theme");
    process.waitForFinished();
    QString output = process.readAllStandardOutput().trimmed();
    return (output.contains("dark", Qt::CaseInsensitive));
#endif
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
StylePlugin* pIns = nullptr;
 
StylePlugin::StylePlugin()
{
    // 安装本地事件过滤器
    qApp->installNativeEventFilter(&darkTitleBarFilter);
}

StylePlugin* StylePlugin::Instance()
{
    return pIns;
}

bool StylePlugin::IsDarkTheme() const
{
    CStyleManager::CStyle* pStyle = CStyleManager::Instance()->GetStyle(m_curStyle);
    return pStyle != nullptr && pStyle->m_type == CStyleManager::CStyle::Dark;
}


void StylePlugin::InitInstance()
{
    m_timerId = startTimer(2000);

    //载入设置
    QSettings settings(SCOPE_NAME, qApp->applicationName());
    m_curStyle = settings.value("style", u8"Office2016彩色").toString();
    QString strThemeColor = settings.value("themeColor", DEFAULT_THEME_COLOR_MS_WORD).toString();
    m_themeColor.SetColor(QColor(strThemeColor));

    m_followSystemColorAction = new QAction(u8"跟随系统主题颜色");
    m_followSystemColorAction->setCheckable(true);
    m_followSystemColorAction->setChecked(settings.value("followSystemThemeColor").toInt());
    m_followSystemColorModeAction = new QAction(u8"跟随Windows深色/浅色主题");
    m_followSystemColorModeAction->setCheckable(true);
    m_followSystemColorModeAction->setChecked(settings.value("followSystemColorMode").toInt());
}

void StylePlugin::UiInitComplete(IMainFrame *pMainFrame)
{
    m_pMainFrame = pMainFrame;
    m_themeActionGroup = new QActionGroup(this);
    if (pMainFrame != nullptr)
    {
        //默认主题
        m_themeActionGroup->addAction((QAction*)pMainFrame->GetAcion(CMD_DefaultStyle));
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
                    m_themeActionGroup->addAction(AddThemeAction(key, pPlatformSupportedTheme));
                }

                //向“主题”菜单中添加Action
                auto styles = CStyleManager::Instance()->GetAllStyles();
                Q_FOREACH(const auto& style, styles)
                {
                    QAction* pAction = AddThemeAction(style->m_strName, pMenu);
                    m_themeActionGroup->addAction(pAction);
                }

                pMenu->addSeparator();
                pMenu->addAction(m_followSystemColorModeAction);

                //主题颜色
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
    settings.setValue("followSystemColorMode", static_cast<int>(m_followSystemColorModeAction->isChecked()));
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
        return (void*)IsDarkTheme();
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
        //获取主窗口
        QWidget* widget = dynamic_cast<QWidget*>(m_pMainFrame);
        bool darkTheme = IsDarkTheme();
        //设置窗口标题栏颜色模式
        if (widget != nullptr)
            StyleEventFilter::SetWindowDarkTheme((void*)widget->winId(), darkTheme);

        //设置主题菜单的选中项
        auto actions = m_themeActionGroup->actions();
        for (const auto& action : actions)
        {
            if (action->text() == m_curStyle)
                action->setChecked(true);
        }
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

        if (m_followSystemColorModeAction->isChecked())
        {
            int lastDarkMode = -1;
            int isDarkMode = IsWindowsDarkColorMode();
            //系统
            if (lastDarkMode != isDarkMode)
            {
                CStyleManager::CStyle* style;
                if (isDarkMode)
                    style = CStyleManager::Instance()->GetDarkStyle(m_curStyle);
                else
                    style = CStyleManager::Instance()->GetLightStyle(m_curStyle);
                if (style != nullptr)
                {
                    if (style->m_strName != m_curStyle)
                        SetStyle(style->m_strName);
                }
                lastDarkMode = isDarkMode;
            }
        }
    }
}

void StylePlugin::OnStyleActionTriggered(bool)
{
    QAction* pAction = qobject_cast<QAction*>(QObject::sender());
    if (pAction != nullptr)
    {
        //获取选择的主题的深色/浅色类型
        CStyleManager::CStyle* style = CStyleManager::Instance()->GetStyle(pAction->text());
        bool isStyleDark = (style != nullptr && style->m_type == CStyleManager::CStyle::Dark);
        //如果选择的主题的深色/浅色类型和当前系统不一致，则取消“跟随Windows深色/浅色主题”的勾选
        if (isStyleDark != IsWindowsDarkColorMode())
            m_followSystemColorModeAction->setChecked(false);
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
    pIns = new StylePlugin();
    return pIns;
}

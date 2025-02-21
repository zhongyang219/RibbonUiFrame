#ifndef STYLEPLUGIN_H
#define STYLEPLUGIN_H

#include "StylePlugin_global.h"
#include "moduleinterface.h"
#include <QAction>
#include <QPalette>
#include "themecolor.h"
#include "StyleEventFilter.h"

class IMainFrame;

class STYLEPLUGIN_EXPORT StylePlugin
        : public QObject, public IModule
{
    Q_OBJECT
public:
    StylePlugin();
    static StylePlugin* Instance();
    bool IsDarkTheme() const;

    // IModule interface
public:
    virtual void InitInstance() override;
    virtual void UiInitComplete(IMainFrame *pMainFrame) override;
    virtual void UnInitInstance() override;
    eMainWindowType GetMainWindowType() const override ;
    virtual void* GetMainWindow() override;
    virtual const char* GetModuleName() override;
    virtual void OnCommand(const char* strCmd, bool checked) override;
    virtual void* OnMessage(const char* msgType, void* para1 = nullptr, void* para2 = nullptr) override;

private:
    QAction* AddThemeAction(const QString& name, QMenu* pMenu);
    void SetStyle(const QString& styleName);
    void SetThemeColor(const QColor& color);
    virtual void timerEvent(QTimerEvent* event) override;

private slots:
    void OnStyleActionTriggered(bool);
    void OnThemeColorWord();
    void OnThemeColorExcel();
    void OnThemeColorPowerPoint();
    void OnThemeColorOneNote();
    void OnCustomThemeColor();

private:
    QList<QAction*> m_themeActionList;
    IMainFrame* m_pMainFrame{};
    QString m_curStyle;
    QPalette m_defaultPalette;
    ThemeColor m_themeColor;
    QAction* m_followSystemColorAction{};
    int m_timerId{};
    StyleEventFilter darkTitleBarFilter;
};

#ifdef __cplusplus
extern "C" {
#endif

//导出一个名为CreateInstance的函数以创建对象
STYLEPLUGIN_EXPORT IModule* CreateInstance();

#ifdef __cplusplus
}
#endif

#endif // STYLEPLUGIN_H

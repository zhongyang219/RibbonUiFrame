#ifndef TESTMODULE_H
#define TESTMODULE_H

#include "testmodule_global.h"
#include "moduleinterface.h"

#include <QObject>
#include "mainframeinterface.h"
#include "mainwidget.h"

class TESTMODULESHARED_EXPORT TestModule
        : public QObject, public IModule
{
public:
    TestModule();

private:
    CMainWidget m_mainWidget;
    IMainFrame* m_pMainFrame;

    // IModule interface
public:
    virtual void InitInstance() override;
    virtual void UnInitInstance() override;
    virtual void UiInitComplete(IMainFrame* pMainFrame) override;
    virtual void* GetMainWindow() override;
    virtual const char* GetModuleName() override;
    virtual void OnCommand(const char* strCmd, bool checked) override;
    virtual void* CreateUserWidget(const char* strId, void* pParent = nullptr) override;
    virtual eMainWindowType GetMainWindowType() const override;
};

#ifdef __cplusplus
extern "C" {
#endif

//导出一个名为CreateInstance的函数以创建对象
TESTMODULESHARED_EXPORT IModule* CreateInstance();

#ifdef __cplusplus
}
#endif

#endif // TESTMODULE_H

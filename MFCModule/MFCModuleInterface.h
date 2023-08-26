#pragma once
#include "moduleinterface.h"

class MFCModuleInterface : public IModule
{
public:
    // 通过 IModule 继承
    virtual void InitInstance() override;
    virtual void UnInitInstance() override;
    virtual void UiInitComplete(IMainFrame* pMainFrame) override;
    virtual void* GetMainWindow() override;
    virtual eMainWindowType GetMainWindowType() const override;
    virtual const char* GetModuleName() override;
    virtual void OnCommand(const char* strCmd, bool checked) override;
};

#ifdef __cplusplus
extern "C" {
#endif

    //导出一个名为CreateInstance的函数以创建对象
    __declspec(dllexport) IModule* CreateInstance();

#ifdef __cplusplus
}
#endif

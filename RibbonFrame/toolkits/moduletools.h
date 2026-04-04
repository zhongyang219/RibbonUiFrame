#ifndef CMODULETOOLS_H
#define CMODULETOOLS_H
#include "mainframe_global.h"

class IMainFrame;
class RIBBONFRAME_EXPORT CModuleTools
{
public:
    CModuleTools();

    //获取主窗口
    static IMainFrame* GetMainWindow();

    static bool IsDarkTheme();
};

#endif // CMODULETOOLS_H

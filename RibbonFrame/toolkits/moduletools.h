#ifndef CMODULETOOLS_H
#define CMODULETOOLS_H
#include "mainframe_global.h"

class QMainWindow;
class RIBBONFRAME_EXPORT CModuleTools
{
public:
    CModuleTools();

    //获取主窗口
    static QMainWindow* GetMainWindow();

    static bool IsDarkTheme();
};

#endif // CMODULETOOLS_H

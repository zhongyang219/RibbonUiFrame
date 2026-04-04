#include "moduletools.h"
#include "mainframeinterface.h"
#include "ribbonuipredefine.h"
#include <QApplication>
#include <QWidget>

CModuleTools::CModuleTools()
{

}

IMainFrame *CModuleTools::GetMainWindow()
{
    Q_FOREACH (QWidget* w, qApp->topLevelWidgets())
    {
        IMainFrame* pMainWindow = dynamic_cast<IMainFrame*>(w);
        if (pMainWindow != nullptr)
        {
            return pMainWindow;
        }
    }
    return nullptr;

}

bool CModuleTools::IsDarkTheme()
{
    IMainFrame *pMainFrame = GetMainWindow();
    if (pMainFrame != nullptr)
    {
        return (bool)pMainFrame->SendModuleMessage("StylePlugin", MODULE_MSG_IsDarkTheme);
    }
    return false;
}

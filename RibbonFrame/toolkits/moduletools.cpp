#include "moduletools.h"
#include "mainframeinterface.h"
#include "ribbonuipredefine.h"
#include <QMainWindow>
#include <QApplication>

CModuleTools::CModuleTools()
{

}

QMainWindow *CModuleTools::GetMainWindow()
{
    Q_FOREACH (QWidget* w, qApp->topLevelWidgets())
    {
        QMainWindow* pMainWindow = qobject_cast<QMainWindow*>(w);
        if (pMainWindow != nullptr)
            return pMainWindow;
    }
    return nullptr;

}

bool CModuleTools::IsDarkTheme()
{
    IMainFrame *pMainFrame = dynamic_cast<IMainFrame*>(GetMainWindow());
    if (pMainFrame != nullptr)
    {
        return (bool)pMainFrame->SendModuleMessage("StylePlugin", MODULE_MSG_IsDarkTheme);
    }
    return false;
}

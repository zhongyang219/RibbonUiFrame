#include "pch.h"
#include "MFCModuleInterface.h"
#include "MFCModuleDlg.h"

void MFCModuleInterface::InitInstance()
{
    int nReturnCode = -1;
    CWinThread* pThread = AfxGetThread();
    CWinApp* pApp = AfxGetApp();

    // AFX internal initialization
    if (!AfxWinInit(GetModuleHandle(_T("MFCModule")), nullptr, _T(""), SW_SHOW))
        return;

    // App global initializations (rare)
    if (pApp != NULL && !pApp->InitApplication())
        return;

    // Perform specific initializations
    if (!pThread->InitInstance())
    {
        if (pThread->m_pMainWnd != NULL)
        {
            TRACE(traceAppMsg, 0, "Warning: Destroying non-NULL m_pMainWnd\n");
            pThread->m_pMainWnd->DestroyWindow();
        }
    }
    //nReturnCode = pThread->Run();
}

void MFCModuleInterface::UnInitInstance()
{
    int nReturnCode = -1;
    CWinThread* pThread = AfxGetThread();
    nReturnCode = pThread->ExitInstance();
#ifdef _DEBUG
    // Check for missing AfxLockTempMap calls
    if (AfxGetModuleThreadState()->m_nTempMapLock != 0)
    {
        TRACE(traceAppMsg, 0, "Warning: Temp map lock count non-zero (%ld).\n",
            AfxGetModuleThreadState()->m_nTempMapLock);
    }
    AfxLockTempMaps();
    AfxUnlockTempMaps(-1);
#endif

    AfxWinTerm();
}

void MFCModuleInterface::UiInitComplete(IMainFrame* pMainFrame)
{
    //显示主窗口
    CMFCModuleDlg* pDlg = dynamic_cast<CMFCModuleDlg*>(AfxGetMainWnd());
    pDlg->SetWindowVisible(true);
}

void* MFCModuleInterface::GetMainWindow()
{
    return AfxGetMainWnd()->GetSafeHwnd();
}

IModule::eMainWindowType MFCModuleInterface::GetMainWindowType() const
{
    return IModule::MT_HWND;
}

const char* MFCModuleInterface::GetModuleName()
{
    return "MFCModule";
}

void MFCModuleInterface::OnCommand(const char* strCmd, bool checked)
{
}

IModule* CreateInstance()
{
    return new MFCModuleInterface();

}

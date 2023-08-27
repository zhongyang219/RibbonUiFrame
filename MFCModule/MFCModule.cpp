
// MFCModule.cpp: 定义应用程序的类行为。
//

#include "pch.h"
#include "framework.h"
#include "MFCModule.h"
#include "MFCModuleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCModuleApp

BEGIN_MESSAGE_MAP(CMFCModuleApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMFCModuleApp 构造

CMFCModuleApp::CMFCModuleApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CMFCModuleApp 对象

CMFCModuleApp theApp;


// CMFCModuleApp 初始化

BOOL CMFCModuleApp::InitInstance()
{
	CWinApp::InitInstance();

	//// 创建 shell 管理器，以防对话框包含
	//// 任何 shell 树视图控件或 shell 列表视图控件。
	//CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	//SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

#ifdef _WINDLL
    CMFCModuleDlg* pDlg = new CMFCModuleDlg();
	m_pMainWnd = pDlg;
    pDlg->Create(IDD_MFCMODULE_DIALOG);

    return TRUE;
#else
    CMFCModuleDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
    {
        // TODO: 在此放置处理何时用
        //  “确定”来关闭对话框的代码
}
    else if (nResponse == IDCANCEL)
    {
        // TODO: 在此放置处理何时用
        //  “取消”来关闭对话框的代码
    }
    else if (nResponse == -1)
    {
        TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
        TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
    }

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return TRUE;

#endif // _WINDLL
}


#ifdef _WINDLL
////////////////////////////////////////////////////////////////////////////////////////
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

#endif // _WINDLL

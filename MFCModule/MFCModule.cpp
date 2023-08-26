
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
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

    CMFCModuleDlg* pDlg = new CMFCModuleDlg();
	m_pMainWnd = pDlg;
    pDlg->Create(IDD_MFCMODULE_DIALOG);

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return TRUE;
}



int CMFCModuleApp::ExitInstance()
{
    // TODO: 在此添加专用代码和/或调用基类
    //if (pShellManager != nullptr)
    //{
    //    delete pShellManager;
    //}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
    ControlBarCleanUp();
#endif

    return CWinApp::ExitInstance();
}

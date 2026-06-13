#include "pch.h"
#include "MFCModuleService.h"
#include "MFCModule.h"
#include "MFCModuleDlg.h"

void MFCModuleService::SetEditText(const wchar_t* strText)
{
    CMFCModuleDlg* pDlg = dynamic_cast<CMFCModuleDlg*>(theApp.m_pMainWnd);
    if (pDlg != nullptr)
    {
        pDlg->SetEditText(strText);
    }
}

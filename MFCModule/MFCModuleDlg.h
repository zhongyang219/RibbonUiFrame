
// MFCModuleDlg.h: 头文件
//

#pragma once


// CMFCModuleDlg 对话框
class CMFCModuleDlg : public CDialog
{
// 构造
public:
	CMFCModuleDlg(CWnd* pParent = nullptr);	// 标准构造函数
    void SetWindowVisible(bool visible);

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCMODULE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
    bool m_windowVisible{ false };      //窗口是否显示（初始时不显示）

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
    afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
};

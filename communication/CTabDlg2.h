#pragma once
#include "afxdialogex.h"


// CTabDlg2 对话框

class CTabDlg2 : public CDialogEx
{
	DECLARE_DYNAMIC(CTabDlg2)

public:
	CTabDlg2(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTabDlg2();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()
public:
	CEdit TextRecord2;
	CEdit TextContainer2;
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	afx_msg void OnBnClickedRadio6();
};

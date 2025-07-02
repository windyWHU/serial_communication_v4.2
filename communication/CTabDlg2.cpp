// CTabDlg2.cpp: 实现文件
//

#include "pch.h"
#include "communication.h"
#include "afxdialogex.h"
#include "CTabDlg2.h"


// CTabDlg2 对话框

IMPLEMENT_DYNAMIC(CTabDlg2, CDialogEx)

CTabDlg2::CTabDlg2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CTabDlg2::~CTabDlg2()
{
}

void CTabDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, TextRecord2);
	DDX_Control(pDX, IDC_EDIT2, TextContainer2);
}


BEGIN_MESSAGE_MAP(CTabDlg2, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO2, &CTabDlg2::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO4, &CTabDlg2::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO7, &CTabDlg2::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, &CTabDlg2::OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO6, &CTabDlg2::OnBnClickedRadio6)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CTabDlg2 消息处理程序

void CTabDlg2::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CTabDlg2::OnBnClickedRadio4()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CTabDlg2::OnBnClickedRadio7()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CTabDlg2::OnBnClickedRadio8()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CTabDlg2::OnBnClickedRadio6()
{
	// TODO: 在此添加控件通知处理程序代码
}

BOOL CTabDlg2::OnEraseBkgnd(CDC* pDC)
{
    CBitmap bitmap;
    bitmap.LoadBitmap(IDB_BITMAP2);
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap* pOldBmp = memDC.SelectObject(&bitmap);

    BITMAP bit;
    bitmap.GetBitmap(&bit);

    CRect rect;
    GetClientRect(&rect);

    pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bit.bmWidth, bit.bmHeight, SRCCOPY);

    memDC.SelectObject(pOldBmp);
    return TRUE;
}


// communicationDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "communication.h"
#include "communicationDlg.h"
#include "afxdialogex.h"
#include"CTabDlg1.h"
#include"CTabDlg2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
CTabDlg1* ctabDlg1;
CTabDlg2* ctabDlg2;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CcommunicationDlg 对话框



CcommunicationDlg::CcommunicationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COMMUNICATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcommunicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, r_tab1);
}

BEGIN_MESSAGE_MAP(CcommunicationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CcommunicationDlg::OnTcnSelchangeTab1)
ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CcommunicationDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CcommunicationDlg 消息处理程序

BOOL CcommunicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	
	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	CRect tabrect;
	r_tab1.GetClientRect(&tabrect);
	tabrect.DeflateRect(2, 30, 5, 5);
	r_tab1.InsertItem(0, L"串口通信");
    r_tab1.InsertItem(1, L"网口通信");
	ctabDlg1 = new CTabDlg1();
    	ctabDlg2 = new CTabDlg2();
	ctabDlg1->Create(IDD_DIALOG1, &r_tab1);
    	ctabDlg2->Create(IDD_DIALOG2, &r_tab1);
	ctabDlg1->MoveWindow(tabrect);
    	ctabDlg2->MoveWindow(tabrect);
	ctabDlg1->ShowWindow(SW_SHOW);
    	ctabDlg2->ShowWindow(SW_HIDE);
	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CcommunicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CcommunicationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CcommunicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//void CcommunicationDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
//{
	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}

void CcommunicationDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	switch (r_tab1.GetCurSel())
	{
        	case 0:
                			ctabDlg1->ShowWindow(SW_SHOW);
                			ctabDlg2->ShowWindow(SW_HIDE);
                			break;
        	case 1:
                			ctabDlg1->ShowWindow(SW_HIDE);
                			ctabDlg2->ShowWindow(SW_SHOW);
                			break;
	}
	*pResult = 0;
}

#pragma once
#include "afxdialogex.h"
#include "CSerialPort/SerialPort.h"
#include "CSerialPort/SerialPortInfo.h"

// 消息定义和结构体必须放在类定义前
#define WM_DISPLAY_RAW_DATA (WM_USER + 200)
struct RawSerialData {
    std::vector<char> data;
    CString portName;
};
using namespace itas109;


class CTabDlg1 : public CDialogEx, public CSerialPortListener
{
    DECLARE_DYNAMIC(CTabDlg1)

public:
    CTabDlg1(CWnd* pParent = nullptr);
    virtual ~CTabDlg1();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG1 };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg LRESULT OnDisplayRawData(WPARAM wParam, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC); // 添加声明

    DECLARE_MESSAGE_MAP()

private:
    void onReadEvent(const char* portName, unsigned int readBufferLen);
    void SplitString(const CString& str, LPCTSTR delimiter, CStringArray& tokens);

    CSerialPort m_serialPort;
    HICON m_hIcon; // Add this declaration for the icon handle

public:
    // 控件变量
    CEdit TextRecord1;
    CEdit TextContainer1;
    CComboBox m_port1;
    CComboBox m_port2;
    CComboBox m_port3;
    CComboBox m_port4;
    CComboBox m_port5;
    CButton Container_default1;
    CButton button2;

    // 消息处理函数
    afx_msg void OnEnSetfocusEdit2();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedRadio3();
    afx_msg void OnBnClickedRadio2();
    afx_msg void OnBnClickedRadio4();
    afx_msg void OnBnClickedRadio5();
    afx_msg void OnBnClickedRadio8();
    afx_msg void OnEnChangeEdit1();
    afx_msg void OnBnClickedButton3();
};
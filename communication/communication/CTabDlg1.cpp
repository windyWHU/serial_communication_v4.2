// CTabDlg1.cpp: 实现文件
//

#include "pch.h"
#include "communication.h"
#include "afxdialogex.h"
#include "CTabDlg1.h"
#include <atlconv.h> 
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#define WM_DISPLAY_RAW_DATA (WM_USER + 200)
// 自定义消息，用于主线程安全显示串口接收到的数据

// CTabDlg1 对话框类实现
// 该类负责串口通信界面的所有逻辑，包括串口参数设置、打开关闭、数据收发、UI交互等

// MFC宏，声明CTabDlg1为动态创建类
IMPLEMENT_DYNAMIC(CTabDlg1, CDialogEx)

// 全局变量，记录单选框状态（发送/接收格式）
int m_nRadioGroup1;
int m_nRadioGroup2;

// 构造函数，初始化对话框和串口图标
CTabDlg1::CTabDlg1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent) // 调用父类构造，指定对话框资源ID
{
    // m_hIcon 是类的成员变量，保存窗口图标句柄
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME); // 加载主程序图标
}

// 析构函数
CTabDlg1::~CTabDlg1()
{
}

// 对话框初始化函数
BOOL CTabDlg1::OnInitDialog()
{
    // 调用父类初始化
    CDialogEx::OnInitDialog();

    // 以下为界面控件的初始化（设置下拉框默认选项等）
    CComboBox* pCombo1 = (CComboBox*)GetDlgItem(IDC_COMBO1); // 获取串口号下拉框
    if (pCombo1) pCombo1->SetCurSel(0); // 默认选第1项
    CComboBox* pCombo2 = (CComboBox*)GetDlgItem(IDC_COMBO2); // 波特率
    if (pCombo2) pCombo2->SetCurSel(6);
    CComboBox* pCombo3 = (CComboBox*)GetDlgItem(IDC_COMBO3); // 校验位
    if (pCombo3) pCombo3->SetCurSel(0);
    CComboBox* pCombo4 = (CComboBox*)GetDlgItem(IDC_COMBO4); // 数据位
    if (pCombo4) pCombo4->SetCurSel(3);
    CComboBox* pCombo5 = (CComboBox*)GetDlgItem(IDC_COMBO5); // 停止位
    if (pCombo5) pCombo5->SetCurSel(0);

    // 设置发送/接收格式单选框的默认值
    m_nRadioGroup1 = 3; // 发送格式：文本
    m_nRadioGroup2 = 3; // 接收格式：文本
    UpdateData(FALSE);  // 更新控件显示

    // 设置发送区初始文本
    TextContainer1.SetWindowTextW(L"请输入数据");
    return TRUE;  // 返回TRUE表示继续默认焦点处理
}

// MFC数据交换函数，将控件和类成员变量绑定
void CTabDlg1::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    // DDX_Control用于将控件和成员变量绑定
    DDX_Control(pDX, IDC_EDIT1, TextRecord1);      // 接收区
    DDX_Control(pDX, IDC_EDIT2, TextContainer1);   // 发送区
    DDX_Control(pDX, IDC_COMBO1, m_port1);         // 串口号
    DDX_Control(pDX, IDCANCEL, Container_default1);
    DDX_Radio(pDX, IDC_RADIO1, m_nRadioGroup1);    // 发送格式单选框
    DDX_Radio(pDX, IDC_RADIO5, m_nRadioGroup2);    // 接收格式单选框
    DDX_Control(pDX, IDC_COMBO2, m_port2);         // 波特率
    DDX_Control(pDX, IDC_COMBO3, m_port3);         // 校验位
    DDX_Control(pDX, IDC_COMBO4, m_port4);         // 数据位
    DDX_Control(pDX, IDC_COMBO5, m_port5);         // 停止位
    DDX_Control(pDX, IDC_BUTTON2, button2);        // 打开/关闭按钮
}

// 消息映射表，关联控件事件和成员函数
BEGIN_MESSAGE_MAP(CTabDlg1, CDialogEx)
ON_EN_SETFOCUS(IDC_EDIT2, &CTabDlg1::OnEnSetfocusEdit2) // 发送区获得焦点
ON_BN_CLICKED(IDC_BUTTON2, &CTabDlg1::OnBnClickedButton2) // 打开/关闭串口
ON_BN_CLICKED(IDC_BUTTON1, &CTabDlg1::OnBnClickedButton1) // 发送数据
ON_MESSAGE(WM_DISPLAY_RAW_DATA, &CTabDlg1::OnDisplayRawData) // 串口接收数据显示
ON_BN_CLICKED(IDCANCEL, &CTabDlg1::OnBnClickedCancel)
ON_BN_CLICKED(IDC_RADIO3, &CTabDlg1::OnBnClickedRadio3)
ON_BN_CLICKED(IDC_RADIO2, &CTabDlg1::OnBnClickedRadio2)
ON_BN_CLICKED(IDC_RADIO4, &CTabDlg1::OnBnClickedRadio4)
ON_BN_CLICKED(IDC_RADIO5, &CTabDlg1::OnBnClickedRadio5)
ON_BN_CLICKED(IDC_RADIO8, &CTabDlg1::OnBnClickedRadio8)
ON_EN_CHANGE(IDC_EDIT1, &CTabDlg1::OnEnChangeEdit1)
ON_WM_PAINT()
ON_WM_ERASEBKGND()
ON_BN_CLICKED(IDC_BUTTON3, &CTabDlg1::OnBnClickedButton3) // 清空接收区
END_MESSAGE_MAP()

// 打开/关闭串口按钮事件
void CTabDlg1::OnBnClickedButton2()
{
    // 判断串口是否已打开
    if (m_serialPort.isOpen() == false)
    {
        // 1. 绑定串口接收事件（回调到本类的onReadEvent）
        m_serialPort.connectReadEvent(this);

        // 2. 获取串口参数（端口号、波特率、校验位、数据位、停止位）
        CString strPort, strBaudRate, strParity, strDataBits, strStopBits;
        m_port1.GetWindowText(strPort);       // 端口号（如"COM1"）
        m_port2.GetWindowText(strBaudRate);   // 波特率（如"9600"）
        m_port3.GetWindowText(strParity);     // 校验位（如"NONE"）
        m_port4.GetWindowText(strDataBits);   // 数据位（如"8 bit"）
        m_port5.GetWindowText(strStopBits);   // 停止位（如"1 bit"）

        // 3. 参数转换为CSerialPort库需要的类型
        int baudRate = _ttoi(strBaudRate); // 字符串转整数
        itas109::Parity parity;
        if (strParity == "NONE")       parity = itas109::Parity::ParityNone;
        else if (strParity == "ODD")   parity = itas109::Parity::ParityOdd;
        else if (strParity == "EVEN")  parity = itas109::Parity::ParityEven;
        else                           parity = itas109::Parity::ParityNone; // 默认

        itas109::DataBits dataBits;
        if (strDataBits == "5 bit")    dataBits = itas109::DataBits::DataBits5;
        else if (strDataBits == "6 bit") dataBits = itas109::DataBits::DataBits6;
        else if (strDataBits == "7 bit") dataBits = itas109::DataBits::DataBits7;
        else if (strDataBits == "8 bit") dataBits = itas109::DataBits::DataBits8;
        else                           dataBits = itas109::DataBits::DataBits8; // 默认

        itas109::StopBits stopBits;
        if (strStopBits == "1 bit")    stopBits = itas109::StopBits::StopOne;
        else if (strStopBits == "1.5 bit") stopBits = itas109::StopBits::StopOneAndHalf;
        else if (strStopBits == "2 bit") stopBits = itas109::StopBits::StopTwo;
        else                           stopBits = itas109::StopBits::StopOne; // 默认

        // 4. 初始化串口对象（设置参数）
        // CT2A(strPort) 将CString转为const char*，适配CSerialPort库
        m_serialPort.init(
            CT2A(strPort),      // 端口名
            baudRate,           // 波特率
            parity,             // 校验位
            dataBits,           // 数据位
            stopBits,           // 停止位
            itas109::FlowControl::FlowNone, // 流控（默认无）
            4096                // 读缓冲区大小
        );
        // 5. 打开串口
        m_serialPort.open();

        // 6. 根据打开结果更新UI
        if (m_serialPort.isOpen())
        {
            button2.SetWindowTextW(L"关闭"); // 按钮变为关闭
            MessageBox(_T("打开成功"));
        }
        else
        {
            MessageBox(_T("打开失败"));
        }
    }
    else
    {
        // 已打开则关闭串口
        m_serialPort.close();
        button2.SetWindowTextW(L"打开");
        MessageBox(_T("关闭成功"));
    }
}

// 发送区获得焦点时，清空默认提示
void CTabDlg1::OnEnSetfocusEdit2()
{
    CString text;
    TextContainer1.GetWindowText(text);
    if( text == L"请输入数据" )
        TextContainer1.SetWindowTextW(L"");
}

// 发送按钮事件，处理数据并通过串口发送
void CTabDlg1::OnBnClickedButton1()
{
    // 1. 检查串口是否打开且发送区有内容
    if (m_serialPort.isOpen() && TextContainer1.GetWindowTextLength() != 0)
    {
        UpdateData(TRUE);
        CString strText;
        CString mode;
        TextContainer1.GetWindowText(strText);
        strText.Trim(); // 去除首尾空格
        std::vector<BYTE> senddata; // 用于存放要发送的字节数据

        // 2. 根据发送格式（单选框）处理数据
        switch (m_nRadioGroup1) {
        case 0: // 二进制发送
        {
            CString strBinary = strText;
            strBinary.Remove(' '); // 移除空格
            int len = strBinary.GetLength();
            if (len == 0) {
                MessageBox(_T("请输入二进制数据！"), _T("提示"), MB_ICONWARNING);
                return;
            }
            if (len % 8 != 0) {
                MessageBox(_T("二进制输入长度必须为8的倍数！"), _T("提示"), MB_ICONWARNING);
                return;
            }
            // 每8位转为1字节
            for (int i = 0; i < len; i += 8) {
                CString byteStr = strBinary.Mid(i, 8);
                for (int j = 0; j < 8; ++j) {
                    if (byteStr[j] != '0' && byteStr[j] != '1') {
                        MessageBox(_T("二进制输入只能包含0和1！"), _T("提示"), MB_ICONWARNING);
                        return;
                    }
                }
                BYTE b = (BYTE)_tcstoul(byteStr, NULL, 2); // 字符串转二进制
                senddata.push_back(b);
            }
            mode = _T("Binary");
        }
        break;
        case 1: // 十六进制发送
        {
            CStringArray hexTokens;
            SplitString(strText, _T(" "), hexTokens); // 按空格分割
            if (hexTokens.GetCount() == 0) {
                MessageBox(_T("请输入十六进制数据！"), _T("提示"), MB_ICONWARNING);
                return;
            }
            for (int i = 0; i < hexTokens.GetCount(); i++) {
                CString token = hexTokens[i];
                token.Trim();
                if (token.IsEmpty()) continue;
                if (token.GetLength() > 2) {
                    MessageBox(_T("每个十六进制字节不能超过2位！"), _T("提示"), MB_ICONWARNING);
                    return;
                }
                if (token.GetLength() == 1) token = _T("0") + token;
                for (int j = 0; j < token.GetLength(); ++j) {
                    if (!((token[j] >= '0' && token[j] <= '9') ||
                        (token[j] >= 'A' && token[j] <= 'F') ||
                        (token[j] >= 'a' && token[j] <= 'f'))) {
                        MessageBox(_T("输入包含非法十六进制字符！"), _T("提示"), MB_ICONWARNING);
                        return;
                    }
                }
                BYTE b = (BYTE)_tcstoul(token, NULL, 16); // 字符串转16进制
                senddata.push_back(b);
            }
            if (senddata.empty()) {
                MessageBox(_T("没有可发送的十六进制数据！"), _T("提示"), MB_ICONWARNING);
                return;
            }
            mode = _T("Hex");
        }
        break;
        case 2: // UTF-8发送
        case 3: // 文本发送（UTF-8编码，保证中文不乱码）
        {
            // strText 是 CString，MFC Unicode 工程下就是宽字符
            int wlen = strText.GetLength();
            if (wlen == 0) break;

            // 计算需要多少字节（不包含结尾\0）
            int utf8Len = WideCharToMultiByte(CP_UTF8, 0, strText, wlen, NULL, 0, NULL, NULL);
            if (utf8Len > 0) {
                std::vector<char> utf8Buf(utf8Len);
                WideCharToMultiByte(CP_UTF8, 0, strText, wlen, utf8Buf.data(), utf8Len, NULL, NULL);
                senddata.assign(utf8Buf.begin(), utf8Buf.end());
            }
            mode = (m_nRadioGroup1 == 2) ? _T("UTF-8") : _T("Text");
        }
        break;
        }

        // 3. 发送数据
        if (!senddata.empty()) {
            m_serialPort.writeData(senddata.data(), senddata.size()); // 发送数据到串口
            // 4. 记录发送日志到接收区
            CString strText_before;
            CTime currentTime = CTime::GetCurrentTime();
            CString strTimestamp = currentTime.Format(_T("%Y-%m-%d %H:%M:%S"));
            TextRecord1.GetWindowText(strText_before);
            TextRecord1.SetWindowText(strText_before + strTimestamp + "(send)" + "(" + mode + ")" + ": " + strText + "\r\n");
        }
    }
    else
    {
        if (!m_serialPort.isOpen())
            MessageBox(_T("需要先打开端口"));
    }
}

// 字符串分割函数，按指定分隔符分割字符串到tokens数组
void CTabDlg1::SplitString(const CString& str, LPCTSTR delimiter, CStringArray& tokens)
{
    tokens.RemoveAll();
    int pos = 0;
    CString token;
    while (AfxExtractSubString(token, str, pos, delimiter[0])) {
        tokens.Add(token);
        pos++;
    }
}

// 串口接收事件回调（由CSerialPort库在子线程调用）
// 读取数据后，投递到主线程处理
void CTabDlg1::onReadEvent(const char* portName, unsigned int readBufferLen) {
    if (readBufferLen > 0) {
        std::vector<char> buffer(readBufferLen);
        int recLen = m_serialPort.readData(buffer.data(), readBufferLen);

        if (recLen > 0) {
            RawSerialData* pRawData = new RawSerialData;
            pRawData->data.assign(buffer.begin(), buffer.begin() + recLen);
            pRawData->portName = portName;

            // PostMessage将数据投递到主线程，避免跨线程操作UI
            PostMessage(WM_DISPLAY_RAW_DATA, (WPARAM)pRawData, 0);
        }
    }
}

// 主线程处理串口接收数据，安全更新界面
LRESULT CTabDlg1::OnDisplayRawData(WPARAM wParam, LPARAM lParam) {
    RawSerialData* pRawData = reinterpret_cast<RawSerialData*>(wParam);
    if (!pRawData || !::IsWindow(TextRecord1.m_hWnd)) {
        delete pRawData;
        return 0;
    }

    UpdateData(TRUE);
    int displayMode = m_nRadioGroup2;  // 接收格式单选框状态

    CString strDisplay, strMode;
    const auto& data = pRawData->data;

    // 按接收格式显示数据
    switch (displayMode) {
    case 3: // 文本
    case 2: // UTF-8
    {
        CA2T utf16Text(data.data(), CP_UTF8);
        strDisplay = utf16Text.m_psz;
        strMode = (displayMode == 2) ? "UTF-8" : "Text";
        break;
    }
    case 1: // 十六进制
    {
        CString tmp;
        for (BYTE b : data) {
            tmp.AppendFormat(_T("%02X "), b);
        }
        strDisplay = tmp.TrimRight();
        strMode = "Hex";
        break;
    }
    case 0: // 二进制
    {
        CString tmp;
        for (BYTE b : data) {
            CString byteStr;
            for (int j = 7; j >= 0; j--) {
                byteStr += ((b >> j) & 1) ? _T("1") : _T("0");
            }
            tmp += byteStr + _T(" ");
        }
        strDisplay = tmp.TrimRight();
        strMode = "Binary";
        break;
    }
    }

    // 显示到接收区
    CString strTimestamp = CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"));
    CString strTextBefore;
    TextRecord1.GetWindowText(strTextBefore);

    TextRecord1.SetWindowText(strTextBefore
        + strTimestamp + "(receive)(" + strMode + "): "
        + strDisplay + "\r\n");

    delete pRawData;
    return 0;
}

// 清空接收区按钮
void CTabDlg1::OnBnClickedButton3()
{
    TextRecord1.SetWindowTextW(L"");
    MessageBox(_T("清空成功"));
}

// 其它控件事件（可根据需要补充实现）
void CTabDlg1::OnBnClickedCancel() { }
void CTabDlg1::OnBnClickedRadio3() { }
void CTabDlg1::OnBnClickedRadio2() { }
void CTabDlg1::OnBnClickedRadio4() { }
void CTabDlg1::OnBnClickedRadio5() { }
void CTabDlg1::OnBnClickedRadio8() { }
void CTabDlg1::OnEnChangeEdit1() { }

// 绘制窗口图标（窗口最小化时）
void CTabDlg1::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint(); // 只调用基类
    }
}

BOOL CTabDlg1::OnEraseBkgnd(CDC* pDC)
{
    CBitmap bitmap;
    bitmap.LoadBitmap(IDB_BITMAP1);
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap* pOldBmp = memDC.SelectObject(&bitmap);

    BITMAP bit;
    bitmap.GetBitmap(&bit);

    CRect rect;
    GetClientRect(&rect);

    // 只画背景，不影响控件
    pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, bit.bmWidth, bit.bmHeight, SRCCOPY);

    memDC.SelectObject(pOldBmp);
    return TRUE; // 表示已处理背景擦除
}



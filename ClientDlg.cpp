
// 客户端Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClientDlg 对话框




CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	theApp.active = this;
	theApp.m_udp->Close();//关闭UDP套接字
	theApp.m_udp = NULL;
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listcontrol);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CClientDlg::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_QUIT, &CClientDlg::OnBnClickedQuit)
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CRect rect;
	// 获取编程语言列表视图控件的位置和大小
	m_listcontrol.GetClientRect(&rect);
	// 为列表视图控件添加全行选中和栅格风格
	m_listcontrol.SetExtendedStyle(m_listcontrol.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 为列表视图控件添加两列
	m_listcontrol.InsertColumn(0, L"在线用户", LVCFMT_CENTER, rect.Width()*3/10, 1);
	m_listcontrol.InsertColumn(1, L"在线用户", LVCFMT_CENTER, rect.Width()*3/10, 1);
	m_listcontrol.InsertColumn(2, L"昵称", LVCFMT_CENTER, rect.Width()*3/10, 1);
	m_listcontrol.InsertColumn(3, L"用户IP", LVCFMT_CENTER, rect.Width()*4/10, 1);
	m_listcontrol.DeleteColumn(0);
	m_caption = (CString)"(" + theApp.m_userID + (CString)") " + theApp.m_userNAME + (CString)"的Client";
	SetWindowTextW(m_caption);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClientDlg::OnPaint()
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CClientDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return;
	CDialogEx::OnClose();
}


void CClientDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)//双击列表视图控件事件响应程序
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CString strLangID;    
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;
	if (-1 != pNMListView->iItem)        // 如果iItem不是-1，就说明有列表项被选择
	{
		strLangID = m_listcontrol.GetItemText(pNMListView->iItem, 0);
	}
	else
		return;

	if (strLangID == theApp.m_userID)
	{
		MessageBox(L"不能给自己发信息！",L"提示",MB_OK);
		return;
	}

	map<CString,CChat *>::iterator it = theApp.m_userMsg.find(strLangID);

	if(it == theApp.m_userMsg.end())
	{
		CChat* c;
		c = new CChat;
		c->m_toID = strLangID;
		c->m_name = theApp.m_client.find(strLangID)->second;
		c->Create(IDD_CHAT,this);
		c->ShowWindow(SW_SHOW);
		theApp.m_userMsg.insert(make_pair(strLangID,c));




		/*消息记录*/

		map<CString, CString>::iterator map_it = theApp.m_MsgRecode.find(strLangID);

		if (map_it == theApp.m_MsgRecode.end())
		{
			theApp.m_MsgRecode.insert(make_pair(strLangID, L""));
			return;
		}

		c->UpdateData(true);

		c->m_receiveMsg += theApp.m_MsgRecode[strLangID];

		
		c->m_receiveCtrl.LineScroll(c->m_receiveCtrl.GetLineCount() - 1);

		c->UpdateData(false);

	}
	else
		return;
}


void CClientDlg::OnBnClickedQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(L"退出后消息记录清空！！",L"提示",MB_OK);

	exit(0);
}

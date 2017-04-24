
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
	, m_StateStr(_T(""))
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
	DDX_Text(pDX, IDC_EDIT1, m_StateStr);
	DDX_Control(pDX, IDC_COMBO1, m_comboxBox);
	DDX_Control(pDX, IDC_Picture, m_Picture);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CClientDlg::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_QUIT, &CClientDlg::OnBnClickedQuit)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CClientDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON1, &CClientDlg::OnBnClickedButton1)
	ON_MESSAGE(WM_SHOWTASK, OnShowTask)
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
	m_caption = (CString)"(" + theApp.m_userID + (CString)") " + theApp.m_userNAME;
	UpdateData(true);
	m_StateStr = L"在线";


	for (int i = 1; i < 84; i++)
	{
		CString tmp;
		tmp.Format(_T("%d"), i);
		m_comboxBox.AddString(tmp);
	}
	UpdateData(false);
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
	UINT a = MessageBox(L"退出??", L"提示", MB_YESNO);
	
	if (a == 6)
	{
		exit(0);
	}
}

void CClientDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UINT a = MessageBox(L"退出??", L"提示", MB_YESNO);

	if (a == 6)
	{
		CDialogEx::OnClose();
	}
}


void CClientDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	int nIndex = m_comboxBox.GetCurSel();

	CString strCBText;

	m_comboxBox.GetLBText(nIndex, strCBText);

	int a = _ttoi(strCBText);

	CBitmap bitmap;  // CBitmap对象，用于加载位图   
	HBITMAP hBmp;    // 保存CBitmap加载的位图的句柄   

	bitmap.LoadBitmap(134+a);  // 将位图IDB_BITMAP1加载到bitmap   
	hBmp = (HBITMAP)bitmap.GetSafeHandle();  // 获取bitmap加载位图的句柄   
	m_Picture.SetBitmap(hBmp);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1   

	UpdateData(false);
}


void CClientDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd; nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_SHOWTASK;//自定义的消息名称
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	memcpy(nid.szTip, L"程序名称", 10); //信息提示条
	Shell_NotifyIcon(NIM_ADD, &nid); //在托盘区添加图标
	ShowWindow(SW_HIDE); //隐藏主窗口
}

LRESULT CClientDlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	if (wParam != IDR_MAINFRAME)
		return 1;
	switch (lParam)
	{
	case WM_RBUTTONUP://右键起来时弹出快捷菜单，这里只有一个“关闭”
	{ 
		LPPOINT lpoint = new tagPOINT;
	    ::GetCursorPos(lpoint);//得到鼠标位置
	     CMenu menu;
	    menu.CreatePopupMenu();//声明一个弹出式菜单
						   //增加菜单项“关闭”，点击则发送消息WM_DESTROY给主窗口将程序结束。
	    menu.AppendMenu(MF_STRING, WM_DESTROY, L"关闭"); //确定弹出式菜单的位置
	    menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this); //资源回收
	    HMENU hmenu = menu.Detach();
	    menu.DestroyMenu();
	    delete lpoint;
	} 
	break;
	case WM_LBUTTONDBLCLK://双击左键的处理
	{ 
		this->ShowWindow(SW_SHOW);//显示主窗口
	    //DeleteTray();
	} 
	break;
	default:
		break;
	}
	return 0;
}

//void CClientDlg::DeleteTray()
//{
//	NOTIFYICONDATA nid;
//	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
//	nid.hWnd = this->m_hWnd;
//	nid.uID = IDR_MAINFRAME;
//	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
//	nid.uCallbackMessage = WM_SHOWTASK;//自定义的消息名称
//	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
//	strcpy(nid.szTip, "程序名称"); //信息提示条为“计划任务提醒”
//	Shell_NotifyIcon(NIM_DELETE, &nid); //在托盘区删除图标
//}
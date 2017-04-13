
// �ͻ���Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CClientDlg �Ի���




CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	theApp.active = this;
	theApp.m_udp->Close();//�ر�UDP�׽���
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


// CClientDlg ��Ϣ��������

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵������ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ����Ӷ���ĳ�ʼ������
	CRect rect;
	// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С
	m_listcontrol.GetClientRect(&rect);
	// Ϊ�б���ͼ�ؼ�����ȫ��ѡ�к�դ����
	m_listcontrol.SetExtendedStyle(m_listcontrol.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// Ϊ�б���ͼ�ؼ���������
	m_listcontrol.InsertColumn(0, L"�����û�", LVCFMT_CENTER, rect.Width()*3/10, 1);
	m_listcontrol.InsertColumn(1, L"�����û�", LVCFMT_CENTER, rect.Width()*3/10, 1);
	m_listcontrol.InsertColumn(2, L"�ǳ�", LVCFMT_CENTER, rect.Width()*3/10, 1);
	m_listcontrol.InsertColumn(3, L"�û�IP", LVCFMT_CENTER, rect.Width()*4/10, 1);
	m_listcontrol.DeleteColumn(0);
	m_caption = (CString)"(" + theApp.m_userID + (CString)") " + theApp.m_userNAME + (CString)"��Client";
	SetWindowTextW(m_caption);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CClientDlg::OnClose()
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	return;
	CDialogEx::OnClose();
}


void CClientDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)//˫���б���ͼ�ؼ��¼���Ӧ����
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	*pResult = 0;
	CString strLangID;    
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;
	if (-1 != pNMListView->iItem)        // ���iItem����-1����˵�����б��ѡ��
	{
		strLangID = m_listcontrol.GetItemText(pNMListView->iItem, 0);
	}
	else
		return;

	if (strLangID == theApp.m_userID)
	{
		MessageBox(L"���ܸ��Լ�����Ϣ��",L"��ʾ",MB_OK);
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




		/*��Ϣ��¼*/

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
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	MessageBox(L"�˳�����Ϣ��¼��գ���",L"��ʾ",MB_OK);

	exit(0);
}
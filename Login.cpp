// Login.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "Login.h"
#include "afxdialogex.h"


// CLogin 对话框

IMPLEMENT_DYNAMIC(CLogin, CDialogEx)

CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogin::IDD, pParent)
	, m_userID(L"")
	, m_password(L"")
{
	theApp.log = this;
	tcp_tag = 0;
	GetPath(sPath, L"\\res\\remember.txt");   //记录之前打开过的账号
	Read();
}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PASSWORD, m_password);
	DDX_Control(pDX, IDC_USERNAME, m_remember);
	DDX_CBString(pDX, IDC_USERNAME, m_userID);
}


BEGIN_MESSAGE_MAP(CLogin, CDialogEx)
	ON_MESSAGE(WM_MSG,OnMsg)
	ON_BN_CLICKED(IDC_BUTTON1, &CLogin::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLogin::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CLogin::OnBnClickedButton3)
END_MESSAGE_MAP()



// CLogin 消息处理程序
void CLogin::GetPath(CString &sPath, CString filename)
{
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);//返回该当前应用程序*.exe的全路径
	sPath.ReleaseBuffer();
	int nPos;
	nPos = sPath.ReverseFind('\\');//ReverseFind()在一个较大的字符串中从末端开始查找某个字符
	sPath = sPath.Left(nPos);
	nPos = sPath.ReverseFind('\\');
	sPath = sPath.Left(nPos);
	sPath += filename;
}


void CLogin::Read()
{
	fstream infile(sPath,fstream::in);
	if(infile)
	{
		string temp;
		while(!infile.eof())
		{
			infile>>temp;
			vec.push_back(temp);
		}
	}
	infile.close();
}

void CLogin::Write()
{
	fstream out(sPath,fstream::out);
	if(out)
	{
		out<<vec[0];
		for(int i = 1;i != vec.size();i++)
			out<<endl<<vec[i];
	}
	out.close();
}

void CLogin::OnBnClickedButton1()   //登陆按钮
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(m_userID == ""||m_password == "")
		return;
	else if(m_userID.GetLength()>=6)
	{
		MessageBox(L"账号超出5位数字！");
		return;
	}
	if(tcp_tag == 0)
	{
		if(theApp.search != 1)
		{
			CString st(L"客户端查找服务器");

			theApp.m_udp->SendTo(st,2*st.GetLength(),1001,L"255.255.255.255");//发送广播

			MessageBox(L"未搜寻到服务器！", L"客户端");
			return;
		}

		theApp.serverip = theApp.m_udp->s;//服务器IP
		
		theApp.m_tcp = new CTcpSocket();

		if(!theApp.m_tcp)
		{
			MessageBox(L"内存不足！");
			return;
		}
		else if(!theApp.m_tcp->Create())
		{
			MessageBox(L"创建套接字失败！");
			return;
		}
		else if(!theApp.m_tcp->Connect(theApp.serverip,1003))  //服务器中对应的端口是1003
		{
			MessageBox(L"连接服务器失败");
			return;
		}
		tcp_tag = 1;
	}
	CHead temp;
	temp.type = 4;		//把用户信息发送给服务器
	strcpy_s(temp.ip,(CStringA)theApp.m_localip);
	strcpy_s(temp.usermsg.ID,(CStringA)m_userID);
	strcpy_s(temp.usermsg.PASSWORD,(CStringA)m_password);
	strcpy_s(temp.fromID,(CStringA)m_userID);
	theApp.m_tcp->Send(&temp,sizeof(temp));
}


void CLogin::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	exit(0);
}

LRESULT CLogin::OnMsg(WPARAM wParam,LPARAM lParam)
{
	if(theApp.tag == 1)
	{
		string t((CStringA)m_userID);
		int k = 0;
		for(int i =0;i!=vec.size();i++)
		{
			if(t == vec[i])
			{
				k = 1;
				break;
			}
		}
		if( k == 0)
			vec.push_back((string)(CStringA)m_userID);

		Write();

		CHead temp;

		strcpy_s(temp.ip,(CStringA)theApp.m_localip);

		strcpy_s(temp.usermsg.ID,(CStringA)m_userID);

		strcpy_s(temp.usermsg.PASSWORD,(CStringA)m_password);

		strcpy_s(temp.fromID,(CStringA)m_userID);

		temp.type = 1;

		strcpy_s(temp.usermsg.NAME,(CStringA)theApp.m_userNAME);

		theApp.m_tcp->Send(&temp,sizeof(temp));

		CDialogEx::OnOK();
	}
	else
		MessageBox(L"账号或密码错误！",L"客户端");
	return TRUE;
}

void CLogin::OnBnClickedButton3()  //注册按钮
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_register.DoModal() != IDOK)  //注册账户窗口
		return;
	if(tcp_tag == 0)    //未创建时需新建socket，初始化等
	{
		if(theApp.search != 1)
		{
			CString st(L"客户端查找服务器");
			theApp.m_udp->SendTo(st,2*st.GetLength(),1001,L"255.255.255.255");//发送广播
			MessageBox(L"未搜寻到服务器！",L"客户端");
			return;
		}
		theApp.serverip = theApp.m_udp->s;//服务器IP
		theApp.m_tcp = new CTcpSocket();
		if(!theApp.m_tcp)
		{
			MessageBox(L"内存不足！");
			return;
		}
		else if(!theApp.m_tcp->Create())
		{
			MessageBox(L"创建套接字失败！");
			return;
		}
		else if(!theApp.m_tcp->Connect(theApp.serverip,1003)) 
		{
			MessageBox(L"连接服务器失败");
			return;
		}
		tcp_tag = 1;
	}
	CHead temp;
	CString only(theApp.m_localip);
	only += "00000";
	temp.type = 5;		//把注册信息发送给服务器
	strcpy_s(temp.msg,(CStringA)only);
	strcpy_s(temp.ip,(CStringA)theApp.m_localip);
	strcpy_s(temp.usermsg.NAME,(CStringA)m_register.name);
	strcpy_s(temp.usermsg.PASSWORD,(CStringA)m_register.mima);
	theApp.m_tcp->Send(&temp,sizeof(temp));
}



BOOL CLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	for(int i=0;i!=vec.size();i++)
		m_remember.AddString((CString)vec[i].c_str());

	HICON m_hIcon;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);//IDR_ICON为图标资源名
	SetIcon(m_hIcon, TRUE); // Set big icon
	SetIcon(m_hIcon, FALSE); // Set small icon

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
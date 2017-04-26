// Login.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "Login.h"
#include "afxdialogex.h"


// CLogin �Ի���

IMPLEMENT_DYNAMIC(CLogin, CDialogEx)

CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogin::IDD, pParent)
	, m_userID(L"")
	, m_password(L"")
{
	theApp.log = this;
	tcp_tag = 0;
	GetPath(sPath, L"\\res\\remember.txt");   //��¼֮ǰ�򿪹����˺�
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



// CLogin ��Ϣ�������
void CLogin::GetPath(CString &sPath, CString filename)
{
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);//���ظõ�ǰӦ�ó���*.exe��ȫ·��
	sPath.ReleaseBuffer();
	int nPos;
	nPos = sPath.ReverseFind('\\');//ReverseFind()��һ���ϴ���ַ����д�ĩ�˿�ʼ����ĳ���ַ�
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

void CLogin::OnBnClickedButton1()   //��½��ť
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(m_userID == ""||m_password == "")
		return;
	else if(m_userID.GetLength()>=6)
	{
		MessageBox(L"�˺ų���5λ���֣�");
		return;
	}
	if(tcp_tag == 0)
	{
		if(theApp.search != 1)
		{
			CString st(L"�ͻ��˲��ҷ�����");

			theApp.m_udp->SendTo(st,2*st.GetLength(),1001,L"255.255.255.255");//���͹㲥

			MessageBox(L"δ��Ѱ����������", L"�ͻ���");
			return;
		}

		theApp.serverip = theApp.m_udp->s;//������IP
		
		theApp.m_tcp = new CTcpSocket();

		if(!theApp.m_tcp)
		{
			MessageBox(L"�ڴ治�㣡");
			return;
		}
		else if(!theApp.m_tcp->Create())
		{
			MessageBox(L"�����׽���ʧ�ܣ�");
			return;
		}
		else if(!theApp.m_tcp->Connect(theApp.serverip,1003))  //�������ж�Ӧ�Ķ˿���1003
		{
			MessageBox(L"���ӷ�����ʧ��");
			return;
		}
		tcp_tag = 1;
	}
	CHead temp;
	temp.type = 4;		//���û���Ϣ���͸�������
	strcpy_s(temp.ip,(CStringA)theApp.m_localip);
	strcpy_s(temp.usermsg.ID,(CStringA)m_userID);
	strcpy_s(temp.usermsg.PASSWORD,(CStringA)m_password);
	strcpy_s(temp.fromID,(CStringA)m_userID);
	theApp.m_tcp->Send(&temp,sizeof(temp));
}


void CLogin::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox(L"�˺Ż��������",L"�ͻ���");
	return TRUE;
}

void CLogin::OnBnClickedButton3()  //ע�ᰴť
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_register.DoModal() != IDOK)  //ע���˻�����
		return;
	if(tcp_tag == 0)    //δ����ʱ���½�socket����ʼ����
	{
		if(theApp.search != 1)
		{
			CString st(L"�ͻ��˲��ҷ�����");
			theApp.m_udp->SendTo(st,2*st.GetLength(),1001,L"255.255.255.255");//���͹㲥
			MessageBox(L"δ��Ѱ����������",L"�ͻ���");
			return;
		}
		theApp.serverip = theApp.m_udp->s;//������IP
		theApp.m_tcp = new CTcpSocket();
		if(!theApp.m_tcp)
		{
			MessageBox(L"�ڴ治�㣡");
			return;
		}
		else if(!theApp.m_tcp->Create())
		{
			MessageBox(L"�����׽���ʧ�ܣ�");
			return;
		}
		else if(!theApp.m_tcp->Connect(theApp.serverip,1003)) 
		{
			MessageBox(L"���ӷ�����ʧ��");
			return;
		}
		tcp_tag = 1;
	}
	CHead temp;
	CString only(theApp.m_localip);
	only += "00000";
	temp.type = 5;		//��ע����Ϣ���͸�������
	strcpy_s(temp.msg,(CStringA)only);
	strcpy_s(temp.ip,(CStringA)theApp.m_localip);
	strcpy_s(temp.usermsg.NAME,(CStringA)m_register.name);
	strcpy_s(temp.usermsg.PASSWORD,(CStringA)m_register.mima);
	theApp.m_tcp->Send(&temp,sizeof(temp));
}



BOOL CLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	for(int i=0;i!=vec.size();i++)
		m_remember.AddString((CString)vec[i].c_str());

	HICON m_hIcon;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);//IDR_ICONΪͼ����Դ��
	SetIcon(m_hIcon, TRUE); // Set big icon
	SetIcon(m_hIcon, FALSE); // Set small icon

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

// �ͻ���.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientApp

BEGIN_MESSAGE_MAP(CClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CClientApp ����

CClientApp::CClientApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CClientApp ����

CClientApp theApp;


// CClientApp ��ʼ��

BOOL CClientApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(L"��ҵ���");

	srand((unsigned)time(0));
	m_tcp = NULL;
	m_udp = NULL;
	search = 0;//��������Ѱ��ʶ
	tag = 0;
	GetLocalIp(m_localip);//��ȡ����IP
	int a=1;
	m_udp = new CUdpSocket();//����UDP�׽���
	if(!m_udp)
	{
		AfxMessageBox(L"�ڴ治�㣡");
		exit(0);
	}
	if(!m_udp->Create(1000,SOCK_DGRAM))
	{
		AfxMessageBox(L"�����׽���ʧ�ܣ�");
		exit(0);
	}

	m_udp->SetSockOpt(SO_BROADCAST,&a,sizeof(BOOL),SOL_SOCKET);//�����׽���ѡ������͹㲥��Ϣ
	CString st(L"�ͻ��˲��ҷ�����");//fisrt

	m_udp->SendTo(st,2*st.GetLength(),1001,L"255.255.255.255");//���͹㲥

	if(login.DoModal()!=IDOK)
		exit(0);

	m_userID = login.m_userID;

	CClientDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
		
	}
	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	
	return FALSE;
}

void CClientApp::GetLocalIp(CString &ip)
{
	char name[21] = {0}, **pptr;
    struct hostent *hptr;
	char str[32] = {0};
	gethostname(name,sizeof(name));
	hptr = gethostbyname(name);
    switch(hptr->h_addrtype)
    {
        case AF_INET:
			pptr=hptr->h_addr_list;

            inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str));

			break;
        default:
			break;
    }
	ip = str;


	//ip = L"192.168.1.150";
//	char buf[256] = "";
//
//	struct hostent *ph = 0;
//
//	WSADATA w;
//
//	WSAStartup(0x0101, &w);//��һ�б�����ʹ���κ�SOCKET����ǰд��
//
//	gethostname(buf, 256);
//
//	string hostNmae = buf;//�˴���ñ�������
//
//	ph = gethostbyname(buf);
//
//	ip = inet_ntoa(*((struct in_addr *)ph->h_addr_list[0]));//�˴���ñ���IP
//	int a = 0;
}
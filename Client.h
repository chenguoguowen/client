
// Client.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "Login.h"
#include "ClientDlg.h"
#include "Head.h"
#include "UdpSocket.h"
#include "TcpSocket.h"
#include "Chat.h"
// CClientApp:
// �йش����ʵ�֣������ Client.cpp
//

typedef struct _SOCKET_STREAM_FILE_INFO {

	TCHAR       szFileTitle[128];                   //�ļ��ı�����
	DWORD       dwFileAttributes;                   //�ļ�������
	FILETIME    ftCreationTime;                     //�ļ��Ĵ���ʱ��
	FILETIME    ftLastAccessTime;                   //�ļ���������ʱ��
	FILETIME    ftLastWriteTime;                    //�ļ�������޸�ʱ��
	DWORD       nFileSizeHigh;                      //�ļ���С�ĸ�λ˫��
	DWORD       nFileSizeLow;                       //�ļ���С�ĵ�λ˫��
	DWORD       dwReserved0;                        //������Ϊ0
	DWORD       dwReserved1;                        //������Ϊ0

} SOCKET_STREAM_FILE_INFO, *PSOCKET_STREAM_FILE_INFO;


class CClientApp : public CWinApp
{
public:
	CFont m_font;  //����
	CClientApp();
	CHead head;
	CClientDlg *active;
	CLogin *log;
	CUdpSocket *m_udp;
	CTcpSocket *m_tcp;
	int search;//��������Ѱ��ʶ
	int tag;//�˺�����ƥ���ʶ
	CLogin login;//��¼�˵�
	CString serverip;//��¼������IP
	CString m_localip;//����IP
	CString m_userID;//�˺�
	CString m_userNAME;//�û���
	map<CString,CChat *> m_userMsg;//��¼����Ի���
	map<CString,CString> m_MsgRecode;//��¼������Ϣ
	map<CString,CString> m_clientMsg;//��¼���߿ͻ���ID == ip
	map<CString,CString> m_client;//ID == NAME
	
private:
	void GetLocalIp(CString &ip);
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CClientApp theApp;    //ȫ�ֱ��������Ϣ
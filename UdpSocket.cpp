// UdpSocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "UdpSocket.h"


// CUdpSocket

CUdpSocket::CUdpSocket()
{
	s = "";
}

CUdpSocket::~CUdpSocket()
{
}


// CUdpSocket ��Ա����


void CUdpSocket::OnReceive(int nErrorCode)//�ͻ��˼����˿�1000�����Ͷ˿�1001
{
	// TODO: �ڴ����ר�ô����/����û���
	TCHAR recBuf[1024] = {0};
	int len = sizeof(SOCKADDR_IN);
	int recBytes = ReceiveFrom(recBuf, 1023, (SOCKADDR*)&m_serverAddr, &len, 0);//���ܷ�������ip��Ϣ
	if(wcscmp(recBuf,L"������Ӧ��") == 0)
	{
		s = inet_ntoa(m_serverAddr.sin_addr);  //��¼��������IP
		
		theApp.head.type = 0;
		theApp.search = 1;
	}
	CSocket::OnReceive(nErrorCode);
}

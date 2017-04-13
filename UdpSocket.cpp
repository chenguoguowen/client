// UdpSocket.cpp : 实现文件
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


// CUdpSocket 成员函数


void CUdpSocket::OnReceive(int nErrorCode)//客户端监听端口1000，发送端口1001
{
	// TODO: 在此添加专用代码和/或调用基类
	TCHAR recBuf[1024] = {0};
	int len = sizeof(SOCKADDR_IN);
	int recBytes = ReceiveFrom(recBuf, 1023, (SOCKADDR*)&m_serverAddr, &len, 0);//接受服务器的ip信息
	if(wcscmp(recBuf,L"服务器应答") == 0)
	{
		s = inet_ntoa(m_serverAddr.sin_addr);  //记录服务器的IP
		
		theApp.head.type = 0;
		theApp.search = 1;
	}
	CSocket::OnReceive(nErrorCode);
}

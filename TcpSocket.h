#pragma once
#include "Head.h"
// CTcpSocket ÃüÁîÄ¿±ê

class CTcpSocket : public CSocket
{
public:
	CTcpSocket();
	virtual ~CTcpSocket();
	virtual void OnReceive(int nErrorCode);
	void ChatUpdateMsg(CHead Msg);
	void UpdateListCtrl(CHead Msg); 
	void ReceiveFile();
};



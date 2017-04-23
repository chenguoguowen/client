
// Client.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "Login.h"
#include "ClientDlg.h"
#include "Head.h"
#include "UdpSocket.h"
#include "TcpSocket.h"
#include "Chat.h"
// CClientApp:
// 有关此类的实现，请参阅 Client.cpp
//

typedef struct _SOCKET_STREAM_FILE_INFO {

	TCHAR       szFileTitle[128];                   //文件的标题名
	DWORD       dwFileAttributes;                   //文件的属性
	FILETIME    ftCreationTime;                     //文件的创建时间
	FILETIME    ftLastAccessTime;                   //文件的最后访问时间
	FILETIME    ftLastWriteTime;                    //文件的最后修改时间
	DWORD       nFileSizeHigh;                      //文件大小的高位双字
	DWORD       nFileSizeLow;                       //文件大小的低位双字
	DWORD       dwReserved0;                        //保留，为0
	DWORD       dwReserved1;                        //保留，为0

} SOCKET_STREAM_FILE_INFO, *PSOCKET_STREAM_FILE_INFO;


class CClientApp : public CWinApp
{
public:
	CFont m_font;  //字体
	CClientApp();
	CHead head;
	CClientDlg *active;
	CLogin *log;
	CUdpSocket *m_udp;
	CTcpSocket *m_tcp;
	int search;//服务器搜寻标识
	int tag;//账号密码匹配标识
	CLogin login;//登录菜单
	CString serverip;//记录服务器IP
	CString m_localip;//本地IP
	CString m_userID;//账号
	CString m_userNAME;//用户名
	map<CString,CChat *> m_userMsg;//记录聊天对话框
	map<CString,CString> m_MsgRecode;//记录聊天信息
	map<CString,CString> m_clientMsg;//记录在线客户端ID == ip
	map<CString,CString> m_client;//ID == NAME
	
private:
	void GetLocalIp(CString &ip);
// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CClientApp theApp;    //全局变量存放信息
// TcpSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "TcpSocket.h"


// CTcpSocket

CTcpSocket::CTcpSocket()
{
}

CTcpSocket::~CTcpSocket()
{
}


// CTcpSocket 成员函数


void CTcpSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	int n;
	CString temp;
	CHead Msg;
	n=Receive(&Msg,sizeof(Msg));
	switch(Msg.type)
	{
	case 0:
	case 1:
	case 2:
		UpdateListCtrl(Msg);
		break;
	case 3:
		ChatUpdateMsg(Msg);
		break;
	case 4:    ////把用户信息发送给服务器
		if(Msg.tag != 0)
		{
			theApp.tag = 1;
			theApp.m_userNAME = Msg.usermsg.NAME;
		}
		SendMessage(theApp.log->GetSafeHwnd(),WM_MSG,0,0);
		break;
	case 5:
		theApp.m_userID = Msg.usermsg.ID;

		temp += L"恭喜您注册成功，账号是：" + theApp.m_userID + L"，请牢记！";

		theApp.log->m_userID = Msg.usermsg.ID;

		theApp.log->UpdateData(false);

		AfxMessageBox(temp);

		break;

	case 6://文件传输
		ReceiveFile();
	
		break;
	default:
		break;
	}
	CSocket::OnReceive(nErrorCode);
}


void CTcpSocket::ReceiveFile()
{

	CString fileName;

	SOCKET_STREAM_FILE_INFO StreamFileInfo;

	theApp.m_tcp->Receive(&StreamFileInfo, sizeof(SOCKET_STREAM_FILE_INFO));


	CFileDialog saveDlg(false, NULL, StreamFileInfo.szFileTitle, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL, 0);
	
	if (saveDlg.DoModal() == IDOK)
	{
		fileName = saveDlg.GetPathName();
	}
	else
	{
		//closesocket(sock);
		return;
	}
	//recvFileBuf[SIZEFILE]={0};
	CFile destFile(fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
	//CFile destFile(StreamFileInfo.szFileTitle, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

	UINT dwRead = 0;
	while (dwRead<StreamFileInfo.nFileSizeLow)
	{
		byte* data = new byte[1024];
		memset(data, 0, 1024);

		UINT dw = Receive(data, 1024);
		destFile.Write(data, dw);

		dwRead += dw;
	}

	SetFileTime((HANDLE)destFile.m_hFile, &StreamFileInfo.ftCreationTime,
		&StreamFileInfo.ftLastAccessTime, &StreamFileInfo.ftLastWriteTime);
	
	SetFileAttributes(StreamFileInfo.szFileTitle, StreamFileInfo.dwFileAttributes);
	
	destFile.Close();

	CString temp = (CString)theApp.head.fromID + L"对你发送文件，请查收！";
	AfxMessageBox(temp);
}

void CTcpSocket::ChatUpdateMsg(CHead Msg)//接收消息
{
	CTime time;

	time = CTime::GetCurrentTime();  //获取现在时间

	CString strTime = time.Format(L"%Y-%m-%d %H:%M:%S  ");

	map<CString,CChat*>::iterator it = theApp.m_userMsg.find((CString)Msg.fromID);

	CString temp;

	temp = L"(" + (CString)Msg.fromID + L") " + (CString)Msg.usermsg.NAME;

	if(it != theApp.m_userMsg.end())
	{
		//是否已与消息来源打开了聊天框，如果已经打开了，则处理消息；如果没有，则先打开对话框
		it->second->m_receiveMsg += strTime + temp + L"\r\n    " + (CString)Msg.msg + L"\r\n";


		theApp.m_MsgRecode[(CString)Msg.fromID] += strTime + temp + L"\r\n    " + (CString)Msg.msg + L"\r\n";


		it->second->UpdateData(false);

		it->second->m_receiveCtrl.LineScroll(it->second->m_receiveCtrl.GetLineCount()-1);
	}
	else
	{
		CChat* c;

		c = new CChat;

		c->m_toID = Msg.fromID;

		c->m_name = Msg.usermsg.NAME;

		c->Create(IDD_CHAT,theApp.active);

		c->ShowWindow(SW_SHOW);

		/*CString strTemp;
		strTemp.Format(L"%s",Msg.fromID);*/
		theApp.m_userMsg.insert(make_pair(Msg.fromID,c));



		/*消息记录*/
		map<CString, CString>::iterator map_it = theApp.m_MsgRecode.find((CString)Msg.fromID);

		if (map_it == theApp.m_MsgRecode.end())
		{
			theApp.m_MsgRecode.insert(make_pair(Msg.fromID, L""));
		}
		else
		{
			c->m_receiveMsg += theApp.m_MsgRecode[(CString)Msg.fromID];
		}



		c->m_receiveMsg += strTime + temp + L"\r\n    " + (CString)Msg.msg + L"\r\n";

		theApp.m_MsgRecode[(CString)Msg.fromID] += strTime + temp + L"\r\n    " + (CString)Msg.msg + L"\r\n";



		c->UpdateData(false);

		c->m_receiveCtrl.LineScroll(c->m_receiveCtrl.GetLineCount()-1);
	}
}

void CTcpSocket::UpdateListCtrl(CHead Msg)//更新在线用户列表
{
	if(Msg.type == 0)//有客户端退出
	{
		theApp.m_clientMsg.erase((CString)Msg.usermsg.ID);
		theApp.m_client.erase((CString)Msg.usermsg.ID);
	}
	else if(Msg.type == 1)//有客户端加入
	{
		theApp.m_clientMsg.insert(make_pair((CString)Msg.usermsg.ID,(CString)Msg.ip));
		theApp.m_client.insert(make_pair((CString)Msg.usermsg.ID,(CString)Msg.usermsg.NAME));

		CHead temp;

		temp.type = 2;

		strcpy_s(temp.fromID,(CStringA)theApp.m_userID);

		strcpy_s(temp.toID,Msg.fromID);

		strcpy_s(temp.usermsg.NAME,(CStringA)theApp.m_userNAME);

		strcpy_s(temp.ip,(CStringA)theApp.m_localip);

		strcpy_s(temp.usermsg.ID,(CStringA)theApp.m_userID);

		theApp.m_tcp->Send(&temp,sizeof(temp));
	}
	else if(Msg.type == 2)//我已在线……
	{
		theApp.m_clientMsg.insert(make_pair((CString)Msg.usermsg.ID,(CString)Msg.ip));

		theApp.m_client.insert(make_pair((CString)Msg.usermsg.ID,(CString)Msg.usermsg.NAME));
	}
	map<CString,CString>::iterator it;

	int i;

	theApp.active->m_listcontrol.DeleteAllItems();

	for(it = theApp.m_clientMsg.begin(),i = 0;it != theApp.m_clientMsg.end();it++,i++)
	{
		theApp.active->m_listcontrol.InsertItem(i, it->first);
		theApp.active->m_listcontrol.SetItemText(i,1,theApp.m_client.find(it->first)->second);

		theApp.active->m_listcontrol.SetItemText(i, 2, it->second);
	}
}
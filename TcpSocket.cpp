// TcpSocket.cpp : ʵ���ļ�
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


// CTcpSocket ��Ա����


void CTcpSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
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
	case 4:    ////���û���Ϣ���͸�������
		if(Msg.tag != 0)
		{
			theApp.tag = 1;
			theApp.m_userNAME = Msg.usermsg.NAME;
		}
		SendMessage(theApp.log->GetSafeHwnd(),WM_MSG,0,0);
		break;
	case 5:
		theApp.m_userID = Msg.usermsg.ID;

		temp += L"��ϲ��ע��ɹ����˺��ǣ�" + theApp.m_userID + L"�����μǣ�";

		theApp.log->m_userID = Msg.usermsg.ID;

		theApp.log->UpdateData(false);

		AfxMessageBox(temp);

		break;

	case 6://�ļ�����
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

	CString temp = (CString)theApp.head.fromID + L"���㷢���ļ�������գ�";
	AfxMessageBox(temp);
}

void CTcpSocket::ChatUpdateMsg(CHead Msg)//������Ϣ
{
	CTime time;

	time = CTime::GetCurrentTime();  //��ȡ����ʱ��

	CString strTime = time.Format(L"%Y-%m-%d %H:%M:%S  ");

	map<CString,CChat*>::iterator it = theApp.m_userMsg.find((CString)Msg.fromID);

	CString temp;

	temp = L"(" + (CString)Msg.fromID + L") " + (CString)Msg.usermsg.NAME;

	if(it != theApp.m_userMsg.end())
	{
		//�Ƿ�������Ϣ��Դ�������������Ѿ����ˣ�������Ϣ�����û�У����ȴ򿪶Ի���
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



		/*��Ϣ��¼*/
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

void CTcpSocket::UpdateListCtrl(CHead Msg)//���������û��б�
{
	if(Msg.type == 0)//�пͻ����˳�
	{
		theApp.m_clientMsg.erase((CString)Msg.usermsg.ID);
		theApp.m_client.erase((CString)Msg.usermsg.ID);
	}
	else if(Msg.type == 1)//�пͻ��˼���
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
	else if(Msg.type == 2)//�������ߡ���
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
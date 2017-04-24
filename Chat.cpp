// Chat.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "Chat.h"
#include "afxdialogex.h"
#include "MsgRecode.h"


// CChat �Ի���

IMPLEMENT_DYNAMIC(CChat, CDialogEx)

CChat::CChat(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChat::IDD, pParent)
	, m_receiveMsg(L"")
	, m_sendMsg(L"")
{

}

CChat::~CChat()
{
}

void CChat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RECEIVEMSG, m_receiveMsg);
	DDX_Text(pDX, IDC_SENDMSG, m_sendMsg);
	DDX_Control(pDX, IDC_RECEIVEMSG, m_receiveCtrl);

	/*DDX_Control(pDX, IDC_RECEIVEMSG, m_richEditCtrl);
	DDX_Control(pDX, IDC_SENDMSG, m_sendRichEditCtrl);*/
	DDX_Control(pDX, IDC_SENDMSG, m_editControl);
}


BEGIN_MESSAGE_MAP(CChat, CDialogEx)
	ON_BN_CLICKED(IDC_SEND, &CChat::OnBnClickedSend)
	ON_BN_CLICKED(IDC_CANCEL, &CChat::OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SendFile, &CChat::OnBnClickedSendfile)
	ON_BN_CLICKED(IDC_BUTTON2, &CChat::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CChat::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CChat::OnBnClickedButton3)
END_MESSAGE_MAP()


// CChat ��Ϣ�������


void CChat::OnBnClickedSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CTime time;
	time = CTime::GetCurrentTime();  //��ȡ����ʱ��
	CString strTime = time.Format(L"%Y-%m-%d %H:%M:%S  ");
	theApp.head.type = 3;
	UpdateData(true);

	if (m_sendMsg.IsEmpty())
	{
		MessageBox(L"��Ϣ����Ϊ�գ�",L"��ʾ",MB_OK);
		return;
	}

	/*��Ϣ��¼*/
	//map<CString, CString>::iterator map_it = theApp.m_MsgRecode.find(m_toID);

	//if (map_it != theApp.m_MsgRecode.end())
	//{
	//	//vector<CString>::iterator it = map_it->second.begin();
	//	
	//	 m_receiveMsg += theApp.m_MsgRecode[m_toID];
	//}
	//else
	//{         
	//	theApp.m_MsgRecode.insert(make_pair(m_toID, L""));
	//}



	m_receiveMsg += strTime + L"��" + theApp.m_userID + L"�� " + theApp.m_userNAME + L"\r\n    " + m_sendMsg + L"\r\n";


	theApp.m_MsgRecode[m_toID] += strTime + L"��" + theApp.m_userID + L"�� " + theApp.m_userNAME + L"\r\n    " + m_sendMsg + L"\r\n";


	CHead Msg;
	Msg.type = 3;
	strcpy_s(Msg.fromID,(CStringA)theApp.m_userID);
	strcpy_s(Msg.usermsg.NAME,(CStringA)theApp.m_userNAME);
	strcpy_s(Msg.usermsg.ID,(CStringA)theApp.m_userID);
	strcpy_s(Msg.toID,(CStringA)m_toID);
	strcpy_s(Msg.ip,theApp.head.ip);
	strcpy_s(Msg.msg,(CStringA)m_sendMsg);
	m_sendMsg = L"";
	UpdateData(false);
	m_receiveCtrl.LineScroll(m_receiveCtrl.GetLineCount()-1);
	theApp.m_tcp->Send(&Msg,sizeof(Msg));
}


void CChat::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnClose();
	CDialogEx::OnCancel();
}


BOOL CChat::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_caption =(CString)"���롾" +  m_toID + (CString)"�� " + m_name + (CString)" ������";
	SetWindowTextW(m_caption);//���öԻ������

	HICON m_hIcon;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);//IDR_ICONΪͼ����Դ��
	SetIcon(m_hIcon, TRUE); // Set big icon
	SetIcon(m_hIcon, FALSE); // Set small icon
	
	if(theApp.m_font.m_hObject)
	{
		// ��ȡ�༭��IDC_FONT_EDIT��CWndָ�룬������������   
		GetDlgItem(IDC_SENDMSG)->SetFont(&theApp.m_font);
		GetDlgItem(IDC_RECEIVEMSG)->SetFont(&theApp.m_font);
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CChat::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	theApp.m_userMsg.erase(m_toID);
	CDialogEx::OnClose();
}


void CChat::OnBnClickedSendfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//CFileDialog m_filedlg(TRUE);
	//if (m_filedlg.DoModal() == IDOK)
	//{
	//	CString str = m_filedlg.GetPathName();
	//	//this->SetDlgItemTextW(IDC_FILEPATHNAME, str);
	//}
	//else
	//{
	//	return;
	//}

	UpdateData(true);
	CHead Msg;
	Msg.type = 6;

	strcpy_s(Msg.fromID, (CStringA)theApp.m_userID);

	strcpy_s(Msg.usermsg.NAME, (CStringA)theApp.m_userNAME);

	strcpy_s(Msg.usermsg.ID, (CStringA)theApp.m_userID);

	strcpy_s(Msg.toID, (CStringA)m_toID);

	strcpy_s(Msg.ip, theApp.head.ip);

	strcpy_s(Msg.msg, (CStringA)(L"���" +m_name+ L"�����ļ�"));

	m_receiveCtrl.LineScroll(m_receiveCtrl.GetLineCount() - 1);

	theApp.m_tcp->Send(&Msg, sizeof(Msg));

	m_receiveMsg +=  L"��" + theApp.m_userID + L"�� " + theApp.m_userNAME + L"\r\n    " + L"���" +m_name+ L"�����ļ�" + L"\r\n";


	theApp.m_MsgRecode[m_toID] +=  L"��" + theApp.m_userID + L"�� " + theApp.m_userNAME + L"\r\n    " + L"���" +  m_name+ L"�����ļ�" + L"\r\n";

	UpdateData(false);

	CFileDialog	Dlg(TRUE);
	if (Dlg.DoModal() != IDOK)
		return;

	CFile myFile;
	if (!myFile.Open(Dlg.GetPathName(), CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(L"�ļ�������!");
		return;
	}

	//CSocket sockSrvr;
	//sockSrvr.Create(800);

	//sockSrvr.Listen();
	//CSocket sockRecv;
	//sockSrvr.Accept(sockRecv);

	SOCKET_STREAM_FILE_INFO	StreamFileInfo;
	WIN32_FIND_DATA             FindFileData;

	FindClose(FindFirstFile(Dlg.GetPathName(), &FindFileData));
	memset(&StreamFileInfo, 0, sizeof(SOCKET_STREAM_FILE_INFO));
	//StreamFileInfo.szFileTitle = myFile.GetFileTitle();
	wsprintf(StreamFileInfo.szFileTitle, L"%s", myFile.GetFileTitle());
	//strcpy_s(StreamFileInfo.szFileTitle, myFile.GetFileTitle());

	int a = 0;
	StreamFileInfo.dwFileAttributes = FindFileData.dwFileAttributes;
	StreamFileInfo.ftCreationTime = FindFileData.ftCreationTime;
	StreamFileInfo.ftLastAccessTime = FindFileData.ftLastAccessTime;
	StreamFileInfo.ftLastWriteTime = FindFileData.ftLastWriteTime;
	StreamFileInfo.nFileSizeHigh = FindFileData.nFileSizeHigh;
	StreamFileInfo.nFileSizeLow = FindFileData.nFileSizeLow;

	theApp.m_tcp->Send(&StreamFileInfo, sizeof(SOCKET_STREAM_FILE_INFO));

	UINT dwRead = 0;
	while (dwRead<StreamFileInfo.nFileSizeLow)
	{
		byte* data = new byte[1024];
		UINT dw = myFile.Read(data, 1024);
		theApp.m_tcp->Send(data, dw);
		dwRead += dw;
	}
	myFile.Close();

	//sockRecv.Close();
	AfxMessageBox(L"������ϣ�");
}


void CChat::OnBnClickedButton2()  //��������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strFontName;    // ��������   

	LOGFONT lf;             // LOGFONT����   

							// ��lf�����ֽ�����   
	memset(&lf, 0, sizeof(LOGFONT));

	// ��lf�е�Ԫ����������Ϊ�����塱   
	_tcscpy_s(lf.lfFaceName, LF_FACESIZE, _T("����"));

	// ��������Ի��򣬳�ʼѡ��������Ϊ�����塱   
	CFontDialog fontDlg(&lf);

	if (IDOK == fontDlg.DoModal())     // ��ʾ����Ի���   
	{
		// ���m_font�Ѿ�������һ��������Դ�������ͷ���   
		if (theApp.m_font.m_hObject)
		{
			theApp.m_font.DeleteObject();
		}
		// ʹ��ѡ�������LOGFONT�����µ�����   
		theApp.m_font.CreateFontIndirect(fontDlg.m_cf.lpLogFont);
	    
		// ��ȡ�༭��IDC_FONT_EDIT��CWndָ�룬������������   
		GetDlgItem(IDC_SENDMSG)->SetFont(&theApp.m_font);
		GetDlgItem(IDC_RECEIVEMSG)->SetFont(&theApp.m_font);
		
	}

	//CFont * f;
	//CFontDialog font;
	//if (IDOK == font.DoModal())
	//{

	//	f = new CFont;
	//	f->CreateFont(16,            // nHeight 
	//		0,           // nWidth 
	//		0,           // nEscapement 
	//		0,           // nOrientation 
	//		FW_BOLD,     // nWeight 
	//		TRUE,        // bItalic 
	//		FALSE,       // bUnderline 
	//		0,           // cStrikeOut 
	//		ANSI_CHARSET,              // nCharSet 
	//		OUT_DEFAULT_PRECIS,        // nOutPrecision 
	//		CLIP_DEFAULT_PRECIS,       // nClipPrecision 
	//		DEFAULT_QUALITY,           // nQuality 
	//		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
	//		_T("Arial"));              // lpszFac

	//	m_editControl.SetFont(f);
	//	m_receiveCtrl.SetFont(f);
	//	Invalidate();
	//}

}




void CChat::OnBnClickedButton4()   //�鿴�����¼
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CMsgRecode* MsgRecode = new CMsgRecode; 

	UpdateData(true);
	MsgRecode->m_MsgRecode = theApp.m_MsgRecode[m_toID];
	MsgRecode->toID = m_toID;
	UpdateData(false);

	MsgRecode->Create(IDD_MessageRecode, this);
	MsgRecode->ShowWindow(SW_SHOW);
}


void CChat::OnBnClickedButton3()  //���������¼
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CTime time;
	CString sPath;
	time = CTime::GetCurrentTime();  //��ȡ����ʱ��
	CString strTime = time.Format(L"%Y-%m-%d");

	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);//���ظõ�ǰӦ�ó���*.exe��ȫ·��
	sPath.ReleaseBuffer();
	int nPos;
	nPos = sPath.ReverseFind('\\');//ReverseFind()��һ���ϴ���ַ����д�ĩ�˿�ʼ����ĳ���ַ�
	sPath = sPath.Left(nPos);
	sPath += L"\\" + strTime +"-"+ m_toID + L".txt";

	CFile file;
	if (!PathFileExists(sPath))
	{
		file.Open(sPath, CFile::modeCreate|CFile::modeWrite);
		file.Close();
	}

	
	fstream out(sPath, fstream::out);
	if (out)
	{		
		CString b = theApp.m_MsgRecode[m_toID];
		string a = CT2A(b);
	
		out << a;
	}	

	out.close(); 
}

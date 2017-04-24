// Chat.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "Chat.h"
#include "afxdialogex.h"
#include "MsgRecode.h"


// CChat 对话框

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


// CChat 消息处理程序


void CChat::OnBnClickedSend()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime time;
	time = CTime::GetCurrentTime();  //获取现在时间
	CString strTime = time.Format(L"%Y-%m-%d %H:%M:%S  ");
	theApp.head.type = 3;
	UpdateData(true);

	if (m_sendMsg.IsEmpty())
	{
		MessageBox(L"消息不能为空！",L"提示",MB_OK);
		return;
	}

	/*消息记录*/
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



	m_receiveMsg += strTime + L"【" + theApp.m_userID + L"】 " + theApp.m_userNAME + L"\r\n    " + m_sendMsg + L"\r\n";


	theApp.m_MsgRecode[m_toID] += strTime + L"【" + theApp.m_userID + L"】 " + theApp.m_userNAME + L"\r\n    " + m_sendMsg + L"\r\n";


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
	// TODO: 在此添加控件通知处理程序代码
	OnClose();
	CDialogEx::OnCancel();
}


BOOL CChat::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_caption =(CString)"正与【" +  m_toID + (CString)"】 " + m_name + (CString)" 聊天中";
	SetWindowTextW(m_caption);//设置对话框标题

	HICON m_hIcon;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);//IDR_ICON为图标资源名
	SetIcon(m_hIcon, TRUE); // Set big icon
	SetIcon(m_hIcon, FALSE); // Set small icon
	
	if(theApp.m_font.m_hObject)
	{
		// 获取编辑框IDC_FONT_EDIT的CWnd指针，并设置其字体   
		GetDlgItem(IDC_SENDMSG)->SetFont(&theApp.m_font);
		GetDlgItem(IDC_RECEIVEMSG)->SetFont(&theApp.m_font);
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CChat::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	theApp.m_userMsg.erase(m_toID);
	CDialogEx::OnClose();
}


void CChat::OnBnClickedSendfile()
{
	// TODO: 在此添加控件通知处理程序代码

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

	strcpy_s(Msg.msg, (CStringA)(L"你对" +m_name+ L"发送文件"));

	m_receiveCtrl.LineScroll(m_receiveCtrl.GetLineCount() - 1);

	theApp.m_tcp->Send(&Msg, sizeof(Msg));

	m_receiveMsg +=  L"【" + theApp.m_userID + L"】 " + theApp.m_userNAME + L"\r\n    " + L"你对" +m_name+ L"发送文件" + L"\r\n";


	theApp.m_MsgRecode[m_toID] +=  L"【" + theApp.m_userID + L"】 " + theApp.m_userNAME + L"\r\n    " + L"你对" +  m_name+ L"发送文件" + L"\r\n";

	UpdateData(false);

	CFileDialog	Dlg(TRUE);
	if (Dlg.DoModal() != IDOK)
		return;

	CFile myFile;
	if (!myFile.Open(Dlg.GetPathName(), CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(L"文件不存在!");
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
	AfxMessageBox(L"发送完毕！");
}


void CChat::OnBnClickedButton2()  //字体设置
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFontName;    // 字体名称   

	LOGFONT lf;             // LOGFONT变量   

							// 将lf所有字节清零   
	memset(&lf, 0, sizeof(LOGFONT));

	// 将lf中的元素字体名设为“宋体”   
	_tcscpy_s(lf.lfFaceName, LF_FACESIZE, _T("宋体"));

	// 构造字体对话框，初始选择字体名为“宋体”   
	CFontDialog fontDlg(&lf);

	if (IDOK == fontDlg.DoModal())     // 显示字体对话框   
	{
		// 如果m_font已经关联了一个字体资源对象，则释放它   
		if (theApp.m_font.m_hObject)
		{
			theApp.m_font.DeleteObject();
		}
		// 使用选定字体的LOGFONT创建新的字体   
		theApp.m_font.CreateFontIndirect(fontDlg.m_cf.lpLogFont);
	    
		// 获取编辑框IDC_FONT_EDIT的CWnd指针，并设置其字体   
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




void CChat::OnBnClickedButton4()   //查看聊天记录
{
	// TODO: 在此添加控件通知处理程序代码

	CMsgRecode* MsgRecode = new CMsgRecode; 

	UpdateData(true);
	MsgRecode->m_MsgRecode = theApp.m_MsgRecode[m_toID];
	MsgRecode->toID = m_toID;
	UpdateData(false);

	MsgRecode->Create(IDD_MessageRecode, this);
	MsgRecode->ShowWindow(SW_SHOW);
}


void CChat::OnBnClickedButton3()  //保存聊天记录
{
	// TODO: 在此添加控件通知处理程序代码
	CTime time;
	CString sPath;
	time = CTime::GetCurrentTime();  //获取现在时间
	CString strTime = time.Format(L"%Y-%m-%d");

	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);//返回该当前应用程序*.exe的全路径
	sPath.ReleaseBuffer();
	int nPos;
	nPos = sPath.ReverseFind('\\');//ReverseFind()在一个较大的字符串中从末端开始查找某个字符
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

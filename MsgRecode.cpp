// MsgRecode.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "MsgRecode.h"
#include "afxdialogex.h"
#include <shlwapi.h>

#pragma comment(lib,"Shlwapi.lib") 

// CMsgRecode 对话框

IMPLEMENT_DYNAMIC(CMsgRecode, CDialogEx)

CMsgRecode::CMsgRecode(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MessageRecode, pParent)
	, m_MsgRecode(_T(""))
{
	GetPathRecode(sPath, L"\\res\\remember.txt");   //记录之前打开过的账号
	ReadRecode();
}

CMsgRecode::~CMsgRecode()
{
}



void CMsgRecode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_MsgRecode);
	//  DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_DateTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DateTime);
}


BEGIN_MESSAGE_MAP(CMsgRecode, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMsgRecode::OnBnClickedOk)
END_MESSAGE_MAP()

// CMsgRecode 消息处理程序

void CMsgRecode::GetPathRecode(CString &sPath, CString filename)
{
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);//返回该当前应用程序*.exe的全路径
	sPath.ReleaseBuffer();
	int nPos;
	nPos = sPath.ReverseFind('\\');//ReverseFind()在一个较大的字符串中从末端开始查找某个字符
	sPath = sPath.Left(nPos);
	nPos = sPath.ReverseFind('\\');
	sPath = sPath.Left(nPos);
	sPath += filename;
}
void CMsgRecode::ReadRecode()
{
	//fstream infile(sPath, fstream::in);
	//if (infile)
	//{
	//	string temp;
	//	while (!infile.eof())
	//	{
	//		infile >> temp;
	//		vec.push_back(temp);
	//	}
	//}
	//infile.close();
}

void CMsgRecode::WriteRecode()
{
	/*fstream out(sPath, fstream::out);
	if (out)
	{
		out << vec[0];
		for (int i = 1; i != vec.size(); i++)
			out << endl << vec[i];
	}
	out.close();*/
}

void CMsgRecode::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime tm;
	
	m_DateTime.GetTime(tm);
	CString strDate = tm.Format("%Y-%m-%d");
	UpdateData(true);
	m_MsgRecode = L"";
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);//返回该当前应用程序*.exe的全路径
	sPath.ReleaseBuffer();
	int nPos;
	nPos = sPath.ReverseFind('\\');//ReverseFind()在一个较大的字符串中从末端开始查找某个字符
	sPath = sPath.Left(nPos);
	sPath += L"\\" + strDate +"-"+ toID + L".txt" ;

	if (PathFileExists(sPath))
	{
		//存在
		fstream infile(sPath, fstream::in);
		if (infile)
		{
			//string temp;
			char temp[255];
			while (!infile.eof())
			{
				//infile >> temp;
				infile.getline(temp,255);
				m_MsgRecode += temp;
				m_MsgRecode += L"\n";
			}
		}
		infile.close();

	}
	else
	{
		m_MsgRecode = L"没有这天的聊天记录！";
	}
	UpdateData(false);
}

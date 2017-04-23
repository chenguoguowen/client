// MsgRecode.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "MsgRecode.h"
#include "afxdialogex.h"


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
}


BEGIN_MESSAGE_MAP(CMsgRecode, CDialogEx)
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
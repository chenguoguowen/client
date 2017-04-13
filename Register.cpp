// Register.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "Register.h"
#include "afxdialogex.h"


// CRegister �Ի���

IMPLEMENT_DYNAMIC(CRegister, CDialogEx)

CRegister::CRegister(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegister::IDD, pParent)
	, name(L"")
	, mima(L"")
	, mima2(L"")
{

}

CRegister::~CRegister()
{
}

void CRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITNAME, name);
	DDX_Text(pDX, IDC_EDITMIMA, mima);
	DDX_Text(pDX, IDC_EDITMIMA2, mima2);
}


BEGIN_MESSAGE_MAP(CRegister, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRegister::OnBnClickedOk)
END_MESSAGE_MAP()


// CRegister ��Ϣ�������


void CRegister::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(name == ""||mima == ""||mima2 == "")
		return;
	if(mima != mima2)
	{
		MessageBox(L"������������벻һ�£����������룡");
		return;
	}
	CDialogEx::OnOK();
}

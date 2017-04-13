#pragma once
#include "Register.h"
#include "afxwin.h"
// CLogin �Ի���

class CLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLogin();

// �Ի�������
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg LRESULT OnMsg(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	int tcp_tag;        //�Ƿ���ڱ�־
	CRegister m_register;
	CString m_userID;
	CString m_password;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CComboBox m_remember;
	CString sPath;
	vector<string> vec;
	void GetPath(CString &sPath,CString filename);
	virtual BOOL OnInitDialog();
	void Read();
	void Write();
};

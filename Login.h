#pragma once
#include "Register.h"
#include "afxwin.h"
// CLogin 对话框

class CLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogin();

// 对话框数据
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnMsg(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	int tcp_tag;        //是否存在标志
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

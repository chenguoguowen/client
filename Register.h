#pragma once


// CRegister 对话框

class CRegister : public CDialogEx
{
	DECLARE_DYNAMIC(CRegister) 

public:
	CRegister(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegister();

// 对话框数据
	enum { IDD = IDD_REGISTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString name;
	CString mima;
	CString mima2;
	afx_msg void OnBnClickedOk();
};

#pragma once


// CRegister �Ի���

class CRegister : public CDialogEx
{
	DECLARE_DYNAMIC(CRegister) 

public:
	CRegister(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegister();

// �Ի�������
	enum { IDD = IDD_REGISTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString name;
	CString mima;
	CString mima2;
	afx_msg void OnBnClickedOk();
};

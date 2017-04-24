#pragma once
#include "ATLComTime.h"
#include "afxdtctl.h"


// CMsgRecode �Ի���

class CMsgRecode : public CDialogEx
{
	DECLARE_DYNAMIC(CMsgRecode)

public:
	CMsgRecode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMsgRecode();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MessageRecode };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_MsgRecode;
	CString toID;
	CString sPath;
	void ReadRecode();
	void WriteRecode();	
	void GetPathRecode(CString &sPath, CString filename);
	afx_msg void OnBnClickedOk();

	CDateTimeCtrl m_DateTime;
};

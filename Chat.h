#pragma once
#include "afxwin.h"


// CChat �Ի��� 


class CChat : public CDialogEx
{
	DECLARE_DYNAMIC(CChat)

public:
	CChat(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChat();

// �Ի�������
	enum { IDD = IDD_CHAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_caption;//����
	CString m_toID;//�������
	CString m_name;
	CString m_receiveMsg;
	CString m_sendMsg;
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	CEdit m_receiveCtrl;
	CEdit m_editControl;
	afx_msg void OnBnClickedSendfile();
	afx_msg void OnBnClickedButton2();

	
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
};

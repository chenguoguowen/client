
// ClientDlg.h : ͷ�ļ�
//

#pragma once
#include "TcpSocket.h"
#include "Head.h"
#include "afxcmn.h"
#include "afxwin.h"

#define WM_SHOWTASK (WM_USER +1)
// CClientDlg �Ի���
class CClientDlg : public CDialogEx
{
// ����
public:
	CClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY_DIALOG };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	CListCtrl m_listcontrol;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedQuit();
public://�Զ������
	CString m_caption;
	CString m_StateStr;
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox m_comboxBox;
	CStatic m_Picture;
	afx_msg void OnBnClickedButton1();
};

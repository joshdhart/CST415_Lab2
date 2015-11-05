
// CST415_Lab2Dlg.h : header file
//

#pragma once
#include "afxwin.h"


// CCST415_Lab2Dlg dialog
class CCST415_Lab2Dlg : public CDialogEx
{
// Construction
public:
	CCST415_Lab2Dlg(CWnd* pParent = NULL);	// standard constructor

	RequestMessagePacket _reqPacket;
	ResponseMessagePacket _rspPacket;

	SOCKET _connectSocket;
	struct sockaddr_in _clientInfo;
	struct sockaddr_in _serverInfo;

	unsigned long _nSysStartTimeMs;
	string _strReqPack;

// Dialog Data
	enum { IDD = IDD_CST415_LAB2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	bool AttemptTCPConnection();
	void Do100Transactions();
	void ConstructReqPackStr();
	void SynchronousSend_Receive();
	void AddToWindowLog(CString strItem);


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartButton();
private:
	CListBox m_lstLog;
};

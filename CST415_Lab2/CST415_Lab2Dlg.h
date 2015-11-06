
// CST415_Lab2Dlg.h : header file
//

#pragma once
#include "AsyncClient.h"
#include "afxwin.h"
#include "Resource.h"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <stdio.h>


// CCST415_Lab2Dlg dialog
class CCST415_Lab2Dlg : public CDialogEx
{
// Construction
public:
	CCST415_Lab2Dlg(CWnd* pParent = NULL);	// standard constructor

	void AsyncReceive();

	RequestMessagePacket _reqPacket;
	ResponseMessagePacket _rspPacket;

	SOCKET _connectSocket;

	ofstream _instructorLog;

	int _nClientPort;
	string _strClientIp;
	string _strReqPack;
	string _strRspPack;

// Dialog Data
	enum { IDD = IDD_CST415_LAB2_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	bool AttemptTCPConnection();
	static UINT Do100Transactions( LPVOID pParam );
	void ConstructReqPackStr();
	void SynchronousSend_Receive();
	void AddToWindowLog(CString strItem);
	void AddToInstructorLog(string strItem);
	void AddTrailerToInstructorLog();

// Implementation
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartButton();
private:
	CAsyncClient* m_asyncClient;

	CListBox m_lstLog;
};

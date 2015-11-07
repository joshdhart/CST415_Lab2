
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
#include <vector>


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

	bool _bLatent;
	int _nCurrentId;
	int _nClientPort;
	string _strClientIp;
	string _strReqPack;
	string _strRspPack;

	int _allLatents[100];
	vector<string> _allReqPacks;
	bool _allRspPacks[100];
	//vector<string> _allRspPacks;

// Dialog Data
	enum { IDD = IDD_CST415_LAB2_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	static UINT Do100Transactions(LPVOID pParam);
	static UINT TrackLatancy(LPVOID pParam);

	bool AttemptTCPConnection();
	void ConstructReqPackStr();
	void SynchronousSend_Receive();
	void AddToWindowLog(CString strItem);
	void AddToInstructorLog(string strItem);
	void AddTrailerToInstructorLog();
	void DoStandInRsp(int id);

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

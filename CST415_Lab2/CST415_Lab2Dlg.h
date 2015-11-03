
// CST415_Lab2Dlg.h : header file
//

#pragma once


// CCST415_Lab2Dlg dialog
class CCST415_Lab2Dlg : public CDialogEx
{
// Construction
public:
	CCST415_Lab2Dlg(CWnd* pParent = NULL);	// standard constructor

	RequestMessagePacket _reqPacket;
	ResponseMessagePacket _rspPacket;

	SOCKET _connectSocket;

// Dialog Data
	enum { IDD = IDD_CST415_LAB2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};

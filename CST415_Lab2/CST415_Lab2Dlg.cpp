
// CST415_Lab2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "CST415_Lab2.h"
#include "CST415_Lab2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCST415_Lab2Dlg dialog



CCST415_Lab2Dlg::CCST415_Lab2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCST415_Lab2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCST415_Lab2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCST415_Lab2Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CCST415_Lab2Dlg message handlers

BOOL CCST415_Lab2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	_connectSocket = INVALID_SOCKET;
	_reqPacket.MessageType = "REQ";
	_reqPacket.StudentName = "HartwellJ";
	_reqPacket.StudentID = "19-5193";
	_reqPacket.ResponseDelay = 0;
	_reqPacket.ForeignHostIPAddress = SERVER_IP_STRING;
	_reqPacket.ForeignHostServicePort = SERVICE_PORT_NUM;
	_reqPacket.StudentData = "blah";
	_reqPacket.ScenarioNo = 1;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCST415_Lab2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCST415_Lab2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



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
	DDX_Control(pDX, IDC_COMM_LOG_LIST, m_lstLog);
}

BEGIN_MESSAGE_MAP(CCST415_Lab2Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START_BUTTON, &CCST415_Lab2Dlg::OnBnClickedStartButton)
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

bool CCST415_Lab2Dlg::AttemptTCPConnection()
{
	WSADATA wsaData;

	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	int iResult;
	CString errorMsg;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		errorMsg.Format(L"WSAStartup failed with error: %d\n", iResult);
		m_lstLog.AddString(errorMsg);
		return false;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(SERVER_IP_STRING, SERVICE_PORT_STRING, &hints, &result);
	if (iResult != 0) {
		errorMsg.Format(L"getaddrinfo failed with error: %d\n", iResult);
		m_lstLog.AddString(errorMsg);
		WSACleanup();
		return false;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		_connectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (_connectSocket == INVALID_SOCKET) {
			errorMsg.Format(L"socket failed with error: %ld\n", WSAGetLastError());
			m_lstLog.AddString(errorMsg);
			WSACleanup();
			return false;
		}

		// Connect to server.
		iResult = connect(_connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(_connectSocket);
			_connectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (_connectSocket == INVALID_SOCKET) {
		errorMsg.Format(L"Unable to connect to server!\n");
		m_lstLog.AddString(errorMsg);
		WSACleanup();
		return false;
	}

	getsockname(_connectSocket, (struct sockaddr*)&_clientInfo, (int*)sizeof(_clientInfo));
	getpeername(_connectSocket, (struct sockaddr*)&_serverInfo, (int*)sizeof(_serverInfo));

	return true;
}

void CCST415_Lab2Dlg::Do100Transactions()
{
	_reqPacket.ClientSocketNo = (int)_connectSocket;
	_reqPacket.ClientIPAddress = inet_ntoa(_clientInfo.sin_addr);
	_reqPacket.ForeignHostIPAddress = inet_ntoa(_serverInfo.sin_addr);

	for (int i = 0; i < 100; i++)
	{
		_reqPacket.RequestID = std::to_string(i);
		// TODO: Send/Receive	
	}
}


// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCST415_Lab2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCST415_Lab2Dlg::OnBnClickedStartButton()
{
	if (AttemptTCPConnection())
	{
		Do100Transactions();
	}
}

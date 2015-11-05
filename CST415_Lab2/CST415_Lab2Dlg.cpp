
// CST415_Lab2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "CST415_Lab2.h"
#include "CST415_Lab2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


string Dec2Bin(unsigned n)
{
	char result[(sizeof(unsigned) * 8) + 1];
	unsigned index = sizeof(unsigned) * 8;
	result[index] = '\0';
	do result[--index] = '0' + (n & 1);
	while (n >>= 1);
	return string(result + index);
}


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

void CCST415_Lab2Dlg::AddToWindowLog(CString strItem)
{
	m_lstLog.AddString(strItem);
	UpdateWindow();
}

void CCST415_Lab2Dlg::AddToInstructorLog(string strItem)
{
	_instructorLog.open("Lab2.Scenario1.HartwellJ.txt", fstream::out | fstream::app);
	strItem += "<CR><LF>";
	_instructorLog << strItem;
	_instructorLog.close();
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
		errorMsg.Format(L"WSAStartup failed with error: %d", iResult);
		AddToWindowLog(errorMsg);
		return false;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(SERVER_IP_STRING, SERVICE_PORT_STRING, &hints, &result);
	if (iResult != 0) {
		errorMsg.Format(L"getaddrinfo failed with error: %d", iResult);
		AddToWindowLog(errorMsg);
		WSACleanup();
		return false;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		_connectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (_connectSocket == INVALID_SOCKET) {
			errorMsg.Format(L"socket failed with error: %ld", WSAGetLastError());
			AddToWindowLog(errorMsg);
			WSACleanup();
			return false;
		}

		// Connect to server.
		AddToWindowLog(L"Attempting to Connect...");
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
		errorMsg.Format(L"Unable to connect to server!");
		AddToWindowLog(errorMsg);
		WSACleanup();
		return false;
	}

	AddToWindowLog(L"Successfully connected");

	getsockname(_connectSocket, (struct sockaddr*)&_clientInfo, (int*)sizeof(_clientInfo));
	getpeername(_connectSocket, (struct sockaddr*)&_serverInfo, (int*)sizeof(_serverInfo));

	return true;
}

void CCST415_Lab2Dlg::Do100Transactions()
{
	_nSysStartTimeMs = GetTickCount64();

	_reqPacket.ClientSocketNo = (int)_connectSocket;
	_reqPacket.ClientIPAddress = inet_ntoa(_clientInfo.sin_addr);
	_reqPacket.ClientServicePort = _clientInfo.sin_port;
	//_reqPacket.ForeignHostIPAddress = inet_ntoa(_serverInfo.sin_addr);
	//_reqPacket.ForeignHostServicePort = _serverInfo.sin_port;

	for (int i = 0; i < 100; i++)
	{
		_reqPacket.RequestID = to_string(i);
		_reqPacket.msTimeStamp = (GetTickCount64() - _nSysStartTimeMs);
		// TODO: Send/Receive
		ConstructReqPackStr();
		SynchronousSend_Receive();
		Sleep(50);	// Must wait at least 50ms between transmissions
	}
}

void CCST415_Lab2Dlg::ConstructReqPackStr()
{
	string strReqPack;

	strReqPack = (_reqPacket.MessageType + '|');
	strReqPack += (to_string(_reqPacket.msTimeStamp) + '|');
	strReqPack += (_reqPacket.RequestID + '|');
	strReqPack += (_reqPacket.StudentName + '|');
	strReqPack += (_reqPacket.StudentID + '|');
	strReqPack += (to_string(_reqPacket.ResponseDelay) + '|');
	strReqPack += (_reqPacket.ClientIPAddress + '|');
	strReqPack += (to_string(_reqPacket.ClientServicePort) + '|');
	strReqPack += (to_string(_reqPacket.ClientSocketNo) + '|');
	strReqPack += (_reqPacket.ForeignHostIPAddress + '|');
	strReqPack += (to_string(_reqPacket.ForeignHostServicePort) + '|');
	strReqPack += (_reqPacket.StudentData + '|');
	strReqPack += (to_string(_reqPacket.ScenarioNo) + '|');

	int nByteSize = strReqPack.size();
	if (nByteSize < 127)
	{
		char cByte = (char)nByteSize;
		string tcpHeader;
		tcpHeader = cByte;
		strReqPack.insert(0, tcpHeader);
	}
	else
	{
		// TODO: Handle
	}
	/*string strBinarySize = Dec2Bin(nByteSize);
	if (strBinarySize.length() < 16)
		strBinarySize.insert(0, (16 - strBinarySize.length()), '0');

	strReqPack.insert(0, strBinarySize);*/
	_strReqPack = strReqPack;
}

void CCST415_Lab2Dlg::SynchronousSend_Receive()
{
	CString errorMsg;
	char* rspPack = new char[];

	// Send
	AddToWindowLog(L"Attempting to Send...");
	const char* cSend = (NULL + _strReqPack.c_str());
	int nError = send(_connectSocket, cSend, _strReqPack.length(), NULL);
	if (nError == SOCKET_ERROR)
	{
		errorMsg.Format(L"send failed with error: %d", WSAGetLastError());
		AddToWindowLog(errorMsg);
	}
	else
	{
		errorMsg.Format(L"send succeeded in sending %d characters", nError);
		AddToWindowLog(errorMsg);
		AddToInstructorLog(_strReqPack);
	}

	// Receive
	AddToWindowLog(L"Attempting to Receive...");
	int nRspSize = recv(_connectSocket, rspPack, sizeof(rspPack), NULL);
	if (nRspSize == SOCKET_ERROR)
	{
		errorMsg.Format(L"recv failed with error: %d", WSAGetLastError());
		AddToWindowLog(errorMsg);
	}
	else
	{
		errorMsg.Format(L"recv successfully received %d characters from host", nRspSize);
		AddToWindowLog(errorMsg);
		// TODO: Parse char* rspPack
		// TODO: Add _strRspPack to the instructor Log file
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
		remove("Lab2.Scenario1.HartwellJ.txt");
		Do100Transactions();
	}
}

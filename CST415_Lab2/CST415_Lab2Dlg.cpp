
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
	, _connectSocket(INVALID_SOCKET)
	, _nClientPort(0)
	, _nCurrentId(-1)
	, _bLatent(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_asyncClient = new CAsyncClient();
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
	m_asyncClient->Initialize(this);

	_reqPacket.MessageType = "REQ";
	_reqPacket.StudentName = "HartwellJ";
	_reqPacket.StudentID = "19-5193";
	_reqPacket.ResponseDelay = 0;
	_reqPacket.ForeignHostIPAddress = SERVER_IP_STRING;
	_reqPacket.ForeignHostServicePort = SERVICE_PORT_NUM;
	_reqPacket.StudentData = "blah";
	//_reqPacket.ScenarioNo = 1;		// Scenario 1
	//_reqPacket.ScenarioNo = 2;		// Scenario 2
	_reqPacket.ScenarioNo = 3;

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

	int nCount = m_lstLog.GetCount();
	m_lstLog.SetCurSel(nCount - 1);

	UpdateWindow();
}

void CCST415_Lab2Dlg::AddToInstructorLog(string strItem)
{
	//_instructorLog.open("Lab2.Scenario1.HartwellJ.txt", fstream::out | fstream::app);
	//_instructorLog.open("Lab2.Scenario2.HartwellJ.txt", fstream::out | fstream::app);
	_instructorLog.open("Lab2.Scenario3.HartwellJ.txt", fstream::out | fstream::app);
	strItem += "\r\n";
	_instructorLog << strItem;
	_instructorLog.close();
}

void CCST415_Lab2Dlg::AddTrailerToInstructorLog()
{
	string trailerLog = "";
	int nResponse;

	time_t currentTime;
	struct tm *localTime;

	time(&currentTime);
	localTime = localtime(&currentTime);

	if (localTime->tm_mon + 1 < 10)
	{
		trailerLog += "0";
		trailerLog += to_string(localTime->tm_mon + 1);
	}
	else
		trailerLog += to_string(localTime->tm_mon + 1);
	if (localTime->tm_mday < 10)
	{
		trailerLog += "0";
		trailerLog += to_string(localTime->tm_mday);
	}
	else
		trailerLog += to_string(localTime->tm_mday);
	trailerLog += to_string(localTime->tm_year + 1900);
	trailerLog += "|";
	if (localTime->tm_hour < 10)
	{
		trailerLog += "0";
		trailerLog += to_string(localTime->tm_hour);
	}
	else
		trailerLog += to_string(localTime->tm_hour);
	if (localTime->tm_min < 10)
	{
		trailerLog += "0";
		trailerLog += to_string(localTime->tm_min);
	}
	else
		trailerLog += to_string(localTime->tm_min);
	if (localTime->tm_sec < 10)
	{
		trailerLog += "0";
		trailerLog += to_string(localTime->tm_sec);
	}
	else
		trailerLog += to_string(localTime->tm_sec);
	trailerLog += "|";

	//nResponse = shutdown(_connectSocket, SD_RECEIVE);		// Scenario 1
	nResponse = m_asyncClient->ShutDown(CAsyncSocket::receives);
	//if (nResponse == ERROR_SUCCESS)		// Scenario 1
	if (nResponse == TRUE)
		trailerLog += "00000|";
	else
	{
		trailerLog += to_string(nResponse);
		trailerLog += "|";
	}

	//nResponse = shutdown(_connectSocket, SD_SEND);		// Scenario 1
	nResponse = m_asyncClient->ShutDown(CAsyncSocket::sends);
	//if (nResponse == ERROR_SUCCESS)		// Scenario 1
	if (nResponse == TRUE)
		trailerLog += "00000|";
	else
	{
		trailerLog += to_string(nResponse);
		trailerLog += "|";
	}

	nResponse = closesocket(_connectSocket);
	if (nResponse == ERROR_SUCCESS)
		trailerLog += "00000";
	else
	{
		trailerLog += to_string(nResponse);
	}

	AddToInstructorLog(trailerLog);
	AddToWindowLog(L"Done - Closing Socket Connection");
}

void CCST415_Lab2Dlg::AsyncReceive()
{
	CString errorMsg;
	char rspPack[512];

	int nRspSize = m_asyncClient->Receive(rspPack, 512, 0);
	if (nRspSize == INVALID_SOCKET)
	{
		errorMsg.Format(L"Async Receive failed with error: %d", WSAGetLastError());
		AddToWindowLog(errorMsg);
	}
	else
	{
		errorMsg.Format(L"Async Receive successfully received %d characters from host", nRspSize);
		AddToWindowLog(errorMsg);
		string strRspPack(rspPack, (nRspSize - 1));

		// ---------------------- Scenario 2 ----------------------
		// Check for multiple Rsp's
		while (count(strRspPack.begin(), strRspPack.end(), '|') > 12)
		{
			int nBars = count(strRspPack.begin(), strRspPack.end(), '|');
			int nSplitPos = strRspPack.find("Req|");
			if (nSplitPos == string::npos)
			{
				nSplitPos = strRspPack.find("ed |");
			}
			string strFirstRsp = strRspPack.substr(0, (nSplitPos + 4));

			//strFirstRsp += "1|";
			//AddToInstructorLog(strFirstRsp);

			string strFindId = (strFirstRsp.substr(strFirstRsp.find("|") + 12, 2));
			int nThisId = atoi(strFindId.c_str());

			if (nThisId == _nCurrentId || _allLatents[nThisId] == 0)
			{
				//AddToInstructorLog(_allReqPacks.at(nThisId));
				AddToInstructorLog(strFirstRsp += "1|");
				_allRspPacks[nThisId] = true;
			}
			else if (_allLatents[nThisId] == 1)
			{
				AddToInstructorLog(_allReqPacks.at(nThisId));
				AddToInstructorLog(strFirstRsp += "3|");
				_allRspPacks[nThisId] = true;
			}
			else if (_allLatents[nThisId] == 2)
			{
				AddToInstructorLog(strFirstRsp += "4|");
			}

			strRspPack = strRspPack.substr((nSplitPos + 5), (strRspPack.length() - (nSplitPos + 5)));
		}

		string strFindId = (strRspPack.substr(strRspPack.find("|") + 12, 2));
		int nThisId = atoi(strFindId.c_str());

		if (nThisId == _nCurrentId || _allLatents[nThisId] == 0)
		{
			//AddToInstructorLog(_allReqPacks.at(nThisId));
			AddToInstructorLog(strRspPack += "1|");
			_allRspPacks[nThisId] = true;

			/*if (_bLatent)
				AddToInstructorLog(strRspPack += "3|");
			else
			{
				AddToInstructorLog(_allReqPacks.at(nThisId));
				AddToInstructorLog(strRspPack += "1|");
				_allRspPacks.push_back(strRspPack);
			}*/
		}
		else if (_allLatents[nThisId] == 1)
		{
			AddToInstructorLog(_allReqPacks.at(nThisId));
			AddToInstructorLog(strRspPack += "3|");
			_allRspPacks[nThisId] = true;
		}
		else if (_allLatents[nThisId] == 2)
		{
			AddToInstructorLog(strRspPack += "4|");
		}

		/*	Scenario 2
		strFirstRsp += "1|";
		AddToInstructorLog(strRspPack);*/

		if (strRspPack.find("|99|") != string::npos)
			AddTrailerToInstructorLog();
	}
}

bool CCST415_Lab2Dlg::AttemptTCPConnection()
{
	if (AfxSocketInit() == FALSE)
	{
		AddToWindowLog(L"Sockets Could Not Be Initialized");
		return FALSE;
	}
	// -------------- SCENARIO 1 --------------

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

	if (m_asyncClient->Attach(_connectSocket) == FALSE)
	{
		AddToWindowLog(L"Unable to attach socket to CAsyncSocket!");
		WSACleanup();
		return false;
	}

	AddToWindowLog(L"Successfully connected");

	/*CString errorMsg;

	m_asyncClient->Create();
	if (!m_asyncClient->Connect(SERVER_IP_LSTRING, SERVICE_PORT_NUM))
	{
		if (GetLastError() == WSAEWOULDBLOCK)
			Sleep(500);
		else 
		{
			errorMsg.Format(L"Unable to connect to server w/ error: %d", GetLastError());
			AddToWindowLog(errorMsg);
			return false;
		}
	}

	AddToWindowLog(L"Successfully connected");*/

	// Get Client IP
	/*char szHostName[255];
	gethostname(szHostName, 255);
	struct hostent *host_entry;
	host_entry = gethostbyname(szHostName);
	_strClientIp = inet_ntoa(*(struct in_addr *)*host_entry->h_addr_list);*/
	_strClientIp = "10.1.20.4";

	// Get Client Port
	struct sockaddr_in sin;
	int addrlen = sizeof(sin);
	//getsockname(_connectSocket, (struct sockaddr *)&sin, &addrlen);		// Scenario 1
	getsockname(m_asyncClient->m_hSocket, (struct sockaddr *)&sin, &addrlen);
	_nClientPort = ntohs(sin.sin_port);

	return true;
}

UINT CCST415_Lab2Dlg::TrackLatancy(LPVOID pParam)
{
	CCST415_Lab2Dlg *theDlg = (CCST415_Lab2Dlg*)pParam;
	int nThisId = theDlg->_nCurrentId;

	//theDlg->_bLatent = false;
	theDlg->_allLatents[nThisId] = 0;
	Sleep(3000);
	if (!theDlg->_allRspPacks[nThisId])
	{
		theDlg->DoStandInRsp(nThisId);
		//theDlg->_bLatent = true;
		theDlg->_allLatents[nThisId] = 1;
		Sleep(20000);
		//theDlg->_bLatent = false;
		theDlg->_allLatents[nThisId] = 2;
	}

	return 0;
}

UINT CCST415_Lab2Dlg::Do100Transactions( LPVOID pParam )
{
	int nReqSize;
	CString strLog;

	CCST415_Lab2Dlg *theDlg = (CCST415_Lab2Dlg*)pParam;

	//_reqPacket.ClientSocketNo = (int)_connectSocket;		// Scenario 1
	theDlg->_reqPacket.ClientSocketNo = (int)theDlg->m_asyncClient->m_hSocket;
	theDlg->_reqPacket.ClientIPAddress = theDlg->_strClientIp;
	theDlg->_reqPacket.ClientServicePort = theDlg->_nClientPort;

	for (int i = 0; i < 100; i++)
	{
		if (i == 3 || i == 5 || i == 8)
			theDlg->_reqPacket.ResponseDelay = 4000;
		else
			theDlg->_reqPacket.ResponseDelay = 0;
		theDlg->_reqPacket.RequestID = to_string(i);
		theDlg->_reqPacket.msTimeStamp = (GetTickCount64());

		theDlg->ConstructReqPackStr();
		//SynchronousSend_Receive();		// Scenario 1
		theDlg->AddToWindowLog(L"Attempting to Send...");
		nReqSize = theDlg->m_asyncClient->Send(theDlg->_strReqPack.c_str(), theDlg->_strReqPack.length(), 0);
		if (nReqSize == INVALID_SOCKET)
		{
			theDlg->AddToWindowLog(L"Async Send Failed");
			return -1;
		}

		strLog.Format(L"Async Send succeeded in sending %d characters to host", nReqSize);
		theDlg->AddToWindowLog(strLog);
		theDlg->_nCurrentId++;
		theDlg->_allReqPacks.push_back(theDlg->_strReqPack);
		AfxBeginThread(TrackLatancy, pParam);
		theDlg->AddToInstructorLog(theDlg->_strReqPack);

		Sleep(100);
	}

	return 0;
}

void CCST415_Lab2Dlg::DoStandInRsp(int id)
{
	_strRspPack = ("RSP|" + to_string(GetTickCount64()) + "|" + to_string(id) + "|HartweJ|19-5193|4000|" + _reqPacket.ClientIPAddress + "|" 
		+ to_string(_reqPacket.ClientServicePort) + "|" + to_string(_reqPacket.ClientSocketNo) + "|192.168.101.210|2605|MY-STAND-IN|2|");
	Byte headerBytes[2] = { '\0', (Byte)_strRspPack.size() };
	_strRspPack.insert(0, (const char*)headerBytes, 2);

	AddToInstructorLog(_allReqPacks.at(id));
	AddToInstructorLog(_strRspPack);
	_allRspPacks[id] = true;
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

	Byte headerBytes[2] = { '\0', (Byte)strReqPack.size() };
	strReqPack.insert(0, (const char*)headerBytes, 2);

	_strReqPack = strReqPack;
}

void CCST415_Lab2Dlg::SynchronousSend_Receive()
{
	CString errorMsg;
	char rspPack[255];

	// Send
	AddToWindowLog(L"Attempting to Send...");
	int nError = send(_connectSocket, _strReqPack.c_str(), _strReqPack.length(), NULL);
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
	int nRspSize = recv(_connectSocket, rspPack, 255, NULL);
	if (nRspSize == SOCKET_ERROR)
	{
		errorMsg.Format(L"recv failed with error: %d", WSAGetLastError());
		AddToWindowLog(errorMsg);
	}
	else
	{
		errorMsg.Format(L"recv successfully received %d characters from host", nRspSize);
		AddToWindowLog(errorMsg);
		string strRspPack(rspPack, (nRspSize-1));
		strRspPack += "1|";
		AddToInstructorLog(strRspPack);
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
	m_lstLog.ResetContent();
	_bLatent = false;
	_nCurrentId = -1;

	for (int i = 0; i < 100; i++)
	{
		_allRspPacks[i] = false;
		_allLatents[i] = 0;
	}

	if (AttemptTCPConnection())
	{
		//remove("Lab2.Scenario1.HartwellJ.txt");		// Scenario 1
		//remove("Lab2.Scenario2.HartwellJ.txt");		// Scenario 2
		remove("Lab2.Scenario3.HartwellJ.txt");
		//Do100Transactions();		// Scenario 1
		AfxBeginThread(Do100Transactions, this);
	}
}

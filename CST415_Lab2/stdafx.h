
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#include <string>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes

#include <afxsock.h>

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars

using namespace std;

#define MAX_PACKET_LENGTH 146

#define SERVER_IP_STRING		"192.168.101.210"
#define SERVICE_PORT_STRING		"2605"
#define SERVICE_PORT_NUM		2605

typedef unsigned char Byte;

struct RequestMessagePacket
{
	string MessageType;
	long msTimeStamp;
	string RequestID;
	string StudentName;
	string StudentID;
	int ResponseDelay;
	string ClientIPAddress;
	int ClientServicePort;
	int ClientSocketNo;
	string ForeignHostIPAddress;
	int ForeignHostServicePort;
	string StudentData;
	short ScenarioNo;
};

struct ResponseMessagePacket
{
	string MessageType;
	long msTimeStamp;
	string RequestID;
	string StudentName;
	string StudentID;
	int ResponseDelay;
	string ForeignHostIPAddress;
	int ForeignHostServicePort;
	int ServerSocketNo;
	string ServerIPAddress;
	int ServerServicePort;
	string ResponseID;
	short ResponseType;
};




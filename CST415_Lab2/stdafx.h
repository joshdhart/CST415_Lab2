
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

using namespace std;

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

#define MAX_PACKET_LENGTH 146

#define SERVER_IP_STRING "192.168.101.210"
#define SERVICE_PORT_NUM 2605

struct RequestMessagePacket
{
	short TCPHeader;
	string MessageType;
	short msTimeStamp;
	string RequestID;
	string StudentName;
	string StudentID;
	short ResponseDelay;
	string ClientIPAddress;
	short ClientServicePort;
	short ClientSocketNo;
	string ForeignHostIPAddress;
	short ForeignHostServicePort;
	string StudentData;
	short ScenarioNo;
};

struct ResponseMessagePacket
{
	short TCPHeader;
	string MessageType;
	short msTimeStamp;
	string RequestID;
	string StudentName;
	string StudentID;
	short ResponseDelay;
	string ForeignHostIPAddress;
	short ForeignHostServicePort;
	short ServerSocketNo;
	string ServerIPAddress;
	short ServerServicePort;
	string ResponseID;
	short ResponseType;
};




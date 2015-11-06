#include "stdafx.h"

#include "AsyncClient.h"
#include "CST415_Lab2Dlg.h"


CAsyncClient::CAsyncClient()
{
	m_dlgHandle = nullptr;
}

CAsyncClient::~CAsyncClient()
{
	delete m_dlgHandle;
}

void CAsyncClient::Initialize(CCST415_Lab2Dlg* dlgHandle)
{
	m_dlgHandle = dlgHandle;
}


void CAsyncClient::OnReceive(int nErrorCode)
{
	m_dlgHandle->AsyncReceive();
	CAsyncSocket::OnReceive(nErrorCode);
}

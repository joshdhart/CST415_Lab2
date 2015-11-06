#pragma once

class CCST415_Lab2Dlg;

class CAsyncClient: public CAsyncSocket
{
public:
	CAsyncClient();
	~CAsyncClient();

	void Initialize(CCST415_Lab2Dlg* dlgHandle);

	void OnReceive(int nErrorCode);

private:
	CCST415_Lab2Dlg* m_dlgHandle;
};


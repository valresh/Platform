#include "stdafx.h"
#include "FlexRsuDebugBridge.h"
#include "DebugApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif*/


// The one and only application object

KDebugApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
	}

	return nRetCode;
}

void ShowDebugWnd( LPCSTR pszPrjName, HWND hMainWnd, SPrjCfg *pConfig, bool bExitOnClose )
{
  theApp.Open( pszPrjName, hMainWnd, pConfig, bExitOnClose );
}

void CloseDebugWnds()
{
  theApp.CloseAll();
}

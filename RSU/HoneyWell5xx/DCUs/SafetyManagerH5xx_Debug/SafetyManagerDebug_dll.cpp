#include "stdafx.h"
#include "SafetyManagerApp.h"
#include "SafetyManagerDebug_dll.h"

KSafetyManagerApp theApp;

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

void ShowDebugWindow( LPCSTR pszPrj, IFscStorage *pOwner )
{
  theApp.Open( pszPrj, pOwner );
}

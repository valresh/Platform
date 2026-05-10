#pragma once



// KDebugApp

class KDebugApp : public CWinApp
{
	DECLARE_DYNCREATE(KDebugApp)

public:
	KDebugApp();           // protected constructor used by dynamic creation
	virtual ~KDebugApp();

  void Open( LPCSTR pszPrjName, HWND hMainWnd, SPrjCfg *pConfig, bool bExitOnClose );
  void CloseAll();
  void CloseAllWPF();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
  BOOL m_bDebugMode;
protected:
  HMODULE m_hGui;
  CDocManager* m_pDocManager;

	DECLARE_MESSAGE_MAP()
};



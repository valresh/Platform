#pragma once
#include <map>
#include <string>
#include <Windows.h>
#include "IModelHandler.h"

class rsuServer
{
	enum
	{
		MAX_LOADSTRING = 100,
		cmdExit		= 102,
		//WM_COMMAND	= 0x0111,
		//WM_COPYDATA = 0x004A,
	};
public:
	bool Initialize();
	void Stop();
	//int Work();
protected:
	void MyRegisterClass();
protected:
	static __int64 s_WndProcTCP(void* context, unsigned int msg, unsigned __int64 wParam, __int64 lParam, __int64 InnerHWnd);
	__int64 WndProcImpl(void* hWnd, unsigned int msg, unsigned __int64 wParam, __int64 lParam);
protected:
	void* OnCopyData(unsigned __int64 wParam, __int64 lParam);
protected:
	void* OnInitialize(std::string szDll, std::string szModelName);
protected:
	//NOTIFYICONDATA m_NI;
	void* m_hServWnd;
	//HWND m_hExecWnd;
	//UINT m_nIconMessage;
	//HICON hIcon;
	//HINSTANCE hInst;								// current instance
	//char szTitle[MAX_LOADSTRING];
	//char szWindowClass[MAX_LOADSTRING];
protected:
	typedef std::map<std::string, tModelHandlerPtr_> tModelsCont_;
	typedef std::map<__int64, tModelHandlerPtr_> tModelsContI_;

	tModelsCont_ m_Servers;
	tModelsContI_ m_ServersI;
};


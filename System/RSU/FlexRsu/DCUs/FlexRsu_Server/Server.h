#pragma once
#include <rsuModel.h>
#include <FlexRsu.h>
#include "IFlexRsuModel.h"
#include <rsu_server.h>

class KServer
    : public IRsuModel
    , protected KFlexRsu
    , protected IFlexRsuModel
{
    bool m_bInited, m_bStep0, m_bStep1;
    int m_indexInPool;
    char m_szShowObject[256 * 4];
    char m_szBigName[64 * 4];
public:
    KServer(LPCSTR pszModelName);
#ifdef _WIN32
private:
    HWND m_hCmdWnd;
    static LRESULT WINAPI s_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static DWORD WINAPI s_WndThread(LPVOID lpParam);
    void WndThread();
    LRESULT WndProcImpl(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    HANDLE m_hWndThread;
#endif
public:
    int DoInitialize() { return Initialize(); };
    int DoAfterInit() { return AfterInit(); };
    int DoAfterRestored() { return AfterRestored(); };
    int DoCalcStep(int dtMs) { return CalcStep(dtMs); };
    int DoStateSave(LPCSTR pszPath) { return StateSave(pszPath); };
    int DoStateRestore(LPCSTR pszPath) { return StateRestore(pszPath); };
    int DoParamsSave(LPCSTR pszPath) { return ParamsSave(pszPath); };
    int DoParamsRestore(LPCSTR pszPath) { return ParamsRestore(pszPath); };
    void DoShowObject(LPCSTR pszName) {
#ifdef _WIN32
        ShowObject(pszName);
#endif
    };
protected:
    //IRsuModel
    virtual int Initialize();
    virtual int AfterInit();
    virtual int AfterRestored();
    virtual int CalcStep(int dtMs);
    virtual int StateSave(LPCSTR pszPath);
    virtual int StateRestore(LPCSTR pszPath);
    virtual int ParamsSave(LPCSTR pszPath);
    virtual int ParamsRestore(LPCSTR pszPath);
#ifdef _WIN32
    virtual void ShowObject(LPCSTR pszName);
#else
    virtual void ShowObject(LPCSTR pszName) {}
#endif
protected:
    //IFlexRsuModel
    virtual BYTE* GetVar(LPSTR pszPntName, eVarType* pType = NULL, tVarSizeType_* pVarSize = NULL)
    {
        return KFlexRsu::GetVar(pszPntName, pType, pVarSize);
    }
#ifdef _WIN32
    virtual bool GetFirstACSObject()
    {
        return false;
    }
    virtual bool GetNextACSObject(void** pData, char* Name, size_t sn, char* Data, size_t sd)
    {
        return false;
    }
    virtual bool ShowACSObject(void* pInfo, HWND hMainWnd, LPCSTR Name, void* pData);
#endif
};

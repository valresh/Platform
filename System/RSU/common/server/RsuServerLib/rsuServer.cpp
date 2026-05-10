#include "rsuServer.h"
#include <network/WinMessagesMQTT.h>
#include <rsuErr.h>
#include <rsuIPC.h>
#include "ModelHandler.h"


bool rsuServer::Initialize()
{
    //LoadString(hInst, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    //LoadString(hInst, IDC_RSUSERVER, szWindowClass, MAX_LOADSTRING);

    //HWND hWnd = ::FindWindow(szWindowClass, NULL);
    //if (::IsWindow(hWnd))
    //{
    //    return false;
    //}

    //m_nIconMessage = ::RegisterWindowMessage("RsuServerMSGs!!!");

    // Initialize global strings
    MyRegisterClass();

    //m_NI.cbSize = sizeof(m_NI);
    //m_NI.uID = 1001;
    //m_NI.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    //m_NI.uCallbackMessage = m_nIconMessage;
    //m_NI.hIcon = hIcon;
    //strcpy_s(m_NI.szTip, szTitle);

    //// Perform application initialization:
    //if (!InitInstance(hInst, 0/*nCmdShow*/))
    //{
    //    return false;
    //}

    ////hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RSUSERVER));

    return false;
}

void rsuServer::Stop()
{
    if (m_hServWnd)
        DestroyWindowMQTT(m_hServWnd);
}

void rsuServer::MyRegisterClass()
{
    m_hServWnd = RegisterClassExMQTT2("RsuServer", (WNDPROCMQTT2)s_WndProcTCP, (void*)this);
//    m_hServWnd = RegisterClassExTCP2("RsuServer", (WNDPROCTCP2)s_WndProcTCP, (void*)this, CommServerId);
}

__int64 rsuServer::s_WndProcTCP(void* context, unsigned int msg, unsigned __int64 wParam, __int64 lParam, __int64 InnerHWnd)
{
    rsuServer* p = reinterpret_cast<rsuServer*>(context);
    if (p)
    {
        if (InnerHWnd != 0)
        {
            auto it = p->m_ServersI.find(InnerHWnd);
            if (it == p->m_ServersI.end())
                return 0;
            return it->second->WndProcImpl((HWND)context, msg, wParam, lParam);
        }
        return p->WndProcImpl(context, msg, wParam, lParam);
    }
    return 0;
}

__int64 rsuServer::WndProcImpl(void* hWnd, unsigned int msg, unsigned __int64 wParam, __int64 lParam)
{
    switch (msg)
    {
    case WM_COPYDATA:
        return (__int64)OnCopyData(wParam, lParam);
    }
    return 0;
}

void* rsuServer::OnCopyData(unsigned __int64 wParam, __int64 lParam)
{
    MCOPYDATASTRUCT& cds = *(MCOPYDATASTRUCT*)lParam;
    SCopyDataHeader& header = *(SCopyDataHeader*)cds.lpData;
    switch (header.type)
    {
    case epkInitialize:
    {
        SInitialModel& init = *(SInitialModel*)cds.lpData;
        return OnInitialize(init.szImplementation, init.szName);
    }
    break;
    }
    return 0;
}

void* rsuServer::OnInitialize(std::string szDll, std::string szModelName)
{
    tModelsCont_::iterator it = m_Servers.find(szModelName);
    if (m_Servers.end() != it)
        return it->second->hCmdWnd;

    KModelHandler* model = new KModelHandler();
    int r = model->Init(0, szDll.c_str(), szModelName.c_str());
    if (r)
    {
        delete model;
        return nullptr;
    }
    m_Servers[szModelName] = model;
#ifndef _OLD
    model->hCmdWnd = (HWND)m_Servers.size();
    m_ServersI[m_Servers.size()] = model;
#endif
    return model->hCmdWnd;
}

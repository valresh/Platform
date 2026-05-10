#pragma once
#include <vector>
#include <functional>
#include "UniBuffer.h"
#include "Common.h"
#include "shemedata.h"
#include "SysDataTypes.h"
#include <Sterver.h>
#include "managed.h"

class CLIENTDLL_API CPipeClient : public CSterver
{
private:
    char m_szActObj[_MAX_PATH];
    managed::INetworkHub& _networkHub;
    std::string _listId;

public:
    CPipeClient(managed::INetworkHub& networkHub);
    virtual ~CPipeClient();

    // Запросы у сервера
    bool  GetAlarms(DWORD rsuId, int nBegin, int nSize, int nParam, void* data, UINT dwSize);//qGetAlarms
    bool  GetEvents(DWORD rsuId, int nBegin, int nSize, int nParam, void* data, UINT dwSize);//qGetEvents
    bool  GetMsgToO(DWORD rsuId, int nBegin, int nSize, int nParam, void* data, UINT dwSize);//qGetMsgToO
    bool  GetData(UINT type, LPCTSTR name, CShBase*);//qGet
    bool  GetData(int nNumber, CShBase*);//qGet
    CShBase* GetBase(int nNumber);//qGet
    bool  SendData(UINT nNumber, struct SSendToModel& send, bool bMessage = true);//qSend
    bool  SendData(struct SSendToModel& send, bool bMessage);//qSend
    void  ChangeBusy(UINT type, LPCTSTR name, BOOL);//qIncBusy,qDecBusy
    bool  Refresh();//qRefreshList
    void  AlarmSil(DWORD dwRsuid = 0);//qUpdateSound
    void  ReGetData();//qGetList
    void  Begin();// Обнуление данных
    bool  End();//qGetList
    bool IsConnected();

    void Obj(const char* szObj)
    {
        strcpy_s(m_szActObj, szObj);
    }

    const char* Obj()
    {
        return m_szActObj;
    }

private:
    bool GetAED(
        DWORD rsuId,
        int nBegin,
        int nSize,
        int nParam,
        void* data,
        UINT dwSize,
        std::function<managed::Buffer(DWORD rsuId, const char* data, size_t size)> sendFunction);

};

//CLIENTDLL_API void  DrawBeko( LPDRAWITEMSTRUCT item );

//inline CShSystem& GiveSystem()
//{
//    return (CShSystem&)CPipeClient::ms_pMainPipe->SH(0);
//}
//
//inline bool SendPipe(SSendToModel& send, bool bMessage = true)
//{
//    return CPipeClient::ms_pMainPipe->SendData(send, bMessage);
//}
//
//CLIENTDLL_API bool IsNoPCY();
//CLIENTDLL_API bool IsWorkModel();
CLIENTDLL_API bool IsTrueModel();
CLIENTDLL_API __time64_t GiveModelTime();
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define IsTypeOK ( sh. RetCode    == CShBase::rcOK )
#define IsTypeOk ( sh->RetCode    == CShBase::rcOK )
#define ISTypeOK ( p->sh.RetCode  == CShBase::rcOK )
#define ISTypeOk ( p->sh->RetCode == CShBase::rcOK )
// Для АРМ-а
#define TypeSh pipe.GetMnemoType( m_nNumber )
#define BaseSh pipe.Sh( nNumber )
#define BaseSH pipe.SH( nNumber )
// Для QueueGDI
#define PipeSH q.m_pPipe->SH(p->nNumber)
#define PipeSh q.m_pPipe->Sh(p->nNumber)
//
#define ObjSh q.m_pPipe->Sh(nNumber)
#define IsTypeSH( A ) ( q.m_pPipe->GetMnemoType(p->nNumber) == A )
#define MainPipe CPipeClient::ms_pMainPipe
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

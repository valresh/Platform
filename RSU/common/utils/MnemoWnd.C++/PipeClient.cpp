//#include <io.h>
#include <stdexcept>
#include "PipeClient.h"
#include "LogFile.h"
#include "EnumQueryTypes.h"

class CMemBuffer : public SUniBuffer
{
public:
    CMemBuffer()
        : SUniBuffer(1)
    {
    }

    CMemBuffer(DWORD q)
        : SUniBuffer(1)
    {
        Write(&q, sizeof(q));
    }

    void Write(void* v, int s)
    {
        AddObj(v, s);
    }
    BYTE* GetBuffer()
    {
        return (BYTE*)m_szBuffer;
    };
};

CPipeClient::CPipeClient(managed::INetworkHub& networkHub)
    :_networkHub(networkHub)
{
}

CPipeClient::~CPipeClient()
{
    if (!_listId.empty())
    {
        _networkHub.ClearList(_listId.c_str());
    }
}

bool CPipeClient::IsConnected()
{
    return _networkHub.IsConnected();
}

bool CPipeClient::Refresh()
{
    if (!IsConnected())
    {
        return false;
    }

    if (_listId.empty())
    {
        return End();
    }

    if (Count())
    {
        auto buffer = _networkHub.RefreshList(_listId.c_str(), managed::Buffer::Allocate);
        if (buffer.Length)
        {
            if (buffer.Length <= Size())
            {
                memcpy(Data(), buffer.Data, buffer.Length);
            }
        }
        else
        {
            buffer.Release();
            return End();
        }

        buffer.Release();
    }

    return true;
}

bool CPipeClient::SendData(SSendToModel& send, bool bMessage)//qSend
{
    send.bMessage = bMessage;
    CMemBuffer buf;
    buf.Write(&send, sizeof(SSendToModel));
    _networkHub.SendData((const char*)buf.GetBuffer(), buf.Count());
    return Refresh();
}
//
bool CPipeClient::SendData(UINT nNumber, struct SSendToModel& send, bool bMessage)
{
    if (nNumber >= Count())
        return false;
    send.eData = (EDataTypes)GetMnemoType(nNumber);
    lstrcpy(send.szName, GetModelName(nNumber));
    return SendData(send, bMessage);
}
//
void CPipeClient::AlarmSil(DWORD dwRsuid)
{
    CMemBuffer buf(qUpdateSound);
    _networkHub.UpdateSound(dwRsuid, (const char*)buf.GetBuffer(), buf.Count());
}
//
bool CPipeClient::GetData(UINT type, LPCTSTR name, CShBase* pB)
{
    CMemBuffer buf;
    buf.Write(&type, sizeof(type));
    buf.Write((void*)name, lstrlen(name) + 1);
    auto buffer = _networkHub.GetData(_listId.c_str(), (const char*)buf.GetBuffer(), buf.Count(), managed::Buffer::Allocate);
    if (buffer.Length != UINT(pB->m_nSize))
    {
        return false;
    }

    memcpy(pB, buffer.Data, buffer.Length);
    buffer.Release();
    return true;
}
//
bool CPipeClient::GetData(int nNumber, CShBase* sh)
{
    int nTotal = Count();
    sh->RetCode = sh->rcNO;
    if (UINT(nNumber) >= Count())
    {
        return false;
    }

    GetData(GetMnemoType(nNumber), GetModelName(nNumber), sh);
    return IsTypeOk;
}
//
CShBase* CPipeClient::GetBase(int nNumber)
{
    CShBase* sh = Base(nNumber);
    if (sh == NULL) return NULL;
    if (GetData(nNumber, sh))
    {
        return sh;
    }

    return NULL;
}

bool CPipeClient::GetAED(
    DWORD rsuId, 
    int nBegin, 
    int nSize, 
    int nParam, 
    void* data, 
    UINT dwSize, 
    std::function<managed::Buffer(DWORD rsuId, const char* data, size_t size)> sendFunction)
{
    CMemBuffer buf(rsuId);
    buf.Write(&nBegin, sizeof(nBegin));
    buf.Write(&nSize, sizeof(nSize));
    buf.Write(&nParam, sizeof(nParam));
    auto response = sendFunction(rsuId, (const char*)buf.GetBuffer(), buf.Count());
    if (response.Length != dwSize)
    {
        return false;
    }

    memcpy(data, response.Data, dwSize);
    response.Release();
    return true;
}

bool CPipeClient::GetAlarms(DWORD rsuId, int nBegin, int nSize, int nParam, void* data, UINT dwSize)
{
    return GetAED(rsuId, nBegin, nSize, nParam, data, dwSize, [&](DWORD rsuId, const char* data, size_t size)
        {
            return _networkHub.GetAlarms(rsuId, data, size, managed::Buffer::Allocate);
        });
}

bool CPipeClient::GetEvents(DWORD rsuId, int nBegin, int nSize, int nParam, void* data, UINT dwSize)
{
    return GetAED(rsuId, nBegin, nSize, nParam, data, dwSize, [&](DWORD rsuId, const char* data, size_t size)
        {
            return _networkHub.GetEvents(rsuId, data, size, managed::Buffer::Allocate);
        });
}

bool CPipeClient::GetMsgToO(DWORD rsuId, int nBegin, int nSize, int nParam, void* data, UINT dwSize)
{
    return GetAED(rsuId, nBegin, nSize, nParam, data, dwSize, [&](DWORD rsuId, const char* data, size_t size)
        {
            return _networkHub.GetMsgToOperator(rsuId, data, size, managed::Buffer::Allocate);
        });
}

void CPipeClient::ChangeBusy(UINT type, LPCTSTR name, BOOL bInc)
{
    CMemBuffer buf; // (bInc ? qIncBusy : qDecBusy);
    buf.Write((void*)&type, sizeof(type));
    buf.Write((void*)name, lstrlen(name) + 1);
    if (bInc)
    {
        _networkHub.IncrementBusy((const char*)buf.GetBuffer(), buf.Count());
    }
    else
    {
        _networkHub.DecrementBusy((const char*)buf.GetBuffer(), buf.Count());
    }
}
//

bool CPipeClient::End()
{
    if (!IsConnected())
    {
        return false;
    }

    UINT nCount = Count();
    if (nCount == 0)
    {
        return true;
    }

    CMemBuffer buf;
    SSterver* obj = Item();
    for (UINT i = 0; i < nCount; i++)
    {
        buf.Write(&(obj[i].nType), sizeof(UINT)); //first write type
        buf.Write(obj[i].szServ + 1, obj[i].szServ[0]); //then write name
    }

    auto listInfo = _networkHub.SetupList((const char *)buf.GetBuffer(), buf.Count(), _listId.c_str(), managed::Buffer::Allocate);
    if (listInfo.Buffer.Length)
    {
        _listId = listInfo.Id;
        if (listInfo.Buffer.Length <= Size())
        {
            memcpy(Data(), listInfo.Buffer.Data, listInfo.Buffer.Length);
        }
        else
        {
            UINT mySize = Size();
            mData.Realloc(listInfo.Buffer.Length);
            mData.AddCurr(listInfo.Buffer.Length - mySize);
            memcpy(Data(), listInfo.Buffer.Data, listInfo.Buffer.Length);
            UINT shiftSh = 0;
            for (int a = 0, C(Count()); a < C; ++a)
            {
                CShBase* pShCur = (CShBase*)mData.Obj(shiftSh);
                SSterver& st = GetAt(a);
                st.nShift = shiftSh;
                shiftSh += pShCur->m_nSize;
            }
        }

        listInfo.Buffer.Release();
        return true;
    }

    return false;
}

void CPipeClient::Begin()
{
    m_nCurrent = 0, mData.m_nCurrent = 0;
    _listId = "";
}

#include <numeric>
#include "CommandHandler.h"
#include "network/Commands.h"
#include "Names.h"
#include "Serializer.h"
#include "ServerThreadBase.h"
#include "LogFile.h"
#include "macros/StrHelps.h"
#include "TrendFile.h"
#include "SMessage.h"
#include "MessageList.h"
#include "EventEntry.h"
#include "crossplatform.h"

using namespace network;

namespace serverexbase
{
    static CTrendFile theTrend;

    static BYTE* Realloc(BYTE* b, int nNew, int& nOld)
    {
        if (nNew <= nOld)
        {
            return b;
        }

        BYTE* c = (BYTE*)malloc(nNew);
        nOld = nNew;
        return c;
    }

    static int SizeSh(DWORD type, KRsuX* pPnt = nullptr)
    {
        switch (type)
        {
        case id_Forsunka: return sizeof(CShForsunka);
        case id_Sensor: return sizeof(CShSensor);
        case id_KVO: return sizeof(CShKVO);
        case id_Klapan: return sizeof(CShKlapan);
        case id_Zadv: return sizeof(CShZadv);
        case id_HS: return sizeof(CShHS);
        case id_PHS: return sizeof(CShPolyHS);
        case id_Pump: return sizeof(CShPump);
        case id_GasAnalyz: return sizeof(CShGasAnalyz);
        case id_System: return sizeof(CShSystem);
        case id_Value: return sizeof(CShValue);
        case id_Edg: return sizeof(CShEdg);
        }

        for (int i = 0; i < _countof(g_extensionsCont); i++)
        {
            if (!g_extensionsCont[i].pSizeShRSUCall)
            {
                continue;
            }

            int size = g_extensionsCont[i].pSizeShRSUCall(type, pPnt);
            if (size > 0)
            {
                return size;
            }

        }

       // ASS(0);
        return 0;
    }

    static void Extractum(DWORD type, CShBase* pSB, CBase* pB) //pB ->  pSB
    {
        for (int i = 0; i < _countof(g_extensionsCont); i++)
        {
            if (g_extensionsCont[i].pExtractumRSUCall &&
                g_extensionsCont[i].pExtractumRSUCall(type, pSB, pB) > 0)
            {
                return;
            }
        }

        pSB->m_nSize = SizeSh(type, NULL);
        switch (type)
        {
        case id_Forsunka: CShForsunka::GetModelData(pB, pSB); break;
        case id_Sensor: CShSensor::GetModelData(pB, pSB); break;
        case id_KVO: CShKVO::GetModelData(pB, pSB); break;
        case id_Klapan: CShKlapan::GetModelData(pB, pSB); break;
        case id_Zadv: CShZadv::GetModelData(pB, pSB); break;
        case id_HS: CShHS::GetModelData(pB, pSB); break;
        case id_PHS: CShPolyHS::GetModelData(pB, pSB); break;
        case id_Pump: CShPump::GetModelData(pB, pSB); break;
        case id_GasAnalyz: CShGasAnalyz::GetModelData(pB, pSB); break;
        case id_System: CShSystem::GetModelData(pB, pSB); break;
        case id_Value: CShValue::GetModelData(pB, pSB); break;
        case id_Edg:	CShEdg::GetModelData(pB, pSB); break;
        default: pSB->m_nSize = 0; ASS(0); return;
        }

        pSB->RetCode = CShBase::rcOK;
    }

    static void Extractum(DWORD type, CShBase* pSB, KRsuX& pnt) //pB ->  pSB
    {
        if (!pSB)
        {
            return;
        }

        int nSize = SizeSh(type, &pnt);
        memset(pSB, 0, nSize);
        pSB->m_nSize = nSize;
        pSB->RetCode = CShBase::rcNO;
        if (!pnt.IsConnected())
        {
            return;
        }

        for (int i = 0; i < _countof(g_extensionsCont); i++)
        {
            if (!g_extensionsCont[i].pGetRsuData)
            {
                continue;
            }

            int r = g_extensionsCont[i].pGetRsuData(type, pSB, pnt);
            if (r != CShBase::rcNO)
            {
                break;
            }
        }
    }

    static CBase* FindConnPointLocal(LPCSTR ObjName, DWORD TypeID)
    {
        CBase* pB = NULL;
        for (int i = 0; i < _countof(g_extensionsCont); i++)
        {
            if (!g_extensionsCont[i].pFindConnPointEx)
            {
                continue;
            }

            pB = g_extensionsCont[i].pFindConnPointEx(ObjName, TypeID);
            if (pB)
            {
                break;
            }
        }

        if (!pB)
        {
            pB = ::FindConnPoint(ObjName, TypeID, -1);
        }

        return pB;
    }

    static CBase* FindBase(DWORD TypeID, char* szName, CShBase* pSB, KRsuX& pnt)
    {
        CBase* pB = FindConnPointLocal(szName, TypeID);
        int r = pnt.LinkTo(szName, TypeID);
        if (!r)
        {
            if (pnt.IsPriorOnCBase())
            {
                pB = NULL;
            }
        }

        if (pSB)
        {
            if (pB != NULL)
            {
                Extractum(TypeID, pSB, pB);
            }
            else
            {
                Extractum(TypeID, pSB, pnt);
            }
        }

        return pB;
    }

    static void ChangeBusy(CBase* pB, int n)
    {
        if (pB != NULL)
        {
            CBusy* pBusy = (CBusy*)pB;
            pBusy->Busy += n;
            if (pBusy->Busy < 0)
            {
                pBusy->Busy = 0;
            }
        }
    }

    static SMessage* Mess(SSendToModel& send)
    {
        static CMessageList thisMess;
        SMessage fnd;
        fnd.eData = send.eData;
        StrCPY(fnd.szName, send.szValue, sizeof(fnd.szName));
        return (SMessage*)bsearch(&fnd, thisMess.m_list, thisMess.m_nCount, sizeof(SMessage), SortMessage);
    }

    static void PutModelLocal(LPCTSTR szUser, SSendToModel& send)
    {
#ifdef _WIN32
        OutputDebugString("попытка установить значение дл¤ \"");
        OutputDebugString(send.szName);
        OutputDebugString("\" в \"");
        OutputDebugString(send.szValue);
        OutputDebugString("\"\n");
#endif
        CEventString str;
        bool bMessage = send.bMessage;
        DWORD dwMess = CEventLogType::etUnknown;
        BOOL bExtPutter = FALSE;

        // ѕростое текстовое сообщение
        if (send.eData == id_Msg)
        {
            str.Event(send.szName);
            dwMess = (DWORD)send.eType;
            bExtPutter = TRUE;
        }
        else
        {
            for (int i = 0; i < _countof(g_extensionsCont); i++)
            {
                if (!g_extensionsCont[i].pPutModelRSUCall)
                {
                    continue;
                }

                DWORD r = g_extensionsCont[i].pPutModelRSUCall(szUser, send, str);
                if (r != CEventLogType::etUnknown && r != CEventLogType::etError)
                {
#ifdef _WIN32
                    if (!str.IsEmpty())
                    {
                        OutputDebugString(str.GetBuffer()), OutputDebugString("\n");
                    }
#endif
                    dwMess = r;
                    bExtPutter = TRUE;
                    break;
                }
            }
        }
        if (!bExtPutter)
        {
            CBase* pBase = FindConnPointLocal(send.szName, send.eData);
            if (pBase == NULL)
            {
                return;
            }

            SMessage* mess = Mess(send);
            dwMess = CEventLogType::etError;
            if (mess != NULL)
            {
                if (mess->PutMdl(send, pBase, str))
                {
                    dwMess = mess->def->dwLog;
                }
            }
            else if (send.bMessage)
            {
                static const char* szFiltr = "Не найдено сообщение '%s'.'%s'";
                str.Event(szFiltr, send.szName, send.szValue);
            }
        }

        if (bMessage && !str.IsEmpty() && dwMess != CEventLogType::etSameValue)
        {
            CEventEntry entry;
            SSendToModel* Send = static_cast<SSendToModel*>(&entry);
            memcpy(Send, &send, sizeof(send));
            entry.nMess = dwMess;
            lstrcpyn(entry.szUser, szUser, sizeof(entry.szUser));
            lstrcpyn(entry.szText, str.GetBuffer(), sizeof(entry.szText));
            char* p = strchr(entry.szUser, '@');
            if (p)
            {
                *p = 0;
            }

            WriteEvents(entry);
        }
    }

    static void GetFromReq(const databuffer& request, char*& name, size_t& pos)
    {
        name = (char*)(request.data() + pos);
        CLogFile::Log("KServerThreadBase::GetFromReq_1: name %s", name);
        pos += strlen(name) + 1;
    }

    static void GetFromReq(const databuffer& request, DWORD& type, char*& name, size_t& pos)
    {
        type = *(DWORD*)(request.data() + pos);
        CLogFile::Log("KServerThreadBase::GetFromReq_0: type %d", type);
        pos += sizeof(type);
        GetFromReq(request, name, pos);
    }

    CommandHandler::CommandHandler()
    {
        ModelStatus = 127;
        _serializer = new internal::Serializer();
    }

    CommandHandler::~CommandHandler()
    {
        if (_serializer)
        {
            delete _serializer;
            _serializer = nullptr;
        }
    }

    network::Response CommandHandler::OnCommand(network::Commands command, const std::string& clientId, const network::Request& request)
    {
        switch (command)
        {
        case network::Commands::GetData:
        case network::Commands::GetDataEx:
        {
            auto getDataRequest = _serializer->GetData.Deserialize(request);
            auto list = GetList(getDataRequest.ListId);
            if (!list)
            {
                return Error("List not found");
            }

            return OnGetDataCommand(*list, getDataRequest.Data, command == network::Commands::GetDataEx);
        }

        case network::Commands::Send:
        {
            auto sendRequest = _serializer->Send.Deserialize(request);
            PutModelLocal(sendRequest.Username.c_str(), *(SSendToModel*)sendRequest.Data.data());
            return Success();
        }

        case network::Commands::IncBusy:
        case network::Commands::DecBusy:
        {
            auto busyRequest = _serializer->Busy.Deserialize(request);
            DWORD type = 0;
            char* chBuf = nullptr;
            size_t pos = 0;
            GetFromReq(busyRequest.Data, type, chBuf, pos);
            CBase* pB = FindConnPointLocal(chBuf, type);
            ChangeBusy(pB, 1 - 2 * ((int)command - (int)network::Commands::IncBusy));
            return Success();
        }

        case network::Commands::SetupList:
        case network::Commands::SetupListEx:
        {
            auto setupListRequest = _serializer->SetupList.Deserialize(request);
            auto list = GetList(setupListRequest.OverrideListId);
            if (!list)
            {
                list = AppendList();
            }

            auto response = OnSetupList(
                *list,
                command == network::Commands::SetupListEx,
                setupListRequest.Data);
            return response;

        }

        case network::Commands::RefreshList:
        case network::Commands::RefreshListEx:
        {
            auto refreshRequest = _serializer->RefreshList.Deserialize(request);
            auto list = GetList(refreshRequest.ListId);
            if (!list)
            {
                return Error("List not found");
            }

            auto response =  OnRefreshList(
                *list,
                command == network::Commands::RefreshListEx,
                [&](const databuffer& databuffer)
                {
                    return Success(databuffer);
                });
            return response;
        }

        case network::Commands::ClearList:
        {
            return OnClearList(clientId);
        }

        case network::Commands::GetAlarms:// Запрос данных из Alarms.dat
        {
            auto getAlarmsRequest = _serializer->GetAlarms.Deserialize(request);
            return OnGetAlarms(getAlarmsRequest.RsuId, getAlarmsRequest.Data);
        }

        case network::Commands::GetEvents:// Запрос данных из Events.dat
        {
            auto getEventsRequest = _serializer->GetEvents.Deserialize(request);
            return OnGetEvents(getEventsRequest.RsuId, getEventsRequest.Data);
        }

        case network::Commands::GetMsgToOperator:// Запрос данных из MsgToOperatorX.dat
        {
            auto getMsgToOperatorRequest = _serializer->GetMsgToOperator.Deserialize(request);
            return OnGetMsgToO(getMsgToOperatorRequest.RsuId, getMsgToOperatorRequest.Data);
        }

        case network::Commands::Trends:
        {
            auto trendsRequest = _serializer->Trends.Deserialize(request);
            return OnTrends(trendsRequest.Data);
        }

        case network::Commands::FindHoney:
        {
            auto findHoneyRequest = _serializer->FindHoney.Deserialize(request);
            return OnFindHoney(findHoneyRequest.RsuId, findHoneyRequest.Data);
        }

        case network::Commands::UpdateSound:
        {
            auto updateSoundRequest = _serializer->UpdateSound.Deserialize(request);
            return OnUpdateSound(updateSoundRequest.RsuId, updateSoundRequest.Data, updateSoundRequest.Username);
        }

        default:
            return network::Response::Empty();
        }
    }

    CommandHandler::List::List()
    {
        _id = std::to_string(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    }

    const std::string& CommandHandler::List::Id() const
    {
        return _id;
    }

    std::shared_ptr<CommandHandler::List> CommandHandler::AppendList()
    {
        std::lock_guard lock(_listsMutex);
        auto list = std::make_shared<List>();
        _lists[list->Id()] = list;
        return list;
    }

    std::shared_ptr<CommandHandler::List> CommandHandler::GetList(const std::string& listId)
    {
        std::lock_guard lock(_listsMutex);
        auto it = _lists.find(listId);
        if (it == _lists.end())
        {
            return nullptr;
        }

        return it->second;
    }

    static void FreeBufferIfNecessary(void* buffer, void* originalBuffer)
    {
        // Buffer was reallocated with Realloc
        if (buffer != originalBuffer)
        {
            free(buffer);
        }
    }

    network::Response CommandHandler::OnClearList(const std::string& sessionId)
    {
        std::lock_guard lock(_listsMutex);
        _lists.erase(sessionId);
        return Success();
    }

    network::Response CommandHandler::OnTrends(databuffer& request)
    {
        SToTrend* to = (SToTrend*)request.data();
        char chReturn = 0;
        try
        {
            DWORD dwOutSize = sizeof(SFromTrend5) + to->nCount * 2 * (to->nPoint + 1) * sizeof(_Sxy);
            network::Response response(dwOutSize, false);
            int nReturn = theTrend.FromTrend(to, (char*)response.data());
            if (nReturn != 0)
            {
                chReturn = -70 - nReturn;
                return network::Response(&chReturn, 1, false);
            }

            return response;
        }
        catch (std::bad_alloc&)
        {
            char chReturn = -70;
            return network::Response(&chReturn, 1, false);
        }
    }

    network::Response CommandHandler::OnFindHoney(DWORD rsuID, const databuffer& request)
    {
        DWORD type = 0;
        char* chBuf = nullptr;
        size_t pos = 0;
        GetFromReq(request, type, chBuf, pos);

        for (int i = 0; i < _countof(g_extensionsCont); i++)
        {
            if (!g_extensionsCont[i].pFindHoney)
            {
                continue;
            }

            BYTE* responseBuffer = (BYTE*)request.data();
            int responseBufferSize = request.size();
            DWORD size = g_extensionsCont[i].pFindHoney(rsuID, chBuf, type, &responseBuffer, responseBufferSize, Realloc);
            CLogFile::Log("KServerThreadBase::GetAnswerToRequest_qFindHoney_1: size %d, type %d, chBuf %s", size, type, chBuf);
            if (size > 0)
            {
                auto databuffer = databuffer::copy((char*)responseBuffer, responseBufferSize);
                FreeBufferIfNecessary(responseBuffer, request.data());
                return Success(databuffer);
            }
        }

        CLogFile::Log("KServerThreadBase::GetAnswerToRequest: NOT FOUND 1");
        LPSTR p = &chBuf[strlen(chBuf)];
        bool bF = false;
        databuffer  databuffer;
        while (p)
        {
            TStringTerminator st(p);
            const char* pszName;
            UINT ID = 0;
            CBase* pBase = NULL;
            while (WhileBase(ID, id_Unknown, &pBase, &pszName))
            {
                if (!strcmp(pszName, chBuf))
                {
                    databuffer.append((char*)&pBase->ID_CLASS, sizeof(pBase->ID_CLASS));
                    databuffer.append(pszName, strlen(pszName) + 1);
                    if (bF)
                        ++p, databuffer.append(p, strlen(p) + 1);
                    else
                        databuffer.append("", 1);
                    return Success(databuffer);
                }
            }

            if (!type)
                break;
            bF = true;
            p = strrchr(chBuf, '.');
        }

        CLogFile::Log("KServerThreadBase::GetAnswerToRequest: NOT FOUND 2");
        return Error("Not found");
    }

    network::Response CommandHandler::OnUpdateSound(DWORD rsuID, databuffer& request, const std::string& userName)
    {
        for (int i = 0; i < _countof(g_extensionsCont); i++)
        {
            if (!g_extensionsCont[i].pAnyOtherRequest)
                continue;

            BYTE* buffer = (BYTE*)request.data();
            int bufferSize = request.size();
            DWORD size = g_extensionsCont[i].pAnyOtherRequest(&buffer, bufferSize, rsuID, Realloc, userName.c_str());
            if (size > 0)
            {
                network::Response response((char*)buffer, size);
                FreeBufferIfNecessary(buffer, request.data());
                return response;
            }
        }

        return Error("Not found");
    }

    network::Response CommandHandler::OnGetDataCommand(List& list, const databuffer& request, bool isExtendedInfo)
    {
        DWORD type = 0;
        char* chBuf = nullptr;
        size_t pos = 0;
        GetFromReq(request, type, chBuf, pos);
        int nSize = 0;
        if (type)
        {
            nSize = SizeSh(type);
        }

        CShBase* pSB = list.mSh.Sh();
        list.mSh.Resize(nSize);
        pSB = list.mSh.Sh();
        SUniBuffer result(1);
        if (type)
        {
            KRsuX pnt;
            CBase* pB = FindBase(type, chBuf, pSB, pnt);
            if (pB)
            {
                Extractum(type, pSB, pB);
            }
            else
            {
                Extractum(type, pSB, pnt);
            }

            // Added because of the different architecture.
            // RSU can't get data for object if simulation not started yet
            if (pSB == nullptr)
            {
                // Returns empty databuffer
                return Success(databuffer());
            }

            // This block is added for TCP only (GetDataEx) to convert types utf8->win1251
            if (isExtendedInfo)
            {
                result.AddObj(&type, sizeof(type));
                result.AddObj(&pSB->m_nSize, sizeof(pSB->m_nSize));
            }

            result.AddObj((char*)pSB, pSB->m_nSize);
        }

        CLogFile::Log("KServerThreadBase::GetAnswerToRequest_qGet: pSB->m_nSize %d", pSB->m_nSize);
        return Success(databuffer(result.m_szBuffer, result.Count()));
    }

    network::Response CommandHandler::OnSetupList(List& list, bool isExtendedInfo, const databuffer& request)
    {
        size_t pos = 0;
        DWORD type;
        char* chBuf;
        int nSize = 0;
        list.DataEntries.clear();
        while (pos < request.size())
        {
            GetFromReq(request, type, chBuf, pos);
            EDataTypes eDataTypes = (EDataTypes)type;
            KRsuX pnt;
            CBase* pB = FindBase(eDataTypes, chBuf, NULL, pnt);
            if (!pB)
            {
                CLogFile::LogEx(LOGLEVEL_LOW, "MnemoGet_11: pB null");
            }
            else
            {
                CLogFile::LogEx(LOGLEVEL_LOW, "MnemoGet_12: connected %d, chBuf %s", pnt.IsConnected(), chBuf);
            }

            list.DataEntries.push_back(DataEntry(pB, chBuf, eDataTypes, pnt));
            nSize = std::max(nSize, SizeSh(type, &pnt));
        }

        list.mSh.Resize(nSize);
        CSystem* pSys = GetSystemData();
        if (pSys != NULL)
        {
            ModelStatus = pSys->StartStatus;
        }

        return OnRefreshList(
            list, 
            isExtendedInfo,
            [&](const databuffer& databuffer)
            {
                auto response = _serializer->SetupList.Serialize(internal::SetupListResponse(list.Id(), databuffer));
                return response;
            });
    }

    network::Response CommandHandler::OnRefreshList(
        List& list, 
        bool isExtendedInfo,
        std::function<network::Response(const databuffer&)> createResponse)
    {
        size_t nSize = list.DataEntries.size();
        CSystem* pSys = GetSystemData();
        if (pSys == NULL || (pSys->StartStatus == CSystem::STATUS_START && pSys->m_nStep < 3))
        {
            ModelStatus = 127;
            return Error("Invalid model status");
        }

        if (ModelStatus != pSys->StartStatus)
        {
            ModelStatus = pSys->StartStatus;
            for (size_t i = 0; i < nSize; i++)
            {
                list.DataEntries.at(i).pBase = FindBase(
                    list.DataEntries.at(i).mType,
                    list.DataEntries.at(i).name,
                    NULL,
                    list.DataEntries.at(i).pnt);
            }
        }

        CShBase* pSB = list.mSh.Sh();
        SUniBuffer result(1);
        try
        {
            for (size_t i = 0; i < nSize; i++)
            {
                DataEntry& DE = list.DataEntries.at(i);
                CBase* pB = list.DataEntries.at(i).pBase;
                EDataTypes eDataTypes = list.DataEntries.at(i).mType;
                if (pB == NULL)
                {
                    Extractum(eDataTypes, pSB, DE.pnt);
                }
                else
                {
                    Extractum(eDataTypes, pSB, pB);
                }

                // Added because of the different architecture.
                // RSU can't get data for object if simulation not started yet
                if (pSB == nullptr)
                {
                    // Returns empty databuffer
                    return createResponse(databuffer());
                }

                // This block is added for TCP only (commands SetupListEx/RefreshListEx) to convert types utf8->win1251
                if (isExtendedInfo)
                {
                    int32_t itemType = eDataTypes;
                    result.AddObj(&itemType, sizeof(itemType));
                    result.AddObj(&pSB->m_nSize, sizeof(pSB->m_nSize));
                }

                result.AddObj((char*)pSB, pSB->m_nSize);
            }

            databuffer databuffer(result.m_szBuffer, result.Count());
            return createResponse(databuffer);
        }
        catch (...)
        {
#ifdef _WIN32
            DWORD dwError = GetLastError();
#else
            DWORD dwError = errno;
#endif
            return Error("Error " + std::to_string(dwError));
        }
    }

    network::Response CommandHandler::OnGetAlarms(DWORD rsuID, databuffer& request)
    {
        for (int i = 0; i < _countof(g_extensionsCont); i++)
        {
            if (!g_extensionsCont[i].pAlarmsSupport)
            {
                continue;
            }

            BYTE* buffer = (BYTE*)request.data();
            int bufferSize = request.size();
            DWORD size = g_extensionsCont[i].pAlarmsSupport(&buffer, bufferSize, rsuID, Realloc);
            if (size > 0)
            {
                auto databuffer = databuffer::copy((char*)buffer, size);
                FreeBufferIfNecessary(buffer, request.data());
                return Success(databuffer);
            }
        }

        return Error("Not found");
    }

    network::Response CommandHandler::OnGetEvents(DWORD rsuID, databuffer& request)
    {
        if (0 != rsuID)
        {
            for (int i = 0; i < _countof(g_extensionsCont); i++)
            {
                if (!g_extensionsCont[i].pEventsSupport)
                {
                    continue;
                }

                BYTE* buffer = (BYTE*)request.data();
                int bufferSize = request.size();
                DWORD size = g_extensionsCont[i].pEventsSupport(&buffer, bufferSize, rsuID, Realloc);
                if (size > 0)
                {
                    auto databuffer = databuffer::copy((char*)buffer, size);
                    FreeBufferIfNecessary(buffer, request.data());
                    return Success(databuffer);
                }
            }

            return Error("Not found");
        }

        FILETIME fileCreation, fileLastWrite;
        std::ifstream hFile(::GetEventLogFilePath(), std::ios::binary);
        if (!hFile.is_open())
        {
            hFile.close();
            return Error("Can't open log file");
        }

        int* N = (int*)(request.data());

        // N[0] - начальный номер записи
        // N[1] - количество записей
        // N[2] - параметр ( здесь не используетс¤ )
        DWORD nReadBytes = N[1] * sizeof(CEventEntry);
        DWORD nBytesRead;
        databuffer databuffer(nReadBytes);
        BOOL bResult = hFile.read((char*)databuffer.data(), nReadBytes) ? true : false;
        nBytesRead = hFile.gcount();
        hFile.close();
        if (bResult && nReadBytes == nBytesRead)
        {
            return Success(databuffer);
        }

        return Error("Impossible to read from log file");
    }

    network::Response CommandHandler::OnGetMsgToO(DWORD rsuID, databuffer& request)
    {
        for (int i = 0; i < _countof(g_extensionsCont); i++)
        {
            if (!g_extensionsCont[i].pMessagesSupport)
            {
                continue;
            }

            BYTE* buffer = (BYTE*)request.data();
            int bufferSize = request.size();
            DWORD size = g_extensionsCont[i].pMessagesSupport(&buffer, bufferSize, rsuID, Realloc);
            if (size > 0)
            {
                auto databuffer = databuffer::copy((char*)buffer, size);
                FreeBufferIfNecessary(buffer, request.data());
                return Success(databuffer);
            }
        }

        return Error("Not found");
    }

    network::Response CommandHandler::Success(const databuffer& databuffer) const
    {
        return _serializer->Serialize(internal::MixedResponse(databuffer));
    }

    network::Response CommandHandler::Success() const
    {
        return _serializer->Serialize(internal::MixedResponse());
    }

    network::Response CommandHandler::Error(const std::string& error) const
    {
        return _serializer->Serialize(internal::MixedResponse(error));
    }
};
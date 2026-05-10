#include <crossplatform.h>
#include "AlarmServer.h"
#include <FlexRsuConnect.h>
#include <rsuNoNames.h>
#include <FlexRsuCommon.h>
#include <rsuErr.h>

KNoName* g_SharedMemory = nullptr;

KAlarmServer* g_pAlarmFile = NULL;
void AlarmTag(SFlex* prawBlockInfo, bool bOn, LPCSTR pszAlrmType, __int64 moment,
    LPCSTR pszDrName, LPCSTR pszComment, LPCSTR pszComment1, LPCSTR pszComment2,
    LPCSTR pszAtValue, tDigital32& Acked, LPCSTR pszStandby1, LPCSTR pszStandby2, LPCSTR pszStandby3)
{
    if (!g_pAlarmFile || (g_SharedMemory && !g_SharedMemory->IsOpened()) || !prawBlockInfo)
    {
        return;
    }

    LPCSTR pszClass = NULL, pszName = NULL;
    int nFcs = 0;
    if (g_SharedMemory && !g_SharedMemory->GetNameEx(prawBlockInfo->ID_PNT_NAME, prawBlockInfo->ID_CLASS, &pszClass, &pszName, &nFcs))
    {
        return;
    }

    CAlarmEntry a;
    a.ID_CLASS = prawBlockInfo->ID_CLASS;
    a.ID_PNT_NAME = prawBlockInfo->ID_PNT_NAME;
    a.btOnOff = bOn;
    a.nMomentAbs = moment;
    a.bAcked = Acked ? true : false;
    strncpy(a.szName, pszName, _countof(a.szName) - 1);
    strncpy(a.szDescr, pszComment, _countof(a.szDescr) - 1);
    strncpy(a.szDescr1, pszComment1, _countof(a.szDescr1) - 1);
    strncpy(a.szDescr2, pszComment2, _countof(a.szDescr2) - 1);
    strncpy(a.szReserve1, pszStandby1, _countof(a.szReserve1) - 1);
    strncpy(a.szReserve2, pszStandby2, _countof(a.szReserve2) - 1);
    strncpy(a.szReserve3, pszStandby3, _countof(a.szReserve3) - 1);
    strncpy(a.szValue, pszAtValue, _countof(a.szValue) - 1);
    strncpy(a.szAlrmType, pszAlrmType, _countof(a.szAlrmType) - 1);
    if (pszDrName)
    {
        strncpy(a.szFileName, pszDrName, _countof(a.szFileName) - 1);
    }

    g_pAlarmFile->WriteAlarm(a);
    Acked = a.bAcked;
}

void FlexAckingAlarm(SFlex* prawBlockInfo, __int64 moment, bool bInAlarm)
{
    if (!g_pAlarmFile || (g_SharedMemory && !g_SharedMemory->IsOpened()) || !prawBlockInfo)
    {
        return;
    }

    LPCSTR pszClass = NULL, pszName = NULL;
    int nFcs = 0;
    if (g_SharedMemory && !g_SharedMemory->GetNameEx(prawBlockInfo->ID_PNT_NAME, prawBlockInfo->ID_CLASS, &pszClass, &pszName, &nFcs))
    {
        return;
    }

    g_pAlarmFile->AckAlarm(prawBlockInfo->ID_PNT_NAME, moment, bInAlarm);
}

KAlarmServer::KAlarmServer(LPCSTR pszModelName)
    : m_bInited(false),
    m_bStep0(false),
    m_bStep1(false)
{
    pFlexAlarm = &AlarmTag;
    pFlexAckingAlarm = &FlexAckingAlarm;
    char szName[64 * 4];
    ns_FlexRsu::GetSharedName(szName);
    g_SharedMemory = KNoName::Create(szName, false);
    g_pAlarmFile = this;
}

CFlexSystem* KAlarmServer::GetFlexSystemDataL()
{
    CBase* pBase = g_SharedMemory->CreateStruct("Весь Flex", CFlexSystem::TypeID, sizeof(CFlexSystem), 0);
    if (pBase)
    {
        ASS(pBase->size == sizeof(CFlexSystem));
        return (CFlexSystem*)pBase;
    }

    return NULL;
}

int KAlarmServer::Initialize()
{
    int result = 0;
    if (!m_bInited)
    {
        m_bInited = true;
        result = KAlarmsFlex::InitL();
    }

    return result;
}

int KAlarmServer::AfterInit()
{
    int result = 0;
    if (!m_bStep0)
    {
        result = KAlarmsFlex::Step1L();
    }
    else
    {
        KAlarmsFlex::Reset();
    }

    m_bStep0 = true;
    return result;
}

int KAlarmServer::AfterRestored()
{
    m_bStep1 = true;
    return KAlarmsFlex::Step1L();
}

int KAlarmServer::CalcStep(int dtMs)
{
    double dtS = dtMs / 1000.;
    return KAlarmsFlex::StepTL(dtS);
}

void KAlarmServer::WriteAlarm(CAlarmEntry& rAlarm)
{
    KAlarmsFlex::WriteAlarmImpl(rAlarm);
}

void KAlarmServer::AckAlarm(DWORD ID_PNT_NAME, __int64 moment, bool bInAlarm)
{
    KAlarmsFlex::AckAlarmImpl(ID_PNT_NAME, moment, bInAlarm);
}

namespace ns_details
{
    static LPCSTR s_szWndClassName = "YAlarmServer_ModelWindowClass";
    static int s_flagRegisteredWindow = 0;
}

#ifdef _WIN32
void KAlarmServer::ShowObject(LPCSTR pszName)
{
}
#endif
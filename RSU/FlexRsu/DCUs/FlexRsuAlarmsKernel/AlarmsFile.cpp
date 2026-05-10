#include "AlarmsFile.h"
#include <FlexRsuCommon.h>
#include <stdio.h>
#include <rsuErr.h>
#include <crosspath.h>
#include "LogFile.h"

CAlarmsFile::CAlarmsFile(int nNumber)
    : m_nNumber(nNumber)
    , m_pbBegWnd(NULL)
    , m_dwCurrSector(0)
    , m_pDataT(NULL)
{
    m_dwAllocKvant = m_RsuMio.page_size();
    m_dwCountKvant = m_dwAllocKvant / sizeof(CAlarmEntry);
    ASS(m_dwAllocKvant == m_dwCountKvant * sizeof(CAlarmEntry));
}

CAlarmsFile::~CAlarmsFile()
{
    Clear();
}

void CAlarmsFile::Clear()
{
    m_RsuMio.close();
    m_pbBegWnd = NULL;
    m_pDataT = NULL;
    m_dwCurrSector = 0;
}

bool CAlarmsFile::Init()
{
    Clear();
    return OpenAlarmData();
}

bool CAlarmsFile::OpenAlarmData()
{
    m_RsuMio.close();
    m_FileName = ns_FlexRsu::GetAlarmFileName(m_nNumber == 1);
    fs::path fn(m_FileName);
    m_RsuMio.open(fn.filename().generic_string().c_str(), m_dwAllocKvant, m_dwAllocKvant * m_dwCurrSector, fn.parent_path().generic_string().c_str());
    return m_RsuMio.state();
}

void CAlarmsFile::SaveAlarm(CAlarmEntry& rAlarm, DWORD& nAlarms)
{
    if ((nAlarms % m_dwCountKvant) == 0)
    {
        if (m_pbBegWnd != NULL)
        {
            m_dwCurrSector++;
        }

        bool success = OpenAlarmData();
        if (!success)
        {
            CLogFile::LogEx(LOGLEVEL_HIGH, "Can't write alarm into '%s'. Error: ''", m_FileName.c_str(), m_RsuMio.last_error().message().c_str());
            return;
        }

        m_pbBegWnd = (BYTE*)m_RsuMio.data();
        ASS(m_pbBegWnd != NULL);
        m_pDataT = (CAlarmEntry*)(m_pbBegWnd);
        memset(m_pDataT, 0, m_dwAllocKvant);
    }

    memcpy(m_pDataT, &rAlarm, sizeof(CAlarmEntry));
    ++m_pDataT;
    nAlarms++;
}

bool CAlarmsFile::FindAlarm(CAlarmEntry& rAlarm, DWORD& nAlarms)
{
    CAlarmEntry* a = (CAlarmEntry*)m_pbBegWnd;
    if (a == NULL)
    {
        return false;
    }

    bool ret = false;
    return ret;
}

void CAlarmsFile::Flush()
{
    if (m_pbBegWnd)
    {
        m_RsuMio.flush();
    }
}

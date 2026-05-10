#include <crossplatform.h>
#include "AlarmsFlex.h"
#include <EventLogTypes.h>
#include <crosslock.h>
#include <fstream>
#include <Common.h>

KAlarmsFlex::KAlarmsFlex()
    : m_pFlexSystem(NULL)
    , m_bChangeFile(false)
    , file0(0)
    , file1(1)
    , m_nStep(0)
{

}

KAlarmsFlex::~KAlarmsFlex()
{

}

int KAlarmsFlex::InitL()
{
    m_pFlexSystem = GetFlexSystemDataL();
    if (!m_pFlexSystem)
    {
        return 1;
    }
    // Обнуляем содержимое файла сигнализаций
    if (!file0.Init())
    {
        return 2;
    }

    if (!file1.Init())
    {
        return 3;
    }

    Reset();
    return 0;
}

int KAlarmsFlex::Step1L()
{
    Reset();
    return 0;
}

void KAlarmsFlex::Reset()
{
    if (m_pFlexSystem)
    {
        m_pFlexSystem->nFlexAlarms0 = 0;
        m_pFlexSystem->nFlexAlarms1 = 0;
        m_pFlexSystem->nFlexAlarmXY = 0;
        m_pFlexSystem->bSound = false;
    }

    file0.Clear();
    file1.Clear();
    file0.Init();
    file1.Init();
    m_nStep = 0;
}

int KAlarmsFlex::StepTL(double dtS)
{
    if (!m_pFlexSystem)
    {
        return 0;
    }

    LOCK(m_csOnContour);
    ++m_nStep;
    if (!m_bChangeFile)
    {
        return 0;
    }

    m_bChangeFile = false;
    int nFileA = m_pFlexSystem->FileFlexA();
    CAlarmsFile& src = (nFileA) ? file1 : file0;
    CAlarmsFile& dst = (nFileA) ? file0 : file1;
    dst.Init();
    DWORD  nSrc = (nFileA) ? m_pFlexSystem->nFlexAlarms1 : m_pFlexSystem->nFlexAlarms0;
    DWORD& nDst = (nFileA) ? m_pFlexSystem->nFlexAlarms0 : m_pFlexSystem->nFlexAlarms1;
	src.Flush();
    std::ifstream hFile(src.FileName());
    if (hFile.is_open())
    {
        DWORD dwSize = nSrc * sizeof(CAlarmEntry);
        std::vector<CAlarmEntry> data;
        data.resize(nSrc);
        if (!data.empty())
        {
            DWORD dwRead = 0;
            hFile.read((char*)&data.at(0), dwSize);
            BOOL fSuccess = false;
            if (hFile)// Читаем данные
            {
                nDst = 0;// Дошли до анализа
                Analiz(dst, nDst, nSrc, &data.at(0));
                m_pFlexSystem->nFlexAlarmXY++;
            }
        }
    }

    return 0;
}

void KAlarmsFlex::Analiz(CAlarmsFile& file, DWORD& nAlarm, int nCount, CAlarmEntry* data)
{
    for (int n = 0; n < nCount; n++)
    {
        if (data[n].bAcked && !data[n].btOnOff)
        {
            continue;
        }

        file.SaveAlarm(data[n], nAlarm);
    }
}

// Запись в файл сигнализаций
void KAlarmsFlex::WriteAlarmImpl(CAlarmEntry& rAlarm)
{
    if (!m_pFlexSystem)
    {
        return;
    }

    LOCK(m_csOnContour);
    BOOL bWriteAsIs = TRUE;
    if (rAlarm.bAcked && !rAlarm.btOnOff)
    {
        int nFileA = m_pFlexSystem->FileFlexA();
        CAlarmsFile& src = (nFileA) ? file1 : file0;
        CAlarmsFile& dst = (nFileA) ? file0 : file1;
        int nSrc = (nFileA) ? m_pFlexSystem->nFlexAlarms1 : m_pFlexSystem->nFlexAlarms0;
        DWORD& nDst = (nFileA) ? m_pFlexSystem->nFlexAlarms0 : m_pFlexSystem->nFlexAlarms1;
        std::ifstream hFile(src.FileName()); //HANDLE hFile = src.HandleT();
        if (hFile.is_open())// Открываем файл
        {
            DWORD dwSize = nSrc * sizeof(CAlarmEntry);
            std::vector<CAlarmEntry> data;
            data.resize(nSrc);
            if (!data.empty())
            {
                hFile.read((char*)&data.at(0), dwSize);
                if (hFile)
                {
                    dst.Init();
                    nDst = 0;
                    for (int i = 0; i < nSrc; i++)
                    {
                        if (data[i].ID_PNT_NAME == rAlarm.ID_PNT_NAME)
                        {
                            continue;
                        }

                        dst.SaveAlarm(data[i], nDst);
                    }
                }

                m_pFlexSystem->nFlexAlarmXY++;
            }

            return;
        }

        m_bChangeFile = true;
    }

    if (bWriteAsIs)
    {
        if (m_pFlexSystem->FileFlexA())
        {
            file1.SaveAlarm(rAlarm, m_pFlexSystem->nFlexAlarms1);
        }
        else
        {
            file0.SaveAlarm(rAlarm, m_pFlexSystem->nFlexAlarms0);
        }
    }

    char* filtr = "%s (%s)";
    static char* a = ("Сигнализация %s (%s)");
    static char* b = ("Конец сигнализации %s (%s)");
    if (rAlarm.btOnOff)
    {
        filtr = a;
        m_pFlexSystem->bSound = (m_nStep > 50) ? true : false;
    }
    else
    {
        filtr = b;
    }

    if (filtr == NULL)
    {
        return;
    }
}

void KAlarmsFlex::AckAlarmImpl(DWORD ID_PNT_NAME, __int64 moment, bool bInAlarm)
{
    if (!m_pFlexSystem)
    {
        return;
    }

    LOCK(m_csOnContour);
    int nFileA = m_pFlexSystem->FileFlexA();
    CAlarmsFile& src = (nFileA) ? file1 : file0;
    CAlarmsFile& dst = (nFileA) ? file0 : file1;
    int nSrc = (nFileA) ? m_pFlexSystem->nFlexAlarms1 : m_pFlexSystem->nFlexAlarms0;
    DWORD& nDst = (nFileA) ? m_pFlexSystem->nFlexAlarms0 : m_pFlexSystem->nFlexAlarms1;
    if (!nSrc)
    {
        return;
    }

    std::ifstream hFile(src.FileName());
    if (!hFile.is_open())
    {
        return;
    }

    DWORD dwSize = nSrc * sizeof(CAlarmEntry);
    std::vector<CAlarmEntry> data;
    data.resize(nSrc);
    hFile.read((char*)&data.at(0), dwSize);
    if (!hFile)
    {
        return;
    }

    dst.Init();
    nDst = 0;
    for (int i = 0; i < nSrc; i++)
    {
        CAlarmEntry& alrm = data[i];
        if (alrm.ID_PNT_NAME == ID_PNT_NAME)
        {
            if (!bInAlarm)
            {
                continue;
            }

            alrm.bAcked = true;
        }

        dst.SaveAlarm(alrm, nDst);
    }

    m_pFlexSystem->nFlexAlarmXY++;
}

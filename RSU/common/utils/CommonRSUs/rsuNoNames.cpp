#include <rsuNoNames.h>
#include <stdio.h>
#include <rsuErr.h>
#include "OnDiskAllocator.h"
#include <rsuClipboard.h>
#include <cstring>
#include <hashRSU.h>

static KNoName s_Opened[10];
LPCSTR KNoName::s_ClassNames[KNoName::eClassNamesCount] = { 0 };
tRsuHandleInitData KNoName::s_InitDataCalls[KNoName::eRsuSupportCount] = { 0 };

static LPCSTR s_pText = "Рекомендуется очистить папку \"Memory\"";
static LPCSTR s_pCaption = "Неустранимая ошибка";

template<typename T, size_t _S>
int ExtendArray(T(&arr)[_S], T pNewCall)
{
    if (pNewCall)
    {
        for (int i = 0; i < _S; i++)
        {
            if (arr[i] == pNewCall)
            {
                return i;
            }

            if (arr[i])
            {
                continue;
            }

            arr[i] = pNewCall;
            return i;
        }
    }

    return -1;
}

template<typename T, size_t _S, typename A>
int RsuHandleCall(T(&arr)[_S], A a)
{
    for (int i = 0; i < sizeof(arr); i++)
    {
        if (!arr[i])
        {
            continue;
        }

        if (0 == arr[i](a))
        {
            return 0;
        }
    }

    return 1;
}
//////////////////////////////////////////////////////////////////////////
KNoName::KNoName()
    : pHeader(nullptr)
    , pOrd(nullptr)
    , pszString4Ords(nullptr)
    , pszObjects(nullptr)
    , m_hVarMapping(nullptr)
    , m_bCrc2Upper(true)
    , m_bCompareByCrc(false)
{
    pfStringComparator = strcmp;

}

KNoName::KNoName(KNoName& src)
{
    this->operator = (src);
}

bool KNoName::IsOpened()
{
    return pHeader ? true : false;
}

KNoName& KNoName::operator = (const KNoName& src)
{
    memcpy(this, &src, sizeof(*this));
    if (pHeader)
    {
        pHeader->clientCounter += 1;
    }

    return *this;
}

KNoName::~KNoName()
{
    if (pHeader)
    {
        pHeader->clientCounter -= 1;
    }
}

template<typename TL, typename TR>
void Test4NewMem(TL thl, TR thr, bool& res)
{
    if (0 == thl)
    {
        return;
    }

    if (thl != thr)
    {
        res = true;
    }
}
static LPCSTR s_szMemExt = "noname";
KNoName* KNoName::Create(LPCSTR pszName, bool bMaster, DWORD nNames /*= 800000*/, DWORD nBtsStrings /*= 0xd00000*/, DWORD nBtsObjs /*= 0x0dd00000*/)
{
    int nEmtpy = -1;
    for (int i = 0; i < _countof(s_Opened); ++i)
    {
        if (!s_Opened[i].pHeader)
        {
            if (-1 == nEmtpy)
            {
                nEmtpy = i;
            }

            continue;
        }

        if (!strcmp(pszName, s_Opened[i].pHeader->szName))
        {
            return &s_Opened[i];
        }
    }

    KNoName* nn = new KNoName();
    if (-1 == nEmtpy)
    {
        return nn;
    }

    DWORD Size = sizeof(*nn->pHeader) + sizeof(*nn->pOrd) * nNames + nBtsStrings + nBtsObjs;
    bool bNewMem = false;
    SNoNameHeader* pHeader = (SNoNameHeader*)OnDiskAllocator(nn->m_hVarMapping, bNewMem, Size, pszName, s_szMemExt, bMaster);
    if (nullptr == pHeader)
    {
        return nn;
    }

    if (bNewMem && !bMaster)
    {
        ASSD(!"как-то криво");
        if (nn->m_hVarMapping != nullptr)
        {
            nn->m_hVarMapping->unmap();
        }

        return nn;
    }

    nn->pHeader = pHeader;
    SNoNameHeader& header = *nn->pHeader;
    if (bMaster)
    {
        Test4NewMem(header.shiftOrds, sizeof(header), bNewMem);
        header.shiftOrds = sizeof(header);
        Test4NewMem(header.countOrds, nNames, bNewMem);
        header.countOrds = nNames;
        Test4NewMem(header.shiftBuff4Ords, header.shiftOrds + header.countOrds * sizeof(*nn->pOrd), bNewMem);
        header.shiftBuff4Ords = header.shiftOrds + header.countOrds * sizeof(*nn->pOrd);
        Test4NewMem(header.nBuff4Ords, nBtsStrings, bNewMem);
        header.nBuff4Ords = nBtsStrings;
        Test4NewMem(header.shiftObjects, header.shiftBuff4Ords + nBtsStrings, bNewMem);
        header.shiftObjects = header.shiftBuff4Ords + nBtsStrings;
        Test4NewMem(header.nBuffObjects, nBtsObjs, bNewMem);
        header.nBuffObjects = nBtsObjs;
        strcpy_s(header.szName, pszName);
        header.clientCounter = 0;
    }

    BYTE* pMem = (BYTE*)nn->pHeader;
    nn->pOrd = (SBuffOrd*)(pMem + header.shiftOrds);
    nn->pszString4Ords = pMem + header.shiftBuff4Ords;
    nn->pszObjects = pMem + header.shiftObjects;
    if (bNewMem)
    {
        memset(nn->pOrd, 0, header.countOrds * sizeof(*nn->pOrd));
        memset(nn->pszString4Ords, 0, header.nBuff4Ords);
        memset(nn->pszObjects, 0, nBtsObjs);
        header.nCurr = sizeof(SBuffRecSeparator);//5
    }

    s_Opened[nEmtpy] = *nn;
    return nn;
}

void KNoName::Reset()
{
    SNoNameHeader& header = *pHeader;
    KNoName& nn = *this;
    memset(nn.pOrd, 0, header.countOrds * sizeof(*nn.pOrd));
    memset(nn.pszString4Ords, 0, header.nBuff4Ords);
    memset(nn.pszObjects, 0, header.nBuffObjects);
    header.nCurr = sizeof(SBuffRecSeparator);//5
    header.nMaxPos = 0;
    header.nCount = 0;
}

void KNoName::SetCompareMethod(bool bUseCrc)
{
    m_bCompareByCrc = bUseCrc;
}

KNoName::tNamesStringComparator KNoName::SetStringComparator(tNamesStringComparator pfnOther)
{
    tNamesStringComparator pCur = pfStringComparator;
    pfStringComparator = pfnOther;
    return pCur;
}

void KNoName::PrintStatistic()
{
    if (!pHeader)
    {
        return;
    }
}

int KNoName::RegisterClassInNames(DWORD idClass, LPCSTR pszClassName)
{
    ASS(idClass < sizeof(s_ClassNames));
    if (s_ClassNames[idClass])
    {
        ASS(!strcmp(s_ClassNames[idClass], pszClassName));
        return idClass;
    }

    s_ClassNames[idClass] = pszClassName;
    return idClass;
}

void KNoName::RegisterInitCBaseCall(tRsuHandleInitData pInitData)
{
    ExtendArray(s_InitDataCalls, pInitData);
}
//////////////////////////////////////////////////////////////////////////
#include <CBase.h>
CBase* KNoName::CreateStruct(LPCTSTR ObjName, DWORD TypeID, int number)
{
    int ID_PNT_NAME = GetID(TypeID, ObjName, true, number);
    if (ID_PNT_NAME <= 0)
    {
        return nullptr;
    }

    CBase* pBase = GetNameData(ID_PNT_NAME);
    if (pBase)
    {
        return pBase;
    }

    return AppendData(ID_PNT_NAME, TypeID);
}

CBase* KNoName::CreateStruct(LPCTSTR ObjName, DWORD TypeID, DWORD sizeObj, int number)
{
    int ID_PNT_NAME = GetID(TypeID, ObjName, true, number);
    if (ID_PNT_NAME <= 0)
    {
        return nullptr;
    }

    CBase* pBase = GetNameData(ID_PNT_NAME);
    if (pBase)
    {
        if (sizeObj != pBase->size)
        {
#ifdef _WIN32
            rsuTxtToClp(ObjName);
            char szBuf[1024] = {};
            snprintf(szBuf, sizeof(szBuf), "%s\nпроблемный объект - \"%s\"", s_pText, ObjName);
            MessageBox(nullptr, szBuf, s_pCaption, MB_OK | MB_ICONASTERISK);
#endif
        }

        ASS(sizeObj == pBase->size)
        {
            return pBase;
        }
    }

    return AppendData(ID_PNT_NAME, TypeID, sizeObj);
}

CBase* KNoName::FindStruct(LPCTSTR ObjName, DWORD TypeID, int number)
{
    int ID_PNT_NAME = GetID(TypeID, ObjName, false, number);
    if (ID_PNT_NAME <= 0)
    {
        return nullptr;
    }

    CBase* pBase = GetNameData(ID_PNT_NAME);
    if (pBase)
    {
        return pBase;
    }

    return nullptr;
}

int KNoName::GetID(LPCSTR szClass, LPCSTR szEntry, bool bAdd, int number)
{
    if (!szClass)
    {
        return 0;
    }

    if (!szEntry)
    {
        return 0;
    }

    if (!pHeader)
    {
        return 0;
    }

    const DWORD classCrc = m_bCompareByCrc ? CreateHASH32(szClass, m_bCrc2Upper) : 0,
        entryCrc = m_bCompareByCrc ? CreateHASH32(szEntry, m_bCrc2Upper) : 0;
    int K = pHeader->nCount;
    int nb = -1, ne = K, no, n, Res;
    UINT nClass = 0;
    UINT nEntry = 0;
    while (ne - nb > 1)
    {
        n = (nb + ne) >> 1;
        no = pOrd[n].nOrd;
        DWORD testClassCrc = 0, testEntryCrc = 0;
        LPCSTR pTestClass = Class(pOrd[no], testClassCrc);
        if (m_bCompareByCrc)
        {
            if (classCrc == testClassCrc)
            {
                Res = 0;
            }
            else if (testClassCrc < classCrc)
            {
                Res = -1;
            }
            else
            {
                Res = 1;
            }
        }
        else
        {
            Res = pfStringComparator(pTestClass, szClass);
        }

        if (Res == 0)
        {
            LPCSTR pTestEntry = Entry(pOrd[no], testEntryCrc);
            if (m_bCompareByCrc)
            {
                if (entryCrc == testEntryCrc)
                {
                    Res = 0;
                }
                else if (testEntryCrc < entryCrc)
                {
                    Res = -1;
                }
                else
                {
                    Res = 1;
                }
            }
            else
            {
                Res = pfStringComparator(pTestEntry, szEntry);
            }

            nClass = pOrd[no].nClass;
        }

        if (Res == 0)
        {
            nEntry = pOrd[no].nEntry;
            if (number > 0)
            {
                int n = Number(pOrd[no]);
                Res = n == number ? 0 : (n < number ? -1 : 1);
            }
        }

        if (Res == 0)
        {
            return no + 1;
        }

        if (Res < 0)
        {
            ne = n;
        }
        else
        {
            nb = n;
        }
    }

    if (!bAdd)
    {
        return 0;
    }
    // Новое имя сразу за nb
    ASS(K < pHeader->countOrds /*MAX_NAMES*/);
    if (nClass == 0)
    {
        pOrd[K].nClass = pHeader->nCurr;
        SetBuffer(szClass);
    }
    else
    {
        pOrd[K].nClass = nClass;
    }

    if (nEntry == 0)
    {
        pOrd[K].nEntry = pHeader->nCurr;
        SetBuffer(szEntry);
    }
    else
        pOrd[K].nEntry = nEntry;

    pOrd[K].nData = -1;
    pOrd[K].nNumber = std::max(0, number);
    //
    if (nb < K - 1)
    {
        for (int n = K - 1; n > nb; --n)
        {
            pOrd[n + 1].nOrd = pOrd[n].nOrd;
        }
    }

    pOrd[nb + 1].nOrd = K;
    ++K;
    pHeader->nCount = K;
    return K;
}

int KNoName::GetID(DWORD TypeID, LPCSTR szEntry, bool bAdd, int number)
{
    if (TypeID > sizeof(s_ClassNames))
    {
        return -1;
    }

    return GetID(s_ClassNames[TypeID], szEntry, bAdd, number);
}



LPCSTR KNoName::Class(SBuffOrd& _ord, DWORD& crc)
{
    if (!pszString4Ords)
    {
        return nullptr;
    }

    SBuffRecSeparator* buf = (SBuffRecSeparator*)(pszString4Ords + _ord.nClass);
    crc = buf->crc32;
    return buf->str;
}

LPCSTR KNoName::Entry(SBuffOrd& _ord, DWORD& crc)
{
    if (!pszString4Ords)
    {
        return nullptr;
    }

    SBuffRecSeparator* buf = (SBuffRecSeparator*)(pszString4Ords + _ord.nEntry);
    crc = buf->crc32;
    return buf->str;
}

int KNoName::Number(SBuffOrd& _ord)
{
    return _ord.nNumber;
}

LPCSTR KNoName::Class(DWORD ID, DWORD& crc)
{
    ASSD(ID < pHeader->countOrds);
    ASSD(ID < pHeader->nCount);
    return Class(pOrd[ID], crc);
}

LPCSTR KNoName::Entry(DWORD ID, DWORD& crc)
{
    ASSD(ID < pHeader->countOrds);
    ASSD(ID < pHeader->nCount);
    return Entry(pOrd[ID], crc);
}

int KNoName::Number(DWORD ID)
{
    ASSD(ID < pHeader->countOrds);
    ASSD(ID < pHeader->nCount);
    return Number(pOrd[ID]);
}

void KNoName::SetBuffer(LPCSTR text)
{
    if (!pHeader)
    {
        return;
    }

    UINT L = (UINT)strlen(text);
    UINT N = pHeader->nCurr + L + sizeof(SBuffRecSeparator);
    ASS(N < pHeader->nBuff4Ords/*MAX_BUFFER*/);
    if (N > pHeader->nBuff4Ords)
    {
        exit(1);
    }

    SBuffRecSeparator* rec = (SBuffRecSeparator*)(pszString4Ords + pHeader->nCurr);
    rec->strLength = L;
    rec->crc32 = m_bCompareByCrc ? CreateHASH32(text, m_bCrc2Upper) : 0;
    memcpy(rec->str, text, L + 1);
    pHeader->nCurr = N;
}

CBase* KNoName::GetNameData(int ID)
{
    if (!pHeader)
    {
        return nullptr;
    }

    ID--;
    ASS(ID < pHeader->nCount);
    ASSD(ID < pHeader->countOrds);
    if (pOrd[ID].nData != -1)
    {
        CBase* pBase = (CBase*)(pszObjects + pOrd[ID].nData);
#ifdef _WIN32
        if (!pBase->size)
        {
            MessageBox(nullptr, s_pText, s_pCaption, MB_OK | MB_ICONASTERISK);
        }
#endif
        ASS(pBase->size > 0);
        return pBase;
    }

    return nullptr;
}

CBase* KNoName::AppendData(int ID, DWORD TYPE, DWORD size/* = 0*/)
{
    if (!pHeader || !pszObjects)
    {
        return nullptr;
    }

    CBase* pBase = (CBase*)(pszObjects + pHeader->nMaxPos);
    pBase->ID_PNT_NAME = ID;
    pBase->ID_CLASS = TYPE;
    if (0 == size)
    {
        if (InitData(pBase) != 0)
        {
            return nullptr;
        }
    }
    else
    {
        pBase->size = size;
    }

    ASS(pBase->size > sizeof(*pBase));
    ID--;
    ASSD(ID < pHeader->nCount);
    ASSD(ID < pHeader->countOrds);
    if (ID >= pHeader->countOrds)
    {
        exit(1);
    }

    pOrd[ID].nData = pHeader->nMaxPos;
    pHeader->nMaxPos += pBase->size;
    ASS(pHeader->nMaxPos < pHeader->nBuffObjects /*MEM_SIZE*/);
    if (pHeader->nMaxPos >= pHeader->nBuffObjects)
    {
#ifdef _WIN32
        ::MessageBox(nullptr,
            "Нехватка памяти под объекты\nобратитесь к разработчику",
            pHeader->szName,
            MB_ICONSTOP);
#endif
        exit(1);
    }

    return pBase;
}

size_t KNoName::GetNAddr(DWORD ID)
{
    ID--;
    ASS(ID >= 0);
    ASSD(ID < pHeader->nCount);
    ASSD(ID < pHeader->countOrds);
    return pOrd[ID].nData;
}

CBase* KNoName::GetBase(size_t AddrBase)
{
    ASSD(AddrBase < pHeader->nMaxPos);
    if (AddrBase > pHeader->nMaxPos)
    {
        return nullptr;
    }

    CBase* pBase = (CBase*)(pszObjects + AddrBase);
    ASSD(pBase->ID_PNT_NAME - 1 < pHeader->nCount);
    ASSD(pBase->ID_PNT_NAME - 1 < pHeader->countOrds);
    ASSD(pOrd[pBase->ID_PNT_NAME - 1].nData == AddrBase);
    return pBase;
}

int KNoName::InitData(CBase* pObj)
{
    return RsuHandleCall(s_InitDataCalls, pObj);
}

LPCSTR KNoName::GetEmpty()
{
    if (!pszString4Ords)
    {
        return nullptr;
    }

    return (LPCSTR)pszString4Ords;
}

int KNoName::CreateRefToData(LPCSTR ObjName, CBase* pBase, int newNumber /*= -1*/)
{
    if (!pOrd)
    {
        return -1;
    }

    ASSD(pBase->ID_PNT_NAME - 1 < pHeader->nCount);
    ASSD(pBase->ID_PNT_NAME - 1 < pHeader->countOrds);
    int number = pOrd[pBase->ID_PNT_NAME - 1].nNumber;
    int ID_PNT_NAME = GetID(pBase->ID_CLASS, ObjName, true, number);
    if (ID_PNT_NAME <= 0)
    {
        return -1;
    }

    ASSD(ID_PNT_NAME - 1 < pHeader->nCount);
    ASSD(ID_PNT_NAME - 1 < pHeader->countOrds);
    pOrd[ID_PNT_NAME - 1].nData = pOrd[pBase->ID_PNT_NAME - 1].nData;
    return 0;
}

bool KNoName::WhileBase(DWORD& ID, const DWORD ClassID, CBase** base, LPCSTR* pszName, int* pFcsNumber)
{
    if (!pHeader)
    {
        return false;
    }

    const DWORD& nCount = pHeader->nCount;
    for (; ID < nCount; ++ID)
    {
        CBase* pBase = GetNameData(ID + 1);
        if (!pBase)
        {
            continue;
        }

        if (ClassID == -1 || ClassID == pBase->ID_CLASS)
        {
            ID++;
            ASS(int(pBase->size) > 0);
            if (base)
            {
                *base = pBase;
            }

            GetNameEx(ID, nullptr, pszName, pFcsNumber);
            return true;
        }
    }

    return false;
}

bool KNoName::GetNameEx(DWORD ID, const char** pszClass, const char** pszEntry, int* pFcsNumber /*= nullptr*/)
{
    if (!pHeader)
    {
        return false;
    }

    ID--;
    if (ID < pHeader->nCount)
    {
        if (pszClass != nullptr)
        {
            DWORD classCrc = 0;
            *pszClass = Class(ID, classCrc);
        }
        if (pszEntry != nullptr)
        {
            DWORD entryCrc = 0;
            *pszEntry = Entry(ID, entryCrc);
        }
        if (pFcsNumber != nullptr) *pFcsNumber = Number(ID);
        return true;
    }
    // В начале буфера всегда нули
    if (pszClass != nullptr) *pszClass = GetEmpty();
    if (pszEntry != nullptr) *pszEntry = GetEmpty();
    if (pFcsNumber != nullptr) *pFcsNumber = 0;
    return false;
}

bool KNoName::GetNameEx(DWORD ID, DWORD ClassID, LPCSTR* pszClass, LPCSTR* pszEntry, int* pFcsNumber /*= nullptr*/)
{
    if (!pHeader)
        return false;
    ID--;
    if (ID < pHeader->nCount)
    {
        CBase* pBase = GetNameData(ID + 1);
        if (pBase && ClassID == pBase->ID_CLASS)
        {
            if (pszClass != nullptr)
            {
                DWORD classCrc = 0;
                *pszClass = Class(ID, classCrc);
            }
            if (pszEntry != nullptr)
            {
                DWORD entryCrc = 0;
                *pszEntry = Entry(ID, entryCrc);
            }
    
            if (pFcsNumber != nullptr)
            {
                *pFcsNumber = Number(ID);
            }

            return true;
        }
    }
    // В начале буфера всегда нули
    if (pszClass != nullptr)
    {
        *pszClass = GetEmpty();
    }

    if (pszEntry != nullptr)
    {
        *pszEntry = GetEmpty();
    }

    if (pFcsNumber != nullptr)
    {
        *pFcsNumber = 0;
    }

    return false;
}

void KNoName::GetName(DWORD ID, char* szClass, size_t _SC, char* szEntry, size_t _SE)
{
    if (!pHeader)
    {
        return;
    }

    ID--;
    ASS((unsigned int)ID < pHeader->nCount);
    DWORD classCrc = 0, entryCrc = 0;
    if (szClass != nullptr)
    {
        strncpy(szClass, Class(ID, classCrc), _SC);
    }

    if (szEntry != nullptr)
    {
        strncpy(szEntry, Entry(ID, entryCrc), _SE);
    }
}

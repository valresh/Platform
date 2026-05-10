#include <crossplatform.h>
#include "SharedListValue.h"
#include <macros/AutoCloser.h>
#include <TagType.h>
#include <SharedFlexBlocks.h>

extern FLEXRSULIST_API SListValue _theList;

//extern HANDLE hSemValue;

SListValue::SListValue()
    : mName(mChar)
{
}

static const char g_chVer = 1;

namespace ns_local
{
    struct SFindAndSort { const char* name; void* Void; };
    inline int SortForName(const void* d1, const void* d2)
    {
        SFindAndSort* p1 = (SFindAndSort*)d1;
        SFindAndSort* p2 = (SFindAndSort*)d2;
        return _Kstricmp(p1->name, p2->name);
    }

    void Qsort(void* list, size_t nCount, size_t nSize, bool& bInit)
    {
        if (bInit)
        {
            bInit = false;
            qsort(list, nCount, nSize, SortForName);
        }
    }

    inline int FindForName(const void* d1, const void* d2)
    {
        const char* p1 = (const char*)d1;
        SFindAndSort* p2 = (SFindAndSort*)d2;
        return _Kstricmp(p1, p2->name);
    }
    void* SearchName(const char* name, const void* list, size_t nCount, size_t nSize)
    {
        return bsearch(name, list, nCount, nSize, FindForName);
    }
}

STagType* FindTagType(char* name)
{
    static STagType list[] =
    {
      {"DIO", NULL, S_DIO::TypeID, S_DIO::BuildList },
      {"AIO", NULL, S_AIO::TypeID, S_AIO::BuildList },
      {"PID", NULL, S_PID::TypeID, S_PID::BuildList },
      {"A2oo3", NULL, S_A2oo3::TypeID, S_A2oo3::BuildList },
      {"OPCAI", NULL, S_OPCAI::TypeID, S_OPCAI::BuildList },
      {"OPCAO", NULL, S_OPCAO::TypeID, S_OPCAO::BuildList },
      {"OPCDI", NULL, S_OPCDI::TypeID, S_OPCDI::BuildList },
      {"OPCDO", NULL, S_OPCDO::TypeID, S_OPCDO::BuildList },
      {"ANATREND", NULL, S_ANATREND::TypeID, S_ANATREND::BuildList },
      {"TCOREAI", NULL, S_TCOREAI::TypeID, S_TCOREAI::BuildList },
      {"TCOREAO", NULL, S_TCOREAO::TypeID, S_TCOREAO::BuildList },
      {"TCOREDI", NULL, S_TCOREDI::TypeID, S_TCOREDI::BuildList },
      {"TCOREDO", NULL, S_TCOREDO::TypeID, S_TCOREDO::BuildList },
      {"TCOREMAI", NULL, S_TCOREMAI::TypeID, S_TCOREMAI::BuildList },
      {"FUTURETREND", NULL, SFlexFutureTrend::TypeID, SFlexFutureTrend::BuildList },
      {"DIGALRM", NULL, S_DIGALRM::TypeID, S_DIGALRM::BuildList },
      {"M2U", NULL, S_M2U::TypeID, S_M2U::BuildList },
      {"COT", NULL, S_COT::TypeID, S_COT::BuildList },
      {"SYSTIME", NULL, S_SYSTIME::TypeID, S_SYSTIME::BuildList },
      {"SYSDATE", NULL, S_SYSDATE::TypeID, S_SYSDATE::BuildList },
      {"ANALRM", NULL, S_ANALRM::TypeID, S_ANALRM::BuildList },
      {"PIDA", NULL, S_PIDA::TypeID, S_PIDA::BuildList },
      {"MOTOR", NULL, S_MOTOR::TypeID, S_MOTOR::BuildList },
      {"VLV", NULL, S_VLV::TypeID, S_VLV::BuildList },
      {"EV", NULL, S_EV::TypeID, S_EV::BuildList },
      {"ADDER", NULL, S_ADDER::TypeID, S_ADDER::BuildList },
      {"RAMP", NULL, S_RAMP::TypeID, S_RAMP::BuildList },
      {"TXTSET", NULL, S_TXTSET::TypeID, S_TXTSET::BuildList },
      {"TMR", NULL, S_TMR::TypeID, S_TMR::BuildList },
      {"PidSKVO", NULL, S_PidSKVO::TypeID, S_PidSKVO::BuildList },
      {"DCI16", NULL, S_DCI16::TypeID, S_DCI16::BuildList },
      {"DCI16", NULL, S_DCI16::TypeID, S_DCI16::BuildList },
      {"SCLR", NULL, S_SCLR::TypeID, S_SCLR::BuildList },
        {"SimDO", NULL, S_SimDO::TypeID, S_SimDO::BuildList },
      {"SimDI", NULL, S_SimDI::TypeID, S_SimDI::BuildList },
      {"SimAO", NULL, S_SimAO::TypeID, S_SimAO::BuildList },
      {"SimAI", NULL, S_SimAI::TypeID, S_SimAI::BuildList },
    };
    static bool bInit = true;
    ns_local::Qsort(list, _countof(list), sizeof(list[0]), bInit);
    if (name == NULL)
        return list;
    if (*name == 0)
        return (STagType*)_countof(list);
    return (STagType*)ns_local::SearchName(name, list, _countof(list), sizeof(list[0]));
}

int SortValueDef(const void* d1, const void* d2)
{
    SFullVarInfo* p1 = (SFullVarInfo*)d1;
    SFullVarInfo* p2 = (SFullVarInfo*)d2;

    if (p1->structType > p2->structType)
        return  1;
    if (p1->structType < p2->structType)
        return -1;

    int ret = _strcmpi(p1->pszName, p2->pszName);
    return ret;
}

void SListValue::Attach()
{
    //BuildList();
    char null = 0;
    mChar.AddObj(&null);
    STagType* lst = FindTagType(NULL);
    size_t nCount1 = (size_t)FindTagType("");
    for (size_t n = 0; n < nCount1; n++)
    {
        lst[n].build();
    }

    nCount1 = Count();
    //BuildName();
    SFullVarInfo* obj = Obj(0);
    size_t nCount2 = Count();
    for (size_t n = 0; n < nCount2; n++)
    {
        obj[n].pszName = mName.Name(HNAME(obj[n].pszName));
    }

    qsort(obj, nCount2, sizeof(obj[0]), SortValueDef);
}

void SListValue::Detach()
{
    Clear();
    mEnum.Clear();
    //mHand.Clear();
    mName.Clear();
    mChar.Clear();
}

// Построение списка переменных
void SListValue::BuildList()
{
    char null = 0;
    mChar.AddObj(&null);

    STagType* lst = FindTagType(NULL);
    size_t nCount = (size_t)FindTagType("");
    for (size_t n = 0; n < nCount; n++)
    {
        lst[n].build();
    }
    nCount = Count();
}

// Преобразование HNAME -> const char*
void SListValue::BuildName()
{
    SFullVarInfo* obj = Obj(0);
    UINT nCount = Count();
    for (UINT n = 0; n < nCount; n++)
        obj[n].pszName = mName.Name(HNAME(obj[n].pszName));
    qsort(obj, nCount, sizeof(obj[0]), SortValueDef);
}

SFullVarInfo& SListValue::AddRaw(LPCSTR name, eVarType eType, DWORD structType, tShiftType_ rawshift, tVarSizeType_ size)
{
    HNAME hName = mName.Add(name);
    SFullVarInfo obj;
    obj.structType = structType;
    obj.pszName = LPCSTR(hName);
    obj.evtType = eType;
    obj.shift = rawshift;
    obj.size = size;
    obj.flags = 0;
    UINT n = AddObj(&obj);
    SFullVarInfo& ret = *Obj(n);
    return ret;
}

SVarInfo* SListValue::Find(LPCSTR pszField, DWORD structType)
{
    if (!pszField)
        return NULL;
    //
    SFullVarInfo* obj = Obj(0);
    UINT nCount = Count();
    //
    SFullVarInfo fnd;
    fnd.structType = structType;
    fnd.pszName = pszField;
    fnd.evtType = evtHZ;
    fnd.shift = 0;
    fnd.size = 0;
    SFullVarInfo* r = (SFullVarInfo*)bsearch(&fnd, obj, nCount, sizeof(obj[0]), SortValueDef);
    return r;
}

DWORD SListValue::EnumFieldsOf(DWORD eSharedType, LPSTR* ppFieldNames, DWORD countIn, DWORD len4Name)
{
    SFullVarInfo* obj = Obj(0);
    UINT nCount = Count();
    DWORD count = 0;
    for (UINT n = 0; n < nCount; ++n, ++obj)
    {
        if (eSharedType != obj->structType)
            continue;
        if (count == countIn)
            break;
        strcpy_s(ppFieldNames[count], len4Name, obj->pszName);
        ++count;
    }
    return count;
}
//////////////////////////////////////////////////////////////////////////
SVarInfo* GetFieldInfo(DWORD eSharedType, LPCSTR pszFieldName)
{
    return _theList.Find(pszFieldName, eSharedType);
}

DWORD EnumFieldsOf(DWORD eSharedType, LPSTR* ppFieldNames, DWORD countIn, DWORD len4Name)
{
    return _theList.EnumFieldsOf(eSharedType, ppFieldNames, countIn, len4Name);
}

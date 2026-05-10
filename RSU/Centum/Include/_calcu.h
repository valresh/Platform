#pragma once
#include "./Y_export.h"
#include "math.h"
#include <string>
#include <Alarms.h>
#include <YBridge2SysParam.h>


//#pragma warning( disable : 4244)
class CRefAny;
struct Y_DATA_API CAlg
{
    template<typename T>
    class KOnExitCall
    {
        typedef void (T::* pfOnExit)();
        pfOnExit method;
        T* owner;
    public:
        KOnExitCall(T* o, pfOnExit m) : owner(o), method(m)
        {
        }
        ~KOnExitCall()
        {
            (owner->*method)();
        }
    };


    enum EOpeGuides
    {
        OG = 1,
    };
    enum ERequest
    {
        RQ = 1,
    };
    static CAlg* pFirst;
    static CAlg* pLast;
    CAlg* pNext;
    char* Name;
    CY_Base* m_pOwner;
    CAlg(char* Name);
    virtual void GetParam(KYBridge2SysParam& params);
    virtual void Prepare0() {}
    virtual void Prepare1() {}
    static CAlg* Find(char* Name);
    virtual void Work(struct CCALCU* W);
    virtual void Work(struct C_SFCSW* W, double dt);
    virtual void Work(struct C_SFCPB* W, double dt);
    virtual void Work(struct C_SFCAS* W, double dt);
    virtual void Work(struct COPSFC* W, double dt);
    virtual void Work(struct COPSBL* W, double dt);
    virtual void Work(struct C_UTPB* W, double dt);
    virtual void Work(struct C_UTSW* W, double dt);
    virtual void OnStatusChange(struct C_SFCSW* W, double dt);
    virtual void OnStatusChange(struct C_SFCPB* W, double dt);
    virtual void OnStatusChange(struct C_SFCAS* W, double dt);
    virtual void OnStatusChange(struct COPSFC* W, double dt);
    virtual void OnStatusChange(struct COPSBL* W, double dt);
    virtual void OnStatusChange(struct C_UTPB* W, double dt);
    virtual void OnStatusChange(struct C_UTSW* W, double dt);
    virtual void SetStep(int stepNo);
    virtual int SaveState();
    virtual int RestoreState(char* StrName);
    virtual int StateSave(struct IYokoStateSer* pStateSer);
    virtual int StateRestore(struct IYokoStateSer* pStateSer);
    int StateSaveImpl(struct IYokoStateSer* pStateSer, void* pW, DWORD size);
    int StateRestoreImpl(struct IYokoStateSer* pStateSer, void* pW, DWORD size);
    void REQUEST(ERequest eog, int n);
    void OPEGUIDE(EOpeGuides eog, int n, int nFcs = -1);
    void OPEGUIDE(LPCSTR pszFormatString, int color = 0, ...);
    void DIALOGUE(LPCSTR pszMsg, LPCSTR txt, int color, LPCSTR pszHeader, LPCSTR pszVariable);
    int GETTIME(int& HOUR, int& MIN, int& SEC);
    void GETDATE(int& YEAR, int& MONTH, int& DAY);
    void LOCALTIME(double gmt, int& YEAR, int& MONTH, int& DAY, int& HOUR, int& MIN, int& SEC);
    double TIMEGMT();
    void MESSAGE(LPCSTR pTxt);
    void HISTORY(LPCSTR pszFormatString, ...);
    int TIMEMS();
    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    std::string FORMAT(LPSTR format, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
    {
        std::string res;
        char buf[128] = { 0 };
        char tmp[128] = { 0 };
        strcpy_s(tmp, format);
        LPSTR p = tmp;
        while (*p)
        {
            *p = tolower(*p);
            p++;
        }
        sprintf_s(buf, tmp, t1, t2, t3, t4, t5);
        res = buf;
        return res;
    }
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    std::string FORMAT(LPSTR format, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
    {
        std::string res;
        char buf[128] = { 0 };
        char tmp[128] = { 0 };
        strcpy_s(tmp, format);
        LPSTR p = tmp;
        while (*p)
        {
            *p = tolower(*p);
            p++;
        }
        sprintf_s(buf, tmp, t1, t2, t3, t4, t5, t6);
        res = buf;
        return res;
    }
    double fabs(CRefAny& v);
    double fabs(double& v);
    double power(double v, double y);
    void sswrite() {}
    void ssread() {}
    void oneshot(CRefAny& obj);
    double dlimit(double v, double low, double hi);
    double llimit(double v, double low, double hi);
    double lmin(double l, double r);
    double dmax(int n, ...);
    double lmax(int n, ...);
    double dmin(int n, ...);
    double dmax(double v1, double v2);
    double dmin(double v1, double v2);
    double lmax(double v1, double v2);
    double dmax(double v1, double v2, double v3);
    int notInt(int v);
    template<typename T>
    void QUIT(T* W)
    {
        W->BSTS = T::_BSTS::STOP;
    }
    double snum(CRefAny& v);
    int len(CRefAny& v);
    void ExecLC(CRefAny& tag);
};

template<size_t _SIZE>
void operator >> (std::string& inStr, char(&outStr)[_SIZE])
{
    size_t l = inStr.length();
    if (l >= _SIZE)
        l = _SIZE;
    else
        outStr[l] = 0;
    CopyMemory(outStr, inStr.c_str(), l);
    //outStr[l] = 0;
}


class Y_DATA_API CRefAny
{
    static CRefAny* pFirst;
    static CRefAny* pLast;
    CRefAny* pNext;
    char* Name;
    char m_Field[24];
    char m_NameObj[20];

    template<typename T>
    struct Y_DATA_API SAnyType
    {
        bool bNoUseLocalValue;
        typedef T _type;
        T value;
        T* pVar;
        USHORT size;
        template<size_t _SIZE>
        void operator = (char(&pThr)[_SIZE])
        {
            if (!pVar)
                return;
            CopyMemory(pVar, pThr, _SIZE);
            pVar[_SIZE] = 0;
        }
        void InitC(char* pThr, USHORT maxLen)
        {
            pVar = pThr;
            bNoUseLocalValue = true;
            size = maxLen;
        }
        void operator = (LPSTR V)
        {
            if (!pVar || !V)
                return;
            if (size > 0)
            {
                size_t l = strlen(V);
                if (l < size)
                    strncpy_s(pVar, size, V, size - 1);
                else
                    CopyMemory(pVar, V, size);
                return;
            }
#pragma warning( push )
#pragma warning( disable: 4996)
            strcpy(pVar, V);
#pragma warning( pop )
        }
        void Init(T* pThr)
        {
            pVar = pThr;
            if (pVar)
                value = *pVar;
        }
        operator T& ()
        {
            if (pVar != 0)
                value = *pVar;
            return value;
        }
        void operator = (T V)
        {
            value = V;
            if (pVar != 0)
                *pVar = value;
        }
        SAnyType<T>& operator = (const SAnyType<T>& Ref)
        {
            if (Ref.pVar != 0)
                value = *Ref.pVar;
            else
                value = Ref.value;
            if (pVar)
                *pVar = value;
            return *this;
        }
        void NotifyOnAssign(struct CY_Base* pObj);
    };
    struct
    {
        SAnyType<int>    discret;
        SAnyType<double> analog;
        SAnyType<BYTE>   W;
        SAnyType<char>   S;
    }m_Value;
    bool m_bNotifyOnAssign;
    CY_Base* m_pObj;
public:
    char m_Type;
public:
    CRefAny(char* Name = "");
    CRefAny(char type);
    //CRefAny( const CRefAny &thr );
    CRefAny& ReLinkToField(LPCSTR pszField = NULL);
    CRefAny& AssignToObj(LPCSTR pszObj);
    CRefAny& AssignToObj(CRefAny obj);
    CRefAny G(int x, int y = -1);
    CRefAny operator[](int x);
    static void InitAll();
    void GetParam(KYBridge2SysParam& params);
    void operator = (CRefAny Ref);
    bool operator !();
    template<typename C>
    bool operator == (C thr)
    {
        switch (m_Type)
        {
        case 'W':
            return m_Value.W == thr;
        case 'D':
            return m_Value.analog == thr;
        }
        return m_Value.discret == thr;
    }
    template<typename C>
    bool operator <= (C thr)
    {
        switch (m_Type)
        {
        case 'W':
            return m_Value.W <= thr;
        case 'D':
            return m_Value.analog <= thr;
        }
        return m_Value.discret <= thr;
    }
    template<typename C>
    bool operator < (C thr)
    {
        switch (m_Type)
        {
        case 'W':
            return m_Value.W < (BYTE)thr;
        case 'D':
            return m_Value.analog < (double)thr;
        }
        return m_Value.discret < (int)thr;
    }
    template<typename C>
    bool operator >= (C thr)
    {
        switch (m_Type)
        {
        case 'W':
            return m_Value.W >= thr;
        case 'D':
            return m_Value.analog >= thr;
        }
        return m_Value.discret >= thr;
    }
    template<typename C>
    bool operator != (C thr)
    {
        switch (m_Type)
        {
        case 'W':
            return m_Value.W != (BYTE)thr;
        case 'D':
            return m_Value.analog != (double)thr;
        }
        return m_Value.discret != (int)thr;
    }
    template<typename C>
    bool operator > (C thr)
    {
        switch (m_Type)
        {
        case 'W':
            return m_Value.W > (BYTE)thr;
        case 'D':
            return m_Value.analog > (double)thr;
        }
        return m_Value.discret > (int)thr;
    }
    template<typename C>
    void operator =(C V)
    {
        switch (m_Type)
        {
        case 'W':
            m_Value.W = (BYTE)V;
            if (m_bNotifyOnAssign && m_pObj)
                m_Value.W.NotifyOnAssign(m_pObj);
            break;
        case 'D':
            m_Value.analog = (double)V;
            break;
        case 'I':
            m_Value.discret = (int)V;
            break;
        }
    }
    template<>
    void operator =(LPCSTR psz)
    {
        if ('S' != m_Type)
        {
            TestAssignMode(psz);
            return;
        }
        m_Value.S = (LPSTR)psz;
    }
    template<size_t _SIZE>
    void operator =(char(&V)[_SIZE])
    {
        if ('S' != m_Type)
        {
            TestAssignMode(V);
            return;
        }
        m_Value.S = V;
    }
    template<>
    bool operator == (LPCSTR thr)
    {
        if ('S' == m_Type)
            return !strcmp((LPSTR)m_Value.S.pVar, thr) ? true : false;
        return CompareNoString(thr);
    }
    template<>
    bool operator != (LPCSTR thr)
    {
        if ('S' == m_Type)
            return strcmp((LPSTR)m_Value.S.pVar, thr) ? true : false;
        return !CompareNoString(thr);
    }
    template<typename C>
    operator C()
    {
        switch (m_Type)
        {
        case 'W':
            return m_Value.W;
        case 'D':
            return (C)m_Value.analog;
        }
        return m_Value.discret;
    }
    LPSTR GetText();
    void ExecLC();
protected:
    void Init();
    void TestAssignMode(LPCSTR pszMode);
    bool CompareNoString(LPCSTR thr);
};

template<size_t _S>
void cat(char(&outStr)[_S], CRefAny v)
{
    strcat_s(outStr, v.GetText());
}

template<size_t _S>
void cat(char(&outStr)[_S], LPCSTR v1, CRefAny& v2)
{
    strcat_s(outStr, v1);
    strcat_s(outStr, v2.GetText());
}

template<size_t _S>
void cat(char(&outStr)[_S], LPCSTR v)
{
    strcat_s(outStr, v);
}


double Y_DATA_API operator * (double thl, CRefAny& thr);
double Y_DATA_API operator * (CRefAny& thl, double thr);
double Y_DATA_API operator * (CRefAny& thl, CRefAny& thr);
double Y_DATA_API operator - (double thl, CRefAny& thr);
double Y_DATA_API operator - (CRefAny& thl, double thr);
CRefAny Y_DATA_API operator - (CRefAny& thl, CRefAny& thr);
bool Y_DATA_API operator >= (CRefAny& thl, double thr);
bool Y_DATA_API operator >= (CRefAny& thl, CRefAny& thr);
bool Y_DATA_API operator >= (double thl, CRefAny& thr);
bool Y_DATA_API operator > (double thl, CRefAny& thr);
bool Y_DATA_API operator < (double thl, CRefAny& thr);
bool Y_DATA_API operator <= (CRefAny& thl, double thr);
bool Y_DATA_API operator <= (CRefAny& thl, CRefAny& thr);
bool Y_DATA_API operator <= (double thl, CRefAny& thr);
CRefAny Y_DATA_API operator + (CRefAny& thl, CRefAny& thr);
double Y_DATA_API operator + (double thl, CRefAny& thr);
CRefAny Y_DATA_API operator + (CRefAny& thl, double thr);
double Y_DATA_API operator / (CRefAny& thl, double thr);
double Y_DATA_API operator / (double thl, CRefAny& thr);
CRefAny Y_DATA_API operator / (CRefAny& thl, CRefAny thr);
bool Y_DATA_API operator != (int thl, CRefAny& thr);
bool Y_DATA_API operator != (double thl, CRefAny& thr);
CRefAny Y_DATA_API operator & (CRefAny& thl, int thr);
CRefAny Y_DATA_API operator | (CRefAny& thl, int thr);
bool Y_DATA_API operator == (double thl, CRefAny& thr);

bool Y_DATA_API operator && (CRefAny& thl, bool thr);
bool Y_DATA_API operator || (CRefAny& thl, bool thr);

struct CCond
{
    int ID;
    static CCond* pFirst;
    static CCond* pLast;
    CCond* pNext;
    char* Name;
    void* pVar;
    char TypeVar;
    CCond(char* Name);
    operator int();
    void operator = (BYTE V);
    void Init();
    static void InitAll();
};

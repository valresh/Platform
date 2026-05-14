//#include <filesystem>
//#include <inttypes.h>
//#include "math.h"
//#include "stdio.h"
//#include "err.h"
#include "CommProc.h"
#include "Param.h"
#include "Defects.h"
#include "BaseModel.h"
#include "Prop.h"

//#include "crossstring.h"

//
//QParams QParams::Params[MAX__PARAMS];
//CMem<QParams,SIZE_PARAMS_BLK,K_PARAMS_BLK> * QParams::pParams = NULL;
QParams QParams::Params[MAX__PARAMS];
int QParams::kParams = 0;
//CRITICAL_SECTION QParams::CS;
std::recursive_mutex QParams::CS;
static bool InitCS = true;
static bool Locked = true;
bool QParams::SaveAllParams = false;
bool QParams::GetParamsList = false;
bool QParams::Test_Default = false;
bool QParams::ReadProps = false;
char* QParams::TabPrefix = NULL;
//extern bool RunSimulator;

QParams* QParams::pFirstChanged = NULL;

CLockParams::CLockParams()
{
    kParams = &QParams::kParams;
    QParams::LockParams();
}

CLockParams::~CLockParams()
{
    QParams::FreeParams();
}

void QParams::LockParams()
{
    CS.lock();
    Locked = true;
    kParams = 0;
}

void QParams::FreeParams()
{
    ASS(Locked)
    kParams = 0;
    CS.unlock();
}

void QParams::Reset()
{
    ASS(Locked)
    kParams = 0;
}

void QParams::WasChanged()
{
    // Нет ли уже в списке
    QParams* pP = pFirstChanged;
    while (pP)
    {
        if (pP->Addr == Addr)
            return;
        pP = pP->pNextChanged;
    }
    QParams* pSave = new QParams();
    pSave->pModel = pModel;
    lstrcpy(pSave->ParamName, ParamName);
    pSave->Type = Type;
    pSave->Len = Len;
    pSave->Addr = Addr;
    pSave->pNextChanged = pFirstChanged;
    pFirstChanged = pSave;
}


QParams& QParams::Get()
{
    ASS(kParams < MAX__PARAMS)
    ASS(Locked)
    return Params[kParams++];
}

QParams& QParams::GetLast()
{
    ASS(kParams > 0)
    if (kParams == 0)
        kParams = 1;
    ASS(kParams < MAX__PARAMS)
    if (kParams > MAX__PARAMS)
        kParams = MAX__PARAMS;
    ASS(Locked)
    return Params[kParams - 1];
}

bool QParams::AddParmToList(const char* Name)
{
    if (SaveAllParams && Name[0] == '#' && !GetParamsList)
        return false;
    if (ReadProps)
        return false;
    return true;
}

bool QParams::AddPropToList(const char* Name)
{
    if (ReadProps)
        return true;
    if (!SaveAllParams)
        return true;
    return false;
}

QParams::QParams()
    : pDefValue(NULL)
    , m_bNoSave(false)
{
    Type = 0;
    Addr = NULL;
    Len = 0;
    dwFlags = 0;
    pModel = NULL;
    pNextParam = NULL;
    extCount = 0;
    pextVals = NULL;
    Visible = 0;
    pNextChanged = NULL;
    AbsAddr = false;
    Class = ' ';
    pVarName = NULL;
    pExternDescr = NULL;
    Used = NULL;
    pShowParam = NULL;
    dwFlags = 0;
}

QParams::QParams(struct IBaseModel* _pModel,
                 char* _ParamName, QParams& par)
    : Type(par.Type)
    , Addr(par.Addr)
    , Len(par.Len)
    , pDefValue(par.pDefValue)
    , m_bNoSave(false)
    , Used(par.Used)
    , pShowParam(par.pShowParam)
{
    pModel = _pModel;
    dwFlags = par.dwFlags;
    if (lstrlen(_ParamName) > MAX_PARM_NAME)
        _ParamName[MAX_PARM_NAME] = 0;
    strncpy(ParamName, _ParamName, sizeof(ParamName));
    pNextParam = NULL;
    extCount = 0;
    pextVals = NULL;
    pModelDescr = NULL;
    pHelpInfo = NULL;
    Visible = 0;
    pNextChanged = NULL;
    AbsAddr = false;
    Property = false;
    Class = ' ';
    pVarName = NULL;
    pExternDescr = NULL;
}

QParams::~QParams()
{
}

bool QParams::IsFixed(void* addr)
{
    return false;
}

int QParams::Copy(BYTE* mem, void*& addr)
{
    if (Type == 'D' || Type == 'K' || Type == 'I' || Type == 'B' || Type == 'H' || Type == 'J' || Type == 'X')
    {
        addr = Addr;
        memmove(mem, Addr, Len);
        Addr = mem;
        return Len;
    }
    return 0;
}

void QParams::SafeCopyTab(const char* Name)
{
    if (TabPrefix == NULL)
    {
        SafeCopy(Name);
        return;
    }
    ASS(Name != NULL);
    int Lpref = lstrlen(TabPrefix);
    int L = lstrlen(Name);
    if (L + Lpref < MAX_PARM_NAME)
    {
        strcpy(ParamName, TabPrefix);
        strcat(ParamName, Name);
        return;
    }
    L = MAX_PARM_NAME - 3 - Lpref;
    strcpy(ParamName, TabPrefix);
    memmove(ParamName + Lpref, Name, L);
    memmove(ParamName + L + Lpref, "..", 3);
}

void QParams::SafeCopy(const char* Name)
{
    if (Name == NULL)
        return;
    int L = lstrlen(Name);
    if (L < MAX_PARM_NAME)
    {
        strcpy(ParamName, Name);
        return;
    }
    L = MAX_PARM_NAME - 3;
    memmove(ParamName, Name, L);
    memmove(ParamName + L, "..", 3);
}

void QParams::SetAddr(struct IBaseModel* _pModel,
                      char* _ParamName,
                      void* _Addr, char _Type, int _Len)
{
    pModel = _pModel;
    SafeCopy(_ParamName);
    Type = _Type;
    Addr = _Addr;
    Len = _Len;
    extCount = 0;
    if (pextVals)
        double pextVals;
    pextVals = NULL;
    pModelDescr = NULL;
    Used = NULL;
    pShowParam = NULL;
    pHelpInfo = NULL;
    Class = 'P';
}

void QParams::SetDefect(struct IBaseModel* _pModel,
                        char* _ParamName,
                        void* pDefect)
{
    pModel = _pModel;
    SafeCopy(_ParamName);
    Type = 'W';
    Addr = pDefect;
    Len = 0;
    extCount = 0;
    if (pextVals)
        double pextVals;
    pextVals = NULL;
    pModelDescr = NULL;
    Used = NULL;
    pShowParam = NULL;
    pHelpInfo = NULL;
    pExternDescr = NULL;
    Class = 'D';
}

//
void QParams::Set(struct IBaseModel* _pModel,
                  char* _ParamName,
                  double& pD, int _Len)
{
    pModel = _pModel;
    SafeCopy(_ParamName);
    Type = 'D';
    Addr = &pD;
    Len = _Len;
    ASS(Len == 8)
    extCount = 0;
    if (pextVals)
        double pextVals;
    pextVals = NULL;
    pModelDescr = NULL;
    Used = NULL;
    pShowParam = NULL;
    pExternDescr = NULL;
    pHelpInfo = NULL;
    Class = 'P';
}
//
void QParams::Set(struct IBaseModel* _pModel,
                  char* _ParamName,
                  float& pF, int _Len)
{
    pModel = _pModel;
    SafeCopy(_ParamName);
    Type = 'K';
    Addr = &pF;
    Len = _Len;
    ASS(Len == 4)
    extCount = 0;
    if (pextVals)
        double pextVals;
    pextVals = NULL;
    pModelDescr = NULL;
    Used = NULL;
    pShowParam = NULL;
    pExternDescr = NULL;
    pHelpInfo = NULL;
    Class = 'P';
}
//
void QParams::Set(struct IBaseModel* _pModel,
                  char* _ParamName,
                  short& pH, int _Len)
{
    pModel = _pModel;
    SafeCopy(_ParamName);
    Type = 'H';
    Addr = &pH;
    Len = _Len;
    ASS(Len == 2)
    extCount = 0;
    if (pextVals)
        double pextVals;
    pextVals = NULL;
    pModelDescr = NULL;
    Used = NULL;
    pShowParam = NULL;
    pExternDescr = NULL;
    pHelpInfo = NULL;
    Class = 'P';
}
//
void QParams::Set(struct IBaseModel* _pModel,
                  char* _ParamName,
                  struct SModePole& mode, int _Len)
{
    pModel = _pModel;
    SafeCopy(_ParamName);
    Type = 'O';
    Addr = &mode;
    Len = _Len;
    extCount = 0;
    if (pextVals)
        double pextVals;
    pextVals = NULL;
    pModelDescr = NULL;
    Used = NULL;
    pShowParam = NULL;
    pExternDescr = NULL;
    pHelpInfo = NULL;
    Class = 'P';
}

//
void QParams::Set(struct IBaseModel* _pModel,
                  char* _ParamName,
                  int& pI, int _Len)
{
    pModel = _pModel;
    SafeCopy(_ParamName);
    Type = 'I';
    Addr = &pI;
    Len = _Len;
    extCount = 0;
    if (pextVals)
        double pextVals;
    pextVals = NULL;
    ASS(Len == 4)
    pModelDescr = NULL;
    Used = NULL;
    pShowParam = NULL;
    pExternDescr = NULL;
    pHelpInfo = NULL;
    Class = 'P';
}

//
void QParams::Set(struct IBaseModel* _pModel,
                  char* _ParamName,
                  unsigned int& pI, int _Len)
{
    pModel = _pModel;
    SafeCopy(_ParamName);
    Type = 'I';
    Addr = &pI;
    Len = _Len;
    extCount = 0;
    if (pextVals)
        double pextVals;
    pextVals = NULL;
    pModelDescr = NULL;
    Used = NULL;
    pShowParam = NULL;
    pExternDescr = NULL;
    pHelpInfo = NULL;
    ASS(Len == 4)
    Class = 'P';
}

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

void QParams::SetE(struct IBaseModel* _pModel,
                   char* _ParamName,
                   unsigned int& pI, int _Len, int C /*= 0*/, SParmStrVal* pVals /*= NULL*/)
{
    pModel = _pModel;
    SafeCopy(_ParamName);
    Type = 'I';
    Addr = &pI;
    Len = _Len;
    pModelDescr = NULL;
    Used = NULL;
    pShowParam = NULL;
    pExternDescr = NULL;
    pHelpInfo = NULL;
    ASS(Len == 4);
    int Max = 10;
    if (pextVals == NULL)
        pextVals = new SParmStrVal[Max];
    C = min(C, Max);
    for (extCount = 0; extCount < C; extCount++)
    {
        strncpy(pextVals[extCount].szName, pVals[extCount].szName, sizeof(pextVals[extCount].szName));
        pextVals[extCount].iValue = pVals[extCount].iValue;
    }
}
void QParams::SetE(struct IBaseModel* _pModel,
                   char* _ParamName,
                   unsigned char& pE, int _Len, int C /*= 0*/, SParmStrVal* pVals /*= NULL*/)
{
    pModel = _pModel;
    SafeCopy(_ParamName);
    Type = 'F';
    Addr = &pE;
    Len = _Len;
    pModelDescr = NULL;
    Used = NULL;
    pShowParam = NULL;
    pExternDescr = NULL;
    pHelpInfo = NULL;
    ASS(Len == 1);
    int Max = 10;
    if (pextVals == NULL)
        pextVals = new SParmStrVal[Max];
    C = min(C, Max);
    for (extCount = 0; extCount < C; extCount++)
    {
        strncpy(pextVals[extCount].szName, pVals[extCount].szName, sizeof(pextVals[extCount].szName));
        pextVals[extCount].eValue = pVals[extCount].eValue;
    }
}
//
void QParams::Set(struct IBaseModel* _pModel,
                  char* _ParamName,
                  __int64& pI, int _Len)
{
    pModel = _pModel;
    SafeCopy(_ParamName);
    Type = 'N';
    Addr = &pI;
    Len = _Len;
    extCount = 0;
    if (pextVals)
        double pextVals;
    pextVals = NULL;
    pModelDescr = NULL;
    Used = NULL;
    pShowParam = NULL;
    pExternDescr = NULL;
    pHelpInfo = NULL;
    ASS(Len == 8)
    Class = 'P';
}
//

void QParams::Set(struct IBaseModel* _pModel,
                  char* _ParamName,
                  bool& pB, int _Len)
{
    pModel = _pModel;
    SafeCopy(_ParamName);
    Type = 'B';
    Addr = &pB;
    Len = _Len;
    extCount = 0;
    if (pextVals)
        double pextVals;
    pextVals = NULL;
    pModelDescr = NULL;
    Used = NULL;
    pShowParam = NULL;
    pExternDescr = NULL;
    pHelpInfo = NULL;
    ASS(Len == 1)
    Class = 'P';
}
//
void QParams::Set(struct IBaseModel* _pModel,
                  char* _ParamName,
                  char* pStr, int _Len)
{
    pModel = _pModel;
    SafeCopy(_ParamName);
    Type = 'S';
    Addr = pStr;
    Len = _Len;
    extCount = 0;
    if (pextVals)
        double pextVals;
    pextVals = NULL;
    pModelDescr = NULL;
    Used = NULL;
    pShowParam = NULL;
    pExternDescr = NULL;
    pHelpInfo = NULL;
    Class = 'P';
}

void QParams::Set(struct IBaseModel* _pModel,
                  char* _ParamName, CStr& Str)
{
    pModel = _pModel;
    SafeCopy(_ParamName);
    Type = 'U';
    Addr = &Str;
    Len = 0;
    extCount = 0;
    if (pextVals)
        double pextVals;
    pextVals = NULL;
    pExternDescr = NULL;
    pModelDescr = NULL;
    Used = NULL;
    pShowParam = NULL;
    pHelpInfo = NULL;
    Class = 'P';
}

void QParams::Set(struct IBaseModel* _pModel,
                  char* _ParamName, IFreeParam& IParam, int _Len)
{
    pModel = _pModel;
    SafeCopy(_ParamName);
    Type = 'X';
    Addr = &IParam;
    Len = _Len;
    extCount = 0;
    if (pextVals)
        double pextVals;
    pextVals = NULL;
    pExternDescr = NULL;
    pModelDescr = NULL;
    Used = NULL;
    pShowParam = NULL;
    pHelpInfo = NULL;
    Class = 'P';
}

void QParams::Set(struct IBaseModel* _pModel,
                  char* _ParamName, struct CGrafParams& Param, int _Len)
{
    pModel = _pModel;
    SafeCopy(_ParamName);
    Type = 'G';
    Addr = &Param;
    Len = _Len;
    extCount = 0;
    if (pextVals)
        double pextVals;
    pextVals = NULL;
    pExternDescr = NULL;
    pModelDescr = NULL;
    Used = NULL;
    pShowParam = NULL;
    pHelpInfo = NULL;
    Class = 'P';
}

//
void QParams::Set(struct IBaseModel* _pModel,
                  char* _ParamName,
                  unsigned char& pU, int _Len)
{
    pModel = _pModel;
    SafeCopy(_ParamName);
    Type = 'F';
    Addr = &pU;
    Len = _Len;
    ASS(Len == 1)
    extCount = 0;
    if (pextVals)
        double pextVals;
    pextVals = NULL;
    pModelDescr = NULL;
    Used = NULL;
    pShowParam = NULL;
    pExternDescr = NULL;
    pHelpInfo = NULL;
    Class = 'P';
}
//
void QParams::Set(struct IBaseModel* _pModel,
                  char* _ParamName,
                  char& pC, int _Len)
{
    pModel = _pModel;
    SafeCopy(_ParamName);
    Type = 'C';
    Addr = &pC;
    Len = _Len;
    ASS(Len == 1)
    extCount = 0;
    if (pextVals)
        double pextVals;
    pextVals = NULL;
    pModelDescr = NULL;
    Used = NULL;
    pShowParam = NULL;
    pExternDescr = NULL;
    pHelpInfo = NULL;
    Class = 'P';
}
//
// double ConvValue(double X, char Value[])
// {
//   char* p = Value;
//   char* post = NULL;
//   while (*p)
//   {
//     if (*p >= '0' && *p <= '9')
//       post = p;
//     p++;
//   }
//   if (post == NULL)
//     return X;
//   post++;
//   if (*post == 0)
//     return X;
//   auto ConvPath = std::filesystem::path(SYSTEM_PATH) / "INI" / "Convert.csv";
//   FILE* F = fopen(ConvPath.string().c_str(), "rt");
//   if (F == NULL)
//     return X;
//   char Str[256];
//   fgets(Str, 255, F);
//   while (fgets(Str, 255, F))
//   {
//     Str[strcspn(Str, "\r\n")] = 0;
//     char* Name = Str;
//     char* A = strchr(Name, ';');
//     if (A == NULL)
//       continue;
//     *A = 0;
//     if (lstrcmp(Name, post) == 0)
//     {
//       A++;
//       char* B = strchr(A, ';');
//       if (B == NULL)
//         continue;
//       *B = 0;
//       B++;
//       double a = cross::locale::safe_atof(A);
//       double b = cross::locale::safe_atof(B);
//       fclose(F);
//       return a * X + b;
//     }
//   }
//   fclose(F);
//   return X;
// }

bool QParams::SetValue(char Value[])
{
    bool bUpdate = false;
    //
    switch (Type)
    {
    case 'D':
    {
        double dVal = atof(Value);
        //
        bUpdate = *(double*)Addr != dVal;
        if (bUpdate)
            *(double*)Addr = dVal;
    }
    break;
    case 'K':
    {
        float dVal = atof(Value);
        bUpdate = *(float*)Addr != dVal;
        if (bUpdate)
            *(float*)Addr = dVal;
    }
    break;
    case 'I':
    {
        int iVal = atoi(Value);
        bUpdate = *(int*)Addr != iVal;
        if (bUpdate)
            *(int*)Addr = iVal;
    }
    break;
    case 'H':
    {
        short iVal = atoi(Value);
        bUpdate = *(short*)Addr != iVal;
        if (bUpdate)
            *(short*)Addr = iVal;
    }
    break;
    case 'J':
    {
        BYTE iVal = atoi(Value);
        bUpdate = *(BYTE*)Addr != iVal;
        if (bUpdate)
            *(BYTE*)Addr = iVal;
    }
    break;
    case 'B':
    {
        bool bVal = false;
        if ( Value[0] == 'Y' || Value[0] == 'y' || Value[0] == '1')
            bVal = true;
        bUpdate = *(bool*)Addr != bVal;
        if (bUpdate)
            *(bool*)Addr = bVal;
    }
    break;
    case 'S':
    {
        int L = lstrlen(Value);
        ASS(L < Len);
        bUpdate = strcmp((char*)Addr, Value) != 0;
        if (bUpdate)
            lstrcpy((char*)Addr, Value);
    }
    break;
    case 'F':
    {
        unsigned char iVal = atoi(Value);
        bUpdate = *(unsigned char*)Addr != iVal;
        if (bUpdate)
            *(unsigned char*)Addr = iVal;
    }
    break;
    case 'C':
    {
        char cVal = Value[0];//atoi ( Value );
        bUpdate = *(char*)Addr != cVal;
        if (bUpdate)
            *(char*)Addr = cVal;
    }
    break;
    case U'У':
    case 'O':
        break;
    case 'U':
    {
        CStr* pStr = (CStr*)Addr;
        bUpdate = strcmp((char*)*pStr, Value) != 0;
        if (bUpdate)
            *pStr = Value;
    }
    break;
    case 'X':
    {
        IFreeParam* pIP = (IFreeParam*)Addr;
        bUpdate = pIP->SetValue(Value); // Update внутри метода SetValue
    }
    break;
    case 'L':
    {
        int iVal = atoi(Value);
        bUpdate = *(int*)Addr != iVal;
        if (bUpdate)
        {
            if (pDefValue == NULL)
            {
                *(int*)Addr = iVal;
                break;
            }
            if (iVal < 0)
                iVal = 0;
            char* P = (char*)pDefValue;
            for (int n = 0; n <= iVal; n++)
            {
                char* Pn = strchr(P, ';');
                if (Pn == NULL)
                {
                    iVal = n - 1;
                    break;
                }
                P = Pn + 1;
            }
            if (iVal < 0)
                iVal = 0;
            *(int*)Addr = iVal;
        }
    }
    break;
    case 'W':
    {
        // CDef* pDef = (CDef*)Addr;
        // bool Fire = false;
        // if (cross::string::starts_with(Value, U'Д') || cross::string::starts_with(Value, U'д') || Value[0] == 'Y' || Value[0] == 'y' || Value[0] == '1')
        //   Fire = true;
        // bool FireDefect(CDef * pDef, BYTE Mask);
        // if (Fire)
        //   FireDefect(pDef, 0);
    }
    break;
    default:// неизвестный тип данных
        ASS(FALSE);
        break;
        //      throw "Неизвестный тип данных";
    }
    if (bUpdate)
        WasChanged();
    //
    return bUpdate;
}
//

bool QParams::Out(char Txt[256])
{
    if (Addr < (void*)0x1000)
    {
        strcpy(Txt, "????");
        return false;
    }
    if (Used)
        *Used = true;
    switch (Type)
    {
    case 'D':
    {
        double V;
        if (pShowParam)
            V = pShowParam->Show_Dbl(this);
        else
            V = *(double*)Addr;
        if (!finite(V))
        {
            lstrcpy(Txt, "#");
            break;
        }
        if (V > 1e10)
        {
            lstrcpy(Txt, ">");
            break;
        }
        if (V < -1e10)
        {
            lstrcpy(Txt, "<");
            break;
        }
        if (ParamName[0] == '#' && ParamName[1] == '%')
            V *= 100.;
        if (V == 0.)
            lstrcpy(Txt, "0");
        else
            if (fabs(V) < 1e-5)
                snprintf(Txt, 128, "%13.5le", V);
            else
                if (fabs(V) < 1e-4)
                    snprintf(Txt, 128, "%11.9lf", V);
                else
                    if (fabs(V) < 1e-3)
                        snprintf(Txt, 128, "%10.8lf", V);
                    else
                        if (fabs(V) < 1e-2)
                            snprintf(Txt, 128, "%9.7lf", V);
                        else
                            if (fabs(V) < 1e-1)
                                snprintf(Txt, 128, "%8.6lf", V);
                            else
                                if (fabs(V) < 1.)
                                    snprintf(Txt, 128, "%7.5lf", V);
                                else
                                    if (fabs(V) < 10.)
                                        snprintf(Txt, 128, "%6.4lf", V);
                                    else
                                        if (fabs(V) < 100.)
                                            snprintf(Txt, 128, "%5.3lf", V);
                                        else
                                            if (fabs(V) < 1000.)
                                                snprintf(Txt, 128, "%5.2lf", V);
                                            else
                                                if (fabs(V) < 10000.)
                                                    snprintf(Txt, 128, "%5.1lf", V);
                                                else
                                                    if (fabs(V) > 10000000.)
                                                        snprintf(Txt, 128, "%e", V);
                                                    else
                                                        snprintf(Txt, 128, "%5.0lf", V);
    }
    break;
    case 'K':
    {
        float V = *(float*)Addr;
        if (ParamName[0] == '#' && ParamName[1] == '%')
            V *= 100.;
        if (V > 1e10)
        {
            Txt[0] = 0;
            break;
        }
        if (V == 0.)
            lstrcpy(Txt, "0");
        else
            if (fabs(V) < 1e-5)
                snprintf(Txt, 128, "%13.5le", V);
            else
                if (fabs(V) < 1e-4)
                    snprintf(Txt, 128, "%11.9lf", V);
                else
                    if (fabs(V) < 1e-3)
                        snprintf(Txt, 128, "%10.8lf", V);
                    else
                        if (fabs(V) < 1e-2)
                            snprintf(Txt, 128, "%9.7lf", V);
                        else
                            if (fabs(V) < 1e-1)
                                snprintf(Txt, 128, "%8.6lf", V);
                            else
                                if (fabs(V) < 1.)
                                    snprintf(Txt, 128, "%7.5lf", V);
                                else
                                    if (fabs(V) < 10.)
                                        snprintf(Txt, 128, "%6.4lf", V);
                                    else
                                        if (fabs(V) < 100.)
                                            snprintf(Txt, 128, "%5.3lf", V);
                                        else
                                            if (fabs(V) < 1000.)
                                                snprintf(Txt, 128, "%5.2lf", V);
                                            else
                                                if (fabs(V) < 10000.)
                                                    snprintf(Txt, 128, "%5.1lf", V);
                                                else
                                                    if (fabs(V) > 10000000.)
                                                        snprintf(Txt, 128, "%e", V);
                                                    else
                                                        snprintf(Txt, 128, "%5.0lf", V);
    }
    break;
    case 'N':
    {
        int64_t V = *(int64_t*)Addr;
        //       snprintf(Txt, 128, PRIi64, V);
    }
    break;
    case 'I':
    {
        int V = *(int*)Addr;
        if ( strcmp ( ParamName, "Единицы измерения" ) == 0 )
        {
            char * MesName( int Mes );
            strcpy ( Txt, MesName( V ));
            char * P = strchr ( Txt, ';' );
            if ( P )
                *P = 0;
            break;
        }

        if (!extCount)
            snprintf(Txt, 128, "%d", V);
        else
        {
            for (int i = 0; i < extCount; i++)
            {
                if (pextVals && pextVals[i].iValue == V)
                {
                    strncpy(Txt, pextVals[i].szName, 128);
                    break;
                }
            }
        }
    }
    break;
    case 'H':
    {
        short V = *(short*)Addr;
        snprintf(Txt, 128, "%d", V);
    }
    break;
    case 'J':
    {
        BYTE J = *(BYTE*)Addr;
        switch (J)
        {
        case 0:
            lstrcpy(Txt, "MAN");
            break;
        case 1:
            lstrcpy(Txt, "AUTO");
            break;
        case 2:
            lstrcpy(Txt, "CAS");
            break;
        }
    }
    break;
    case 'B':
    {
        bool V = *(bool*)Addr;
        if (V)
            lstrcpy(Txt, "Да");
        else
            lstrcpy(Txt, "Нет");
    }
    break;
    case 'S':
    {
        if (Addr)
            lstrcpy(Txt, (char*)Addr);
    }
    break;
    case 'U':
    {
        CStr* pStr = (CStr*)Addr;
        lstrcpy(Txt, (char*)(*pStr));
    }
    break;
    case 'X':
    {
        IFreeParam* pIF = (IFreeParam*)Addr;
        pIF->Out(Txt);
    }
    break;
    case 'F':
    {
        unsigned int V = *(unsigned char*)Addr;
        if (!extCount)
            snprintf(Txt, 128, "%d", V);
        else
        {
            for (int i = 0; i < extCount; i++)
            {
                if (pextVals && pextVals[i].eValue == V)
                {
                    strncpy(Txt, pextVals[i].szName, 128);
                    break;
                }
            }
        }
    }
    break;
    case 'C':
    {
        char V = *(char*)Addr;
        //			snprintf ( Txt, 128, "%d", V );
        Txt[0] = V;
        Txt[1] = 0;
    }
    break;
    case 'O':
    {
    }
    break;
    case U'У':
    {
    }
    break;
    case 'M':
    {
        snprintf(Txt, 128, "##+##");
    }
    break;
    case 'm':
    {
        snprintf(Txt, 128, "##-##");
    }
    break;
    case 'R':
    {
        snprintf(Txt, 128, "$$$$$");
    }
    break;
    case 'W':
    {
        CDef* pDef = (CDef*)Addr;
        if (pDef->Fire)
            snprintf(Txt, 128, "Да");
        else
            snprintf(Txt, 128, "Нет");
    }
    break;
    case 'L':
    {
        int iVal = *(int*)Addr;
        if (pDefValue == NULL)
        {
            snprintf(Txt, 128, "%d", iVal);
            break;
        }
        if (iVal < 0)
            iVal = 0;
        // Выводим индекс текущего значения и список (VM: 04.06.2020)
        // if (lstrlen((char*)pDefValue) < 252)
        // {
        //   sprintf(Txt, "%d\t%s", iVal, (char*)pDefValue);
        //   break;
        // }
        // ...
        char* P = (char*)pDefValue;
        char* Pb = NULL;
        char* Pe = NULL;
        for (int n = 0; n <= iVal; n++)
        {
            Pb = P;
            char* Pn = strchr(P, ';');
            Pe = Pn;
            if (Pn == NULL)
                break;
            P = Pn + 1;
        }
        if (Pb == 0 || Pe == NULL)
        {
            snprintf(Txt, 128, "%d", iVal);
            break;
        }
        int L = (int)(Pe - Pb);
        memmove(Txt, Pb, L);
        Txt[L] = 0;
    }
    break;
    default:// неизвестный тип данных
        Txt[0] = Type;
        Txt[1] = '?';
        Txt[2] = 0;
        break;
        //      throw "Неизвестный тип данных";
    }
    return false;//IsFixed(Addr);
}

void CParmInfo::SetValue(char Value[])
{
    switch (Type)
    {
    case 'D':
    {
        double _D = atof(Value);
        bUpdate = _D != D;
        D = _D;
    }
    break;
    case 'K':
    {
        float _D = atof(Value);
        bUpdate = _D != D;
        D = _D;
    }
    break;
    case 'N':
        break;
    case 'I':
    {
        int _I = atoi(Value);
        bUpdate = _I != I;
        I = _I;
    }
    break;
    case 'H':
    {
        short _I = atoi(Value);
        bUpdate = _I != I;
        I = _I;
    }
    break;
    case 'B':
    {
        bool _B = false;
        BYTE T[4];
        UtfToChar( T, 3, Value );
        if ( T[0] == 0xC4 || T[0] == 0xE4 || T[0] == 'Y' || T[0] == 'y' || T[0] == '1')
            _B = true;
        bUpdate = _B != B;
        B = _B;
    }
    break;
    case 'S':
    {
        int L = lstrlen(Value);
        if (L >= Len)
            return;
        bUpdate = lstrcmp(Str, Value) != 0;
        if (bUpdate)
            Str = Value;
    }
    break;
    case 'U':
    {
        int L = lstrlen(Value);
        if (L >= Len)
            return;
        bUpdate = lstrcmp(Str, Value) != 0;
        if (bUpdate)
            Str = Value;
    }
    break;
    case 'L':
    {
        int _I = atoi(Value);
        bUpdate = _I != I;
        if (bUpdate)
        {
            I = _I;
            if (Default == NULL)
                break;
            if (I < 0)
                I = 0;
            char* P = (char*)Default;
            for (int n = 0; n <= I; n++)
            {
                char* Pn = strchr(P, ';');
                if (Pn == NULL)
                {
                    I = n - 1;
                    break;
                }
                P = Pn + 1;
            }
            if (I < 0)
                I = 0;
        }
    }
    break;
    case U'У':
    case 'T':
    {
        I = 0;
    }
    break;
    default:
        ASS(FALSE);
        break;
    }
}
//
void CParmInfo::Out(char Txt[256])
{
    switch (Type)
    {
    case 'D':
    {
        double V = D;
        if (V > 1e10)
        {
            Txt[0] = '>';
            Txt[1] = 0;
            break;
        }
        if (V < -1e10)
        {
            Txt[0] = '<';
            Txt[1] = 0;
            break;
        }
        if (V == 0.)
            lstrcpy(Txt, "0");
        else
            if (fabs(V) < 1e-5)
                snprintf(Txt, 128, "%13.5le", V);
            else
                if (fabs(V) < 1e-4)
                    snprintf(Txt, 128, "%11.9lf", V);
                else
                    if (fabs(V) < 1e-3)
                        snprintf(Txt, 128, "%10.8lf", V);
                    else
                        if (fabs(V) < 1e-2)
                            snprintf(Txt, 128, "%9.7lf", V);
                        else
                            if (fabs(V) < 1e-1)
                                snprintf(Txt, 128, "%8.6lf", V);
                            else
                                if (fabs(V) < 1.)
                                    snprintf(Txt, 128, "%7.5lf", V);
                                else
                                    if (fabs(V) < 10.)
                                        snprintf(Txt, 128, "%6.4lf", V);
                                    else
                                        if (fabs(V) < 100.)
                                            snprintf(Txt, 128, "%5.3lf", V);
                                        else
                                            if (fabs(V) < 1000.)
                                                snprintf(Txt, 128, "%5.2lf", V);
                                            else
                                                if (fabs(V) < 10000.)
                                                    snprintf(Txt, 128, "%5.1lf", V);
                                                else
                                                    if (fabs(V) > 10000000.)
                                                        snprintf(Txt, 128, "%e", V);
                                                    else
                                                        snprintf(Txt, 128, "%5.0lf", V);
    }
    break;
    case 'K':
    {
        float V = D;
        if (V > 1e10)
        {
            Txt[0] = '?';
            Txt[1] = 0;
            break;
        }
        if (V == 0.)
            lstrcpy(Txt, "0");
        else
            if (fabs(V) < 1e-5)
                snprintf(Txt, 128, "%13.5le", V);
            else
                if (fabs(V) < 1e-4)
                    snprintf(Txt, 128, "%11.9lf", V);
                else
                    if (fabs(V) < 1e-3)
                        snprintf(Txt, 128, "%10.8lf", V);
                    else
                        if (fabs(V) < 1e-2)
                            snprintf(Txt, 128, "%9.7lf", V);
                        else
                            if (fabs(V) < 1e-1)
                                snprintf(Txt, 128, "%8.6lf", V);
                            else
                                if (fabs(V) < 1.)
                                    snprintf(Txt, 128, "%7.5lf", V);
                                else
                                    if (fabs(V) < 10.)
                                        snprintf(Txt, 128, "%6.4lf", V);
                                    else
                                        if (fabs(V) < 100.)
                                            snprintf(Txt, 128, "%5.3lf", V);
                                        else
                                            if (fabs(V) < 1000.)
                                                snprintf(Txt, 128, "%5.2lf", V);
                                            else
                                                if (fabs(V) < 10000.)
                                                    snprintf(Txt, 128, "%5.1lf", V);
                                                else
                                                    if (fabs(V) > 10000000.)
                                                        snprintf(Txt, 128, "%e", V);
                                                    else
                                                        snprintf(Txt, 128, "%5.0lf", V);
    }
    break;
    case 'I':
    {
        int V = I;
        snprintf(Txt, 128, "%d", V);
    }
    break;
    case 'H':
    {
        short V = I;
        snprintf(Txt, 128, "%d", V);
    }
    break;
    case 'B':
    {
        bool V = B;
        if (V)
            lstrcpy(Txt, "Да");
        else
            lstrcpy(Txt, "Нет");
    }
    break;
    case 'S':
    {
        lstrcpy(Txt, (char*)Str);
    }
    break;
    case 'U':
    {
        lstrcpy(Txt, (char*)(Str));
    }
    break;
    //case 'X':
    //  {
    //  IFreeParam * pIF = (IFreeParam*)Addr;
    //  pIF->Out ( Txt );
    //  }
    //break;
    case 'L':
    {
        if (Default == NULL)
        {
            snprintf(Txt, 128, "%d", I);
            break;
        }
        if (I < 0)
            I = 0;
        int K = 0;
        int n = 0;
        char so = 0;
        char s = 0;
        while (1)
        {
            so = s;
            s = Default[n++];
            if (s == 0)
            {
                if (so != ';')
                    K++;
                break;
            }
            if (s == ';')
                K++;
        }
        if (I > K - 1)
            I = K - 1;
        char* P = (char*)Default;
        char* Pb = NULL;
        char* Pe = NULL;
        for (int n = 0; n <= I; n++)
        {
            Pb = P;
            char* Pn = strchr(P, ';');
            Pe = Pn;
            if (Pn == NULL)
                break;
            P = Pn + 1;
        }
        if (Pb == 0 || Pe == NULL)
        {
            snprintf(Txt, 128, "%d", I);
            break;
        }
        int L = (int)(Pe - Pb);
        memmove(Txt, Pb, L);
        Txt[L] = 0;
    }
    break;
    default:// неизвестный тип данных
        Txt[0] = Type;
        Txt[1] = '?';
        Txt[2] = 0;
        break;
    }
}

bool QParams::OutNoM(char Txt[256])
{
    if (Addr < (void*)0x1000)
    {
        strcpy(Txt, "????");
        return false;
    }
    if (Used)
        *Used = true;
    switch (Type)
    {
    case 'D':
    {
        double V;
        if (pShowParam)
            V = pShowParam->Show_Dbl(this);
        else
            V = *(double*)Addr;
        if (ParamName[0] == '#' && ParamName[1] == '%')
            V *= 100.;
        if (V > 1e10)
        {
            Txt[0] = 0;
            break;
        }
        if (V == 0.)
            lstrcpy(Txt, "0");
        else
            if (fabs(V) < 1e-5)
                snprintf(Txt, 255, "%13.5le", V);
            else
                if (fabs(V) < 1e-4)
                    snprintf(Txt, 255, "%11.9lf", V);
                else
                    if (fabs(V) < 1e-3)
                        snprintf(Txt, 255, "%10.8lf", V);
                    else
                        if (fabs(V) < 1e-2)
                            snprintf(Txt, 255, "%9.7lf", V);
                        else
                            if (fabs(V) < 1e-1)
                                snprintf(Txt, 255, "%8.6lf", V);
                            else
                                if (fabs(V) < 1.)
                                    snprintf(Txt, 255, "%7.5lf", V);
                                else
                                    if (fabs(V) < 10.)
                                        snprintf(Txt, 255, "%6.4lf", V);
                                    else
                                        if (fabs(V) < 100.)
                                            snprintf(Txt, 255, "%5.3lf", V);
                                        else
                                            if (fabs(V) < 1000.)
                                                snprintf(Txt, 255, "%5.2lf", V);
                                            else
                                                if (fabs(V) < 10000.)
                                                    snprintf(Txt, 255, "%5.1lf", V);
                                                else
                                                    if (fabs(V) > 10000000.)
                                                        snprintf(Txt, 255, "%e", V);
                                                    else
                                                        snprintf(Txt, 255, "%5.0lf", V);
    }
    break;
    case 'K':
    {
        float V = *(float*)Addr;
        if (ParamName[0] == '#' && ParamName[1] == '%')
            V *= 100.;
        if (V > 1e10)
        {
            Txt[0] = 0;
            break;
        }
        if (V == 0.)
            lstrcpy(Txt, "0");
        else
            if (fabs(V) < 1e-5)
                snprintf(Txt, 255, "%13.5le", V);
            else
                if (fabs(V) < 1e-4)
                    snprintf(Txt, 255, "%11.9lf", V);
                else
                    if (fabs(V) < 1e-3)
                        snprintf(Txt, 255, "%10.8lf", V);
                    else
                        if (fabs(V) < 1e-2)
                            snprintf(Txt, 255, "%9.7lf", V);
                        else
                            if (fabs(V) < 1e-1)
                                snprintf(Txt, 255, "%8.6lf", V);
                            else
                                if (fabs(V) < 1.)
                                    snprintf(Txt, 255, "%7.5lf", V);
                                else
                                    if (fabs(V) < 10.)
                                        snprintf(Txt, 255, "%6.4lf", V);
                                    else
                                        if (fabs(V) < 100.)
                                            snprintf(Txt, 255, "%5.3lf", V);
                                        else
                                            if (fabs(V) < 1000.)
                                                snprintf(Txt, 255, "%5.2lf", V);
                                            else
                                                if (fabs(V) < 10000.)
                                                    snprintf(Txt, 255, "%5.1lf", V);
                                                else
                                                    if (fabs(V) > 10000000.)
                                                        snprintf(Txt, 255, "%e", V);
                                                    else
                                                        snprintf(Txt, 255, "%5.0lf", V);
    }
    break;
    case 'I':
    {
        int V = *(int*)Addr;
        snprintf(Txt, 255, "%d", V);
    }
    break;
    case 'H':
    {
        short V = *(short*)Addr;
        snprintf(Txt, 255, "%d", V);
    }
    break;
    case 'J':
    {
        BYTE J = *(BYTE*)Addr;
        switch (J)
        {
        case 0:
            lstrcpy(Txt, "MAN");
            break;
        case 1:
            lstrcpy(Txt, "AUTO");
            break;
        case 2:
            lstrcpy(Txt, "CAS");
            break;
        }
    }
    break;
    case 'B':
    {
        bool V = *(bool*)Addr;
        if (V)
            lstrcpy(Txt, "Да");
        else
            lstrcpy(Txt, "Нет");
    }
    break;
    case 'S':
    {
        lstrcpy(Txt, (char*)Addr);
    }
    break;
    case 'F':
    {
        unsigned int V = *(unsigned char*)Addr;
        snprintf(Txt, 255, "%d", V);
    }
    break;
    case 'C':
    {
        char V = *(char*)Addr;
        snprintf(Txt, 255, "%d", V);
    }
    break;
    case U'У':
    case 'O':
    {
    }
    break;
    case 'W':
    {
        CDef* pDef = (CDef*)Addr;
        if (pDef->Fire)
            snprintf(Txt, 128, "Да");
        else
            snprintf(Txt, 128, "Нет");
    }
    break;
    default:// неизвестный тип данных
        Txt[0] = Type;
        Txt[1] = '?';
        Txt[2] = 0;
        break;
        //		default:// неизвестный тип данных
        //      throw "Неизвестный тип данных";
    }
    return false;//IsFixed(Addr);
}

void AddSubModelParams(
    char* NameSubModel, struct IBaseModel* pSubModel,
    int& N, struct QParams Params[])
{
    /*******
        struct QParams SubParams[MAX__PARAMS];
        int K = 0;
        if ( pSubModel->GetParams( K, SubParams ))
            return ;
        for ( int n = 0; n < K; n++ )
            {
            ASS(N<MAX__PARAMS)
            Params[N] = SubParams[n];
            ASS(snprintf ( Params[N].ParamName, sizeof(Params[N].ParamName), "%s.%s",
                            NameSubModel, SubParams[n].ParamName )<256);
            N++;
            }
    ******/
}

int CompParams(const void* d1, const void* d2)
{
    QParams* p1 = (QParams*)d1;
    QParams* p2 = (QParams*)d2;
    return lstrcmp(p1->ParamName, p2->ParamName);
}

void Sort(int kParams, QParams Params[])
{
    if (kParams <= 1)
        return;
    qsort(Params, kParams, sizeof(QParams), CompParams);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Нужно ли сохранять параметры в файле?
bool QParams::IsWrite()
{
    if (m_bNoSave)
        return false;
    if (Type == 'T' || Type == 'R' || Type == 'Y' || Type == 'Z' || Type == 'z' || Type == U'У' || Type == 'E')
        return false;
    if (Type == 'X')
    {
        IFreeParam* pIF = (IFreeParam*)Addr;
        return pIF->IsWrite();
    }
    if (ParamName[0] == '#' && !GetParamsList)
        return false;
    if (ParamName[0] == '@')
        return false;
    if (Test_Default)
    {
        if (Addr < (void*)0x1000)
        {
            return false;
        }
        if (Addr != NULL && pDefValue != NULL)
        {
            if (memcmp(Addr, pDefValue, Len) == 0)
                return false;
        }
    }
    /*
      if ( pDefValue )
        {
        if ( Type == 'S' )
                return strcmp((const char*)pDefValue,(const char*)Addr) != 0;
        else
                return memcmp( pDefValue, Addr, Len ) != 0;
        }
    */
    return true;
}
// Нужно ли читать параметры?
bool QParams::IsRead()
{
    if (m_bNoSave)
        return false;
    if (Type == 'T' || Type == 'R' || Type == 'Y' || Type == 'Z' || Type == 'z' || Type == U'У' || Type == 'E')
        return false;
    if (ParamName[0] == '#' && !GetParamsList)
        return false;
    if (Type == 'X')
    {
        IFreeParam* pIF = (IFreeParam*)Addr;
        return pIF->IsWrite();
    }
    if (Addr < (void*)0x1000)
    {
        return false;
    }
    if (IsFixed(Addr))
        return false;
    return true;
}

bool QParams::LastIsTab()
{
    if (kParams <= 0)
        return false;
    if (Params[kParams - 1].Type == 'T')
        return true;
    return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Изменение параметра из диалога
bool QParams::ChangeVal(double Delta)
{
    if (Addr < (void*)0x1000)
    {
        return false;
    }
    WasChanged();
    //if ( IsFixed(Addr) ) return false;
    switch (Type)
    {
    case 'D':
    {
        double V = *(double*)(Addr);
        if (V == 0.)
            V = Delta - 1.;
        else
            V *= Delta;
        *(double*)(Addr) = V;
    }
    break;
    case 'K':
    {
        float V = *(float*)(Addr);
        if (V == 0.)
            V = Delta - 1.;
        else
            V *= Delta;
        *(float*)(Addr) = V;
    }
    break;
    case 'F':
        if (extCount)
        {
            BYTE eVal = *(BYTE*)Addr;
            int i = 0;
            for (i; i < extCount; i++)
            {
                if (pextVals && pextVals[i].eValue == eVal)
                    break;
            }
            if (Delta > 1.)
                i++;
            else
                i--;
            if (i < 0)
                i = 0;
            if (i >= extCount)
                i = extCount - 1;
            if (pextVals)
                *(BYTE*)(Addr) = pextVals[i].eValue;
        }
        break;
    case 'I':
    {
        if (!extCount)
        {
            int I = *(int*)(Addr);
            Delta -= 1.;
            Delta *= 100.;
            I = (int)(I + Delta);
            *(int*)(Addr) = I;
        }
        else
        {
            int iVal = *(int*)Addr;
            int i = 0;
            for (i; i < extCount; i++)
            {
                if (pextVals && pextVals[i].iValue == iVal)
                    break;
            }
            if (Delta > 1.)
                i++;
            else
                i--;
            if (i < 0)
                i = 0;
            if (i >= extCount)
                i = extCount - 1;
            if (pextVals)
                *(int*)(Addr) = pextVals[i].iValue;
        }
    }
    break;
    case 'H':
    {
        short I = *(short*)(Addr);
        Delta -= 1.;
        Delta *= 100.;
        I = (short)(I + Delta);
        *(short*)(Addr) = I;
    }
    break;
    case 'J':
    {
        BYTE J = *(BYTE*)(Addr);
        if (Delta > 1)
            J++;
        if (J && Delta < 1)
            J--;
        *(BYTE*)(Addr) = J;
    }
    break;
    case 'B':
    {
        bool B = *(bool*)(Addr);
        B = !B;
        *(bool*)(Addr) = B;
    }
    break;
    case 'C':
    {
        char C = *(char*)(Addr);
        char ch = C;
        switch (C)
        {
        case '?':ch = 'A'; break;
        case 'A':ch = 'R'; break;
        case 'R':ch = 'M'; break;
        case 'M':ch = '?'; break;
        }
        *(char*)(Addr) = ch;
    }
    break;
    case 'W':
    {
        CDef* pDef = (CDef*)Addr;
        bool FireDefect(CDef * pDef, BYTE Mask);
        FireDefect(pDef, 0);
    }
    break;
    case 'X':
    {
        IFreeParam* pIF = (IFreeParam*)Addr;
        pIF->ChangeVal(Delta);
    }
    break;
    case 'L':
    {
        int iVal = *(int*)Addr;
        if (Delta > 1.)
            iVal++;
        else
            iVal--;
        if (pDefValue == NULL)
        {
            *(int*)Addr = iVal;
            break;
        }
        if (iVal < 0)
            iVal = 0;
        char* P = (char*)pDefValue;
        for (int n = 0; n <= iVal; n++)
        {
            char* Pn = strchr(P, ';');
            if (Pn == NULL)
            {
                iVal = n - 1;
                break;
            }
            P = Pn + 1;
        }
        if (iVal < 0)
            iVal = 0;
        *(int*)Addr = iVal;
    }
    break;
    default:
        return false;
    }
    return true;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void QParams::WrtParam()
{
    if (pModel == NULL)
        return;
    // // сохранение
    // char CompName[HOST_NAME_MAX + 1] = {};
    // //gethostname(CompName, HOST_NAME_MAX);
    // auto szChangePath = std::filesystem::path(GetBasePath()) / "Params" / CompName / ".txt";
    // FILE* F = NULL;
    // if ((F = fopen(szChangePath.string().c_str(), "at")) == 0)
    //   return;
    // fprintf(F, "================= %s{0} =================\n", pModel->ObjName.Str);
    // char szTxt[256];
    // if (!m_bNoSave)
    // {
    //   Out(szTxt);
    //   if (Type == 'S')
    //     fprintf(F, "%s{%c}={%s}\n", ParamName, Type, szTxt);
    //   else
    //     fprintf(F, "%s{%c}=%s\n", ParamName, Type, szTxt);
    // }
    // fclose(F);
}
#if 0
#include "Extern_Pnt.h"
void ACS_Analog(IBaseModel * pModel)
{
    CExtern_Pnt * pPnt = (CExtern_Pnt*)pModel->pModel_Pnt;
    while(pPnt)
    {
        if(pPnt->Type[0] == 'A') pPnt->GetParams();
        pPnt = pPnt->pNext_Model_Pnt;
    }
}

void All_Connected(IBaseModel * pModel)
{
    CExtern_Pnt * pPnt = (CExtern_Pnt*)pModel->pModel_Pnt;
    while(pPnt)
    {
        if(pPnt->Type[1] == 'I' && !pPnt->IsConnection)
        {
            pPnt->Connection = "Auto";
            pPnt->IsConnection = true;
        }
        pPnt = pPnt->pNext_Model_Pnt;
    }
}

void ACS_Diskrete(IBaseModel * pModel)
{
    CExtern_Pnt * pPnt = (CExtern_Pnt*)pModel->pModel_Pnt;
    while ( pPnt )
    {
        if ( pPnt->Type[0] == 'D' )
            pPnt->GetParams( );
        pPnt = pPnt->pNext_Model_Pnt;
    }
}

void ACS_Connect( IBaseModel * pModel )
{
    CExtern_Pnt * pPnt = (CExtern_Pnt*)pModel->pModel_Pnt;
    while ( pPnt )
    {
        if ( pPnt->Type[1] == 'I' && !pPnt->IsConnection)
        {
            pPnt->IsConnection = true;
            pPnt->Connection == "#";
        }
        pPnt = pPnt->pNext_Model_Pnt;
    }
}

void ACS_Params( char * TabName, IBaseModel * pModel )
{
    if ( pModel == NULL || CParams::ReadProps )
        return;
    CExtern_Pnt * pPnt = (CExtern_Pnt*)pModel->pModel_Pnt;
    int K = 0;
    while ( pPnt )
    {
        if ( pPnt->Type[0] == 'A' )
            K++;
        pPnt = pPnt->pNext_Model_Pnt;
    }
    bool TotalTab = false;
    if ( K > 0 )
    {
        if ( TabName[0] && !CParams::SaveAllParams && !CParams::ReadProps )
        {
            TotalTab = true;
            static int Show1 = 0;
            CParams & __P1 = CParams::Get();
            __P1.SetDefaultValue(NULL);
            __P1.Set( pModel, $ TabName, Show1, 4 );
            __P1.Type = 'T';
            __P1.Class = 'X';
            static int Show2 = 0;
            CParams & __P2 = CParams::Get();
            __P2.SetDefaultValue(NULL);
            __P2.Set( pModel, $"Аналоговые точки", Show2, 4 );
            __P2.Type = 'T';
            __P2.Class = 'X';
        }
        pPnt = (CExtern_Pnt*)pModel->pModel_Pnt;
        while ( pPnt )
        {
            if ( pPnt->Type[0] == 'A' )
                pPnt->GetParams( );
            pPnt = pPnt->pNext_Model_Pnt;
        }
        static int Show3 = 0;
        CParams & __P3 = CParams::Get();
        __P3.SetDefaultValue(NULL);
        __P3.Set( pModel, $"Аналоговые точки", Show3, 4 );
        __P3.Type = 'E';
        __P3.Class = 'X';
    }
    K = 0;
    pPnt = (CExtern_Pnt*)pModel->pModel_Pnt;
    while ( pPnt )
    {
        if ( pPnt->Type[0] == 'D' )
            K++;
        pPnt = pPnt->pNext_Model_Pnt;
    }
    if ( K > 0 )
    {
        if ( TabName[0] && !CParams::SaveAllParams && !CParams::ReadProps )
        {
            if ( !TotalTab )
            {
                static int Show1 = 0;
                CParams & __P1 = CParams::Get();
                __P1.SetDefaultValue(NULL);
                __P1.Set( pModel, $ TabName, Show1, 4 );
                __P1.Type = 'T';
                __P1.Class = 'X';
            }
            static int Show3 = 0;
            CParams & __P3 = CParams::Get();
            __P3.SetDefaultValue(NULL);
            __P3.Set( pModel, $"Дискретные точки", Show3, 4 );
            __P3.Type = 'T';
            __P3.Class = 'X';
        }
        pPnt = (CExtern_Pnt*)pModel->pModel_Pnt;
        while ( pPnt )
        {
            if ( pPnt->Type[0] == 'D' )
                pPnt->GetParams( );
            pPnt = pPnt->pNext_Model_Pnt;
        }
        static int Show4 = 0;
        CParams & __P4 = CParams::Get();
        __P4.SetDefaultValue(NULL);
        __P4.Set( pModel, $"Дискретные точки", Show4, 4 );
        __P4.Type = 'E';
        __P4.Class = 'X';
    }
    if ( TotalTab )
    {
        static int Show5 = 0;
        CParams & __P5 = CParams::Get();
        __P5.SetDefaultValue(NULL);
        __P5.Set( pModel, $ TabName, Show5, 4 );
        __P5.Type = 'E';
        __P5.Class = 'X';
    }
}

void ACS_Params( IBaseModel * pModel )
{
    ACS_Params( "АСУ", pModel );
}

#endif

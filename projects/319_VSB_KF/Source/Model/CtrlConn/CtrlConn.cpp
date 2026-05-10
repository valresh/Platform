#include "stdafx.h"
#include "CtrlConn.h"
#include "Err.h"
#include <CommProc.h>
#include <Connect.h>
#include "SetData.h"
#include "EpsVol.h"
#include "Prop.h"
#ifdef LINUX
#include "../LS/LS.h"
#include "WinFiles.h"
#else
#include "..\LS\LS.h"
#endif

const double Press_Max = 6.;
LPCSTR CCtrlConn::m_szScale = "Scale";

BOOL FindSensInfo( const char* szName, double * pValue );
int GetAllConnections( char * ObjName, char * PntName, char & IO, char * RSU[16],  char * Desc[16] );
CCtrlConn * pMainConn = NULL;

CCtrlConn::CCtrlConn( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
//, VBS ( this )
{
    ObjName = _ObjName;
    Model = "CtrlConn";
#include "IO_Clear.h"
#include "Points.h"
    F_err = NULL;
    pScript = NULL;
    nFormula = 1;
    HaveFormula = false;
    ReRead = false;
    kStep = 0;
    //??  pFindSensInfo = FindSensInfo;
    pMainConn = this;
    DiscreteDefects = false;
    NoElectroShort = false;
    TimeNoElectroShort = 0.;
    pConnectionsList = &Connections;
    HaveProperties = true;
    pGetAllConnections = GetAllConnections;
    bIn = false;
}

CCtrlConn::~CCtrlConn()
{
}

#define MAX_SENS 1000
extern CSensInfo SensInfo[MAX_SENS];
extern int kSensInfo;
extern bool SortSensInfo;

int CCtrlConn::Init( int Regim )
{
    CLEAR(SensInfo)
    kSensInfo = 0;
    SortSensInfo = true;
    int Res = 0;
    //#include "IO_Init.h"
    //#include "Points.h"
    //#include "ACS_Init.h"
    //#include "ACS.h"
    //CreateStruct_MM( "Связи_MM" );
    CreateStruct_Rsu( );
    return 0;
}

int CCtrlConn::Step0( )
{
    SetEvents( );
    Press_KIP[0] = Press_Max;
    CStableConnect::Init( );
    return 0;
}

#undef SET_DEF
#define SET_DEF(DefectVar,Comment) \
{\
        LastDefect = NewDefect( );\
        DefectVar = GetDefect( LastDefect );\
        DefectVar->Parent = RootNode;\
        lstrcpy ( DefectVar->Name, Comment );\
        lstrcpy ( DefectVar->ObjName, ObjName );\
        DefectVar->Flags = CDef::Resource|CDef::Defect|CDef::Active;\
        DefectVar->pModel = this;\
}

#undef SET_PARM
#define SET_PARM(Var, vName, vDef, vMin, vMax ) \
{\
        Var = NewDefectPram( LastDefect );\
        Var->Type = CDefParam::tDbl;\
        lstrcpy(Var->Name, vName );\
        Var->Min = (float)(vMin);\
        Var->Max = (float)(vMax);\
        Var->Dbl = Var->_Dbl = (double)(vDef);\
}


int CCtrlConn::Step1( )
{
    int nRes = 0;
    DWORD RootNode = GetRootResource( );
    DWORD LastDefect = 0;
    char Name[256];
    sprintf_s ( Name, sizeof(Name), "Атмосфера" );
    SET_DEF(Resource[nRes],Name)
    double T = T_air;
    SET_PARM(ResParm_T[nRes], "Температура", 15., -50, 60. )
    nRes++;
    //////////////////////////////////////////////////////////////////////
    Char<256>Path;
    Path.Prt( "%sDATA\\Acy\\Errors.csv", PROJECT_ROOT );
    TakeOffReadOnly( (const char*)Path );
    F_err = Fopen ( Path, "wt" );
    Fprintf( F_err, "Файл РСУ;Строка;Объект;Модель;Точка модели;Тип РСУ;Сообщение\n" );
    //
    //  Path.Prt( "%sDATA\\SCRIPT\\ScriptCSV.vbs",  PROJECT_ROOT );
    //  TakeOffReadOnly( Path );
    //  pScript = Fopen( Path, "wt" );
    pScript = NULL;
    //
    DiscreteDefects = false;
    PROPS.Get( "Отказы дискретов", DiscreteDefects );
    ////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //
    ///home/resh/Platform/projects/319_VSB_KF/DATA/Acy
    Path.Prt( "%sDATA\\Acy\\*.csv", PROJECT_ROOT );
    //
    int NumbFile = 1;
    WIN32_FIND_DATA FD;
    HANDLE hFind = FindFirstFile( Path.Str, &FD );
    if( hFind == INVALID_HANDLE_VALUE )
        return 0;
    BOOL Res = TRUE;
    int kErr = 0;
    char * File;
    while ( Res )
    {
        if ( FD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
            Res = FindNextFile( hFind, &FD );
            continue;
        }
        File = FD.cFileName;
        char * P = strchr ( File, '.' );
        if ( P )
            *P = 0;
        if ( strncmp ( File, "Связи_", 6 ) == 0 )
        {
            NumbFile++;
            //      ReadConnections_MM( NumbFile, File );
            //Сброс переменных перед перечтением Связей.
            for ( int n = 0; n < IBaseModel::kObjects; n++ )
            {
                IBaseModel::AllObjects[n]->ResetVariable();
            }
            ReadConnections_MM( File );
            Res = FindNextFile( hFind, &FD );
            continue;
        }
        if ( strncmp ( File, "РСУ_", 4 ) == 0 )
        {
            NumbFile++;
            ////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            ///home/resh/Platform/projects/319_VSB_KF/DATA/Acy
            ReadRsuCsv( NumbFile, File );
            Res = FindNextFile( hFind, &FD );
            continue;
        }
        Res = FindNextFile( hFind, &FD );
    }
    //	Fclose( pScript );
    //
    //  ReadConnections_MM( "Связи_MM" );
    // LS
    CLS * pLS = CLS::pFirstLS;
    while ( pLS )
    {
        CConnect & Conn = Connections.push_back();
        Conn.Init();
        Conn.ThisLS = true;
        Conn.SetDefect = false;
        Conn.pStable = NULL;
        Conn.pSrc = NULL;
        Conn.pDst = NULL;
        Conn.RefFrom = pLS->ObjName;
        Conn.RefTo = "";
        Conn.Desc = "Датчик по месту";
        Conn.Sens = pLS->ObjName.Str;
        Conn.Line = 0;
        Conn.PreDstValue = 0.;
        Conn.OP = NULL;
        Conn.kOP = 0;
        Conn.Type[0] = 'A';
        Conn.SetDefect = true;
        Conn.pTrueValueLS = &pLS->TrueValue;
        Conn.pDefectValueLS = &pLS->Value;
        //
        pLS = pLS->pNextLS;
    }
    Fclose( F_err );
    //
    if ( !ReRead )
    {
        SetConnDefects( );
        char KIPName[64];
        strcpy ( KIPName, "КИП" );
        PROPS.Get( "Имя КИП", KIPName );
        IBaseModel * pModel = IBaseModel::Find( KIPName );
        if ( pModel )
        {
            pModel->SetData ( sd_GetPressKIP, &KIP[0] );
            pModel->SetData ( sd_GetPressKIP, &KIP[1] );
            pModel->SetData ( sd_GetPressKIP, &KIP[2] );
        }
        else
        {
            static double StaticKIP = 10.0;
            KIP[0] = KIP[1] = KIP[2] = & StaticKIP;
        }
        ReadParams( );
        //ReadPID( );
        ReadSens( );
        // bool SensCtrl = false;
        // PROPS.Get( "Контроль по датчикам", SensCtrl );
        // if ( SensCtrl && pSetConnectionsList )
        // 	(*pSetConnectionsList)();
    }
    Press_KIP[0] = *KIP[0] - 1;
    Press_KIP[1] = *KIP[1] - 1;
    Press_KIP[2] = *KIP[2] - 1;
    memset(Electro, true, sizeof(Electro));
    return 0;
}

double * GetAddr( void * Err, int Line, char * Ref, char IO, char * DefModel, char * DefPoint )
{
    if ( Ref == NULL || Ref[0] == 0 )
        return NULL;
    char * P = strchr ( Ref, '.' );
    if ( P )
        *P = 0;
    IBaseModel * pObj = IBaseModel::Find( Ref );
    if ( pObj == NULL )
    {
        Fprintf ( Err, "В строке %d;Объект '%s' не найден;\n", Line, Ref );
        return NULL;
    }
    char * Point = NULL;
    if ( P )
        Point = P + 1;
    if ( pObj->Model == DefModel )
        Point = DefPoint;
    if ( Point == NULL )
    {
        Fprintf ( Err, "В строке %d;У объекта '%s' неправильно заданы точки;\n", Line, Ref );
        return NULL;
    }
    if ( Point[0] == '@' )
        Point++;
    CExtern_Pnt * pPnt = pObj->FindPoint( Point );
    if ( pPnt == NULL )
    {
        Fprintf ( Err, "В строке %d;У объекта '%s' заданая точка не найдена;\n", Line, Ref );
        return NULL;
    }
    if ( pPnt->Type[0] != 'A' && pPnt->Type[1] != IO )
    {
        Fprintf ( Err, "В строке %d;У объекта '%s' заданая точка имеет неправильный тип;\n", Line, Ref );
        return NULL;
    }
    CAnalog * pA = (CAnalog*)pPnt;
    return &pA->Value;
}

CExtern_Pnt * GetPnt( void * Err, int Line, char * Ref )
{
    if ( Ref == NULL || Ref[0] == 0 )
        return NULL;
    char * P = strchr ( Ref, '.' );
    if ( P == NULL )
        return NULL;
    *P = 0;
    IBaseModel * pObj = IBaseModel::Find( Ref );
    if ( pObj == NULL )
    {
        Fprintf ( Err, "В строке %d;Объект '%s' не найден;\n", Line, Ref );
        return NULL;
    }
    char * Point = P + 1;
    if ( Point[0] == '@' )
        Point++;
    CExtern_Pnt * pPnt = pObj->FindPoint( Point );
    if ( pPnt == NULL )
    {
        Fprintf ( Err, "В строке %d;У объекта '%s' заданая точка не найдена;\n", Line, Ref );
        return NULL;
    }
    return pPnt;
}

#define IS_MIN 0x01
#define IS_MAX 0x02
static char * Valid = "<>.0123456789";
int GetLimits( void * Err, int Line, char * Limits, double & Min, double & Max )
{
    int Flag = 0;
    char s;
    int n = 0;
    int p = 0;
    while ( 1 )
    {
        s = Limits[n];
        if ( s == 0 )
            break;
        if ( s == ' ' )
        {
            n++;
            continue;
        }
        if ( s == ',' )
            s = '.';
        char * P = strchr ( Valid, s );
        if ( P == NULL )
        {
            Fprintf ( Err, "В строке %d;В задании пределов недопустимый символ '%c';\n", Line, s );
            return 0;
        }
        Limits[p++] = s;
        n++;
    }
    // 5,3><5,9
    char * P = strstr ( Limits, "><" );
    if ( P )
    {
        // Заданы оба предела
        *P = 0;
        Min = atof ( Limits );
        Max = atof ( P + 2 );
        return IS_MIN|IS_MAX;
    }
    P = strstr ( Limits, "<" );
    if ( P )
    {
        // Заданы Max
        *P = 0;
        Max = atof ( P + 1 );
        return IS_MAX;
    }
    P = strstr ( Limits, ">" );
    if ( P )
    {
        // Заданы MIN
        *P = 0;
        Min = atof ( P + 1 );
        return IS_MIN;
    }
    return 0;
}

void CCtrlConn::ReadSolid( )
{
    Char<512>Path;
    Path.Prt ( "%sDATA\\Equipment\\Датчики.csv", PROJECT_ROOT );
    FILE * F = fopen ( Path, "rt" );
    if ( F == NULL )
        return;
}

void CCtrlConn::ReadSens( )
{
    bool GetMes = true;
    PROPS.Get ( "Брать размерности", GetMes );
    Char<512>Path;
    Path.Prt ( "%sDATA\\Equipment\\Params\\Датчики.csv", PROJECT_ROOT );
    FILE * F = fopen ( Path, "rt" );
    if ( F == NULL )
        return;
    Path.Prt ( "%sDATA\\Equipment\\Params\\ErrДатчики.csv", PROJECT_ROOT );
    void * Err = Fopen ( Path, "wt" );
    char Str[256];
    //Описание	Имя	Размерность	Пределы
    fgets ( Str, 255, F );
    char Res[4][256];
    int Line = 1;
    while ( fgets ( Str, 255, F ))
    {
        Line++;
        int L = lstrlen ( Str ) - 1;
        Str[L] = 0;
        if ( Str[0] == ';' )
            continue;
        Split<256,4,256>( Str, Res );
        char * Desc = Res[0];
        char * Sens = Res[1];
        char * Mes = Res[2];
        char * Limits = Res[3];
        //////////////////////////////////////////
        IBaseModel * pSens = NULL;
        CExtern_Pnt * pPnt = NULL;
        if ( strchr ( Sens, '.' ) == NULL )
        {
            // Датчик
            IBaseModel * pSensObj = IBaseModel::Find( Sens );
            if ( pSensObj == NULL )
            {
                Fprintf ( Err, "В строке %d;Датчик '%s' не найден;\n", Line, Sens );
                continue;
            }
            if ( pSensObj->Model != "Датчик" )
            {
                Fprintf ( Err, "В строке %d;'%s' не Датчик;\n", Line, Sens );
                continue;
            }
            pSens = pSensObj;
            if ( pSens->ObjName == "P134" )
                KKK();
        }
        else
        {
            pPnt = GetPnt( Err, Line, Sens );
            if ( pPnt == NULL )
            {
                Fprintf ( Err, "В строке %d; Точка '%s' не найдена;\n", Line, Sens );
                continue;
            }
            if ( pPnt->Type[0] != 'A' && pPnt->Type[1] != 'O' )
            {
                Fprintf ( Err, "В строке %d; Точка '%s' имеет неправильный тип;\n", Line, Sens );
                continue;
            }
        }
        //
        if ( GetMes && Mes[0] )
        {
            // Размерность
            int vMes = GetEngUnits( Mes );
            if ( vMes < 0 )
            {
                Fprintf ( Err, "В строке %d;Неизвестная размерность '%s';\n", Line, Mes );
            }
            else
            {
                double M = Scale ( vMes );
                if ( pSens )
                    pSens->SetData ( sd_SetSensMes, &vMes );
                if ( pPnt )
                    pPnt->SetMes( vMes, M );
            }
        }
        //
        double Min, Max;
        int Res = GetLimits( Err, Line, Limits, Min, Max );
        if ( Res == 0 )
            continue;
        if ( Res & IS_MAX )
        {
            if ( pSens )
                pSens->SetData ( sd_SetSensMax, &Max );
        }
        if ( Res & IS_MIN )
        {
            if ( pSens )
                pSens->SetData ( sd_SetSensMin, &Min );
        }
    }
    fclose(F);
    Fclose(Err);
}

/*void CCtrlConn::ReadPID( )
  {
  Char<512>Path;
  Path.Prt ( "%sDATA\\Equipment\\Params\\PID.csv", PROJECT_ROOT );
  FILE * F = fopen ( Path, "rt" );
  if ( F == NULL )
    return;
  Path.Prt ( "%sDATA\\Equipment\\Params\\ErrPID.csv", PROJECT_ROOT );
  void * Err = Fopen ( Path, "wt" );
  char Str[256];
//PID	Датчик	Клапан	Главный PID	Мин PV	Макс PV	Мин MV	Макс MV	Примечание

  fgets ( Str, 255, F );
  char Res[9][128];
  int Line = 1;
  while ( fgets ( Str, 255, F ))
    {
    Line++;
    int L = lstrlen ( Str ) - 1;
    Str[L] = 0;
    if ( Str[0] == ';' )
      continue;
    Split<256,9,128>( Str, Res );
    char * PID = Res[0];
    char * PV = Res[1];
    char * MV = Res[2];
    char * SP = Res[3];
    char * MinPV = Res[4];
    char * MaxPV = Res[5];
    char * MinMV = Res[6];
    char * MaxMV = Res[7];
    char * Comm = Res[8];
//////////////////////////////////////////
   //
    IBaseModel * pObj = IBaseModel::Find( PID );
    if ( pObj == NULL )
      {
      Fprintf ( Err, "В строке %d;REG '%s' не найден;\n", Line, PID );
      continue;
      }
    if ( pObj->Model != "REG" )
      {
      Fprintf ( Err, "В строке %d;'%s' не REG;\n", Line, PID );
      continue;
      }
    CPID * pPID = (CPID*)pObj;
//
    if ( PV[0] )
      pPID->pPV = GetAddr( Err, Line, PV, 'O', "Датчик", "Показания" );
//
    if ( MV[0] )
      pPID->pMV = GetAddr( Err, Line, MV, 'I', "Клапан", "Задание" );
//
    if ( SP[0] )
      {
      IBaseModel * pSP = IBaseModel::Find( SP );
      if ( pSP == NULL )
        {
        Fprintf ( Err, "В строке %d;REG '%s' не найден;\n", Line, SP );
        continue;
        }
      if ( pSP->Model != "REG" )
        {
        Fprintf ( Err, "В строке %d;'%s' не REG;\n", Line, SP );
        continue;
        }
      pPID->pMainPID = (CPID*)pSP;
      }
    if ( MinPV[0] )
      pPID->SL = atof ( MinPV );
    if ( MaxPV[0] )
      pPID->SH = atof ( MaxPV );
    if ( MinMV[0] )
      pPID->ML = atof ( MinMV );
    if ( MaxMV[0] )
      pPID->MH = atof ( MaxMV );
    }
  fclose(F);
  Fclose(Err);
  }*/

void CCtrlConn::ReadParams( )
{
    Char<512>Path;
    Path.Prt ( "%sDATA\\Equipment\\Params\\Параметры.csv", PROJECT_ROOT );
    FILE * F = fopen ( Path, "rt" );
    if ( F == NULL )
        return;
    char Str[256];
    while ( fgets ( Str, 255, F ))
    {
        int L = lstrlen ( Str ) - 1;
        Str[L] = 0;
        if ( Str[0] == ';' )
            continue;
        char * Obj = Str;
        if ( *Obj == '#' )
            Obj++;
        char * Param = strchr( Obj, ';' );
        if ( Param == NULL )
            continue;
        *Param = 0;
        //
        IBaseModel * pObj = IBaseModel::Find( Obj );
        if ( pObj == NULL )
            continue;
        Param++;
        char * Value = strchr( Param, ';' );
        if ( Value == NULL )
            continue;
        *Value = 0;
        Value++;
        char * End = strchr( Value, ';' );
        if ( End )
            *End = 0;
        CParams::LockParams( );
        pObj->GetParams( "" );
        for ( int n = 0; n < CParams::kParams; n++ )
        {
            CParams & P = CParams::Params[n];
            if ( lstrcmp ( P.ParamName, Param ) == 0 )
            {
                P.SetValue ( Value );
                break;
            }
        }
        CParams::FreeParams();
    }
    fclose(F);
}

int CCtrlConn::PreStepT()
{
    if ( HaveFormula )
    {
        //VBS.Restart = 1;
        //VBS.Init( "ScriptCSV" );
    }
    return 0;
}

int CCtrlConn::StepT ( double dt )
{
    //  double FI;
    //  BOOL Res = (*pFindSensInfo)("FI_2304",&FI);
    //double U;
    //BOOL ResU;
    //ResU = FindSensInfo( "PISA_113", &U );
    //ResU = FindSensInfo( "PISA_1132", &U );
    //ResU = FindSensInfo( "PISA_1133", &U );
    //ResU = FindSensInfo( "PISA_123", &U );
    //ResU = FindSensInfo( "PISA_1232", &U );
    //ResU = FindSensInfo( "PISA_1233", &U );
    //ResU = FindSensInfo( "PISA_133", &U );
    //ResU = FindSensInfo( "PISA_1332", &U );
    //ResU = FindSensInfo( "PISA_1333", &U );
    Calc ( dt );
    if ( CStableConnect::CallControl >= 0 )
        CStableConnect::Control();
    kStep++;
    return 0;
}

int CCtrlConn::SetData( int TypeData, void * pData )
{
    switch(TypeData)
    {
    case sd_SetConnections:
#ifdef LINUX
        *(void**)pData = &Connections_Info;
    return 0;
#endif
    case sd_SetScript:
        ASS(0);
     return 0;
    case sd_ReReadRSU:
        ReRead = true;
        return 0;
    case sd_SetConnTypy:
        bIn = *(bool*)pData;
     return 0;
    }
    return 1;
}

int CCtrlConn::SaveState( )
{
    S_CLASS("W",_W)
    return 0;
}

int CCtrlConn::RestoreState( char * StrName )
{
    R_CLASS("W",_W)
    return 1;
}

int GetAllConnections( char * ObjName, char * PntName, char & SD, char * RSU[16],  char * Desc[16] )
{
    int N = 0;
    Char<256>Ref;
    Ref.Prt ( "%s.%s", ObjName, PntName );
    for ( int n = 0; n < pMainConn->Connections.L; n++ )
    {
        CConnect & C = pMainConn->Connections[n];
        if ( C.RefTo.Str != NULL && strcmp ( C.RefTo, Ref ) == 0 )
        {
            char * pDesc = C.Desc;
            if ( pDesc == NULL )
                pDesc = "";
            SD = 'D';
            RSU[N] = C.RefFrom;
            Desc[N] = pDesc;
            N++;
            if ( N >= 16 )
                return N;
        }
        if ( C.RefFrom.Str != NULL && strcmp ( C.RefFrom, Ref ) == 0 )
        {
            char * pDesc = C.Desc;
            if ( pDesc == NULL )
                pDesc = "";
            SD = 'S';
            RSU[N] = C.RefTo;
            Desc[N] = pDesc;
            N++;
            if ( N >= 16 )
                return N;
        }
    }
    return N;
}


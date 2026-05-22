#include "stdafx.h"
#include "Trends.h"
#include "ACS_Def.h"
#include "SetData.h"
#include "Connect.h"


BOOL APIENTRY DllMain( HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved )
{
  return TRUE;
}

extern "C" Q_DECL_EXPORT IBaseModel* CreateObject(LPCTSTR ObjName )
{
  return new CTrendsW(ObjName);
}

CTrendsW::CTrendsW( LPCTSTR _ObjName ) : IBaseModel ((char*)_ObjName)
{
  Interval = 0.;
  kStep = 0;
}

CTrendsW::~CTrendsW()
{
}

int CTrendsW::Init( int )
{
  kStep = 0;
  return 0;
}

int CTrendsW::SetData( int TypeData, void * pData )
{
  return 1;
}

int CTrendsW::GetParams( char * )
{
#include "IO_Parms.h"
  PARM( Interval, "#Интервал" )
  return 0;
}

int CTrendsW::UpdateParam( CParams & Param )
{
  return 0;
}

int CTrendsW::Step0()
{
  Trends.Init();
  //  AddVars();
  return 0;
}

int CTrendsW::Step1()
{
  //  ASS(Trends.OpenTrends())
  //  ModelFlags |= MODEL_IN_USE;
  return 0;
}

int CTrendsW::PreStepT()
{
  //  Trends.Add ( "Sys_Time", 'D', &pSys->dModelT );
  //  Trends.Add ( "Sys_Step", 'I', &pSys->m_nStep );
  AddVars();
  AddRegs();
  ASS(Trends.OpenTrends())
  return 0;
}

int CTrendsW::StepT ( double dt )
{
  ASS(Trends.Write())
  kStep++;
  return 0;
}

#define MAX_TXT 128

template <int K,int S>
int Split ( char Str[256], char Res[K][S] )
{
  char * P = Str;
  int N = 0;
  for ( int n = 0; n < K; n++ )
  {
    memset ( Res[n], 0, S );
    if ( P == NULL )
      break;
    char * Pn = strchr ( P, ';' );
    if ( Pn )
    {
      *Pn = 0;
      lstrcpy ( Res[n], P );
      P = Pn + 1;
      if ( Res[n][0] )
        N = n + 1;
      continue;
    }
    if ( P )
    {
      lstrcpy ( Res[n], P );
      P = NULL;
      if ( Res[n][0] )
        N = n + 1;
    }
  }
  return N;
}

void CTrendsW::AddRegs( )
{
  Char<256>Path;
  Path.Prt ( "%sDATA\\Reg\\Reg.csv", PROJECT_ROOT );
  FILE * F = fopen( Path, "rt" );
  if ( F == NULL )
  {
    return;
  }
  char Str[256];
  fgets ( Str, 255, F );
  char Txt[9][MAX_TXT];
  while ( fgets ( Str, 255, F ))
  {
    int L = lstrlen ( Str ) - 1;
    if ( L < 0 )
      L = 0;
    Str[L] = 0;
    int K = Split<9,MAX_TXT>( Str, Txt );
    //
    bool IsReg = false;
    bool On = false;
    if ( lstrcmp ( Txt[0], "Рег" ) == 0 )
    {
      IsReg = true;
      On = true;
    }
    if ( lstrcmp ( Txt[0], "Откл" ) == 0 )
    {
      IsReg = true;
      On = false;
    }
    if ( IsReg )
    {
      char TypeVar;
      void * pVar = ConnectM( Txt[5], &TypeVar );
      if ( pVar == NULL )
        continue;
      Trends.Add ( Txt[5], TypeVar, pVar );
    }
  }
  fclose ( F );
}

void CTrendsW::AddVars( char * PathCSV )
{
  FILE * F = fopen ( PathCSV, "rt" );
  if ( F == NULL )
    return;
  char Str[256];
  fgets( Str, 255, F );
  while ( fgets( Str, 255, F ) )
  {
    char * P = strchr( Str, ';' );
    if ( P == NULL )
      continue;
    *P = 0;
    char * Pnt = Str;
    P++;
    while ( *P == ' ' )
      P++;
    char Type = *P;
    //
    char TypeVar;
    void * pVar = ConnectM( Pnt, &TypeVar );
    if ( pVar == NULL )
      pVar = ConnectR( Pnt, &TypeVar );
    if ( pVar == NULL )
      continue;
    Trends.Add ( Pnt, TypeVar, pVar );
  }
  fclose ( F );
}

#include "RsuX.h"

void * CTrendsW::ConnectR( char * Pnt, char * Type )
{
#ifdef LINUX
  return NULL;
#else
  if ( Pnt == NULL || Pnt[0] == 0 )
    return NULL;
  void * pVar = NULL;
  if ( pGetACSObjectValue )
  {
    (*pGetACSObjectValue)( Pnt, &pVar, Type );
  }
  return pVar;
#endif

}

void * CTrendsW::ConnectM( char * Pnt, char * Type )
{
  if ( Pnt == NULL || Pnt[0] == 0 )
    return NULL;
  char Point[256];
  lstrcpy ( Point, Pnt );
  char * P = strchr ( Point, '.' );
  if ( P == NULL )
    return NULL;
  *P = 0;
  char * Obj = Point;
  IBaseModel * pModel = IBaseModel::Find ( Obj );
  if ( pModel == NULL )
    return NULL;
  char * Param = P + 1;
  // ACS
  CExtern_Pnt * pPnt = NULL;
  if ( pModel && pModel->Points.Root )
    pPnt = (CExtern_Pnt*)::Find( &pModel->Points, Param, false );
  if ( pPnt )
  {
    if ( pPnt->Type[0] == 'A' )
    {
      *Type = 'D';
      CAnalog * pA = (CAnalog*)pPnt;
      double * pAddr = (double*)&(pA->Value);
      return pAddr;
    }
    if ( pPnt->Type[0] == 'D' )
    {
      *Type = 'I';
      CDiscrete * pD = (CDiscrete*)pPnt;
      int * pAddr = (int*)&(pD->Value);
      return pAddr;
    }
    return NULL;
  }
  CLockParams Lock;
  pModel->GetParams( NULL );
  for ( int n = 0; n < CParams::kParams; n++ )
  {
    CParams & Parm = CParams::Params[n];
    if ( lstrcmp ( Parm.ParamName, Param ) == 0 )
    {
      if ( Parm.Type == 'D' )
      {
        *Type = 'D';
        return Parm.Addr;
      }
      if ( Parm.Type == 'I' )
      {
        *Type = 'I';
        return Parm.Addr;
      }
      return NULL;
    }
  }
  return NULL;
}

void CTrendsW::AddVars()
{
  Char<512>Path;
  Path.Prt ( "%sINI\\TRENDS\\*.csv", PROJECT_ROOT );
  WIN32_FIND_DATA FD;
  HANDLE hFind = FindFirstFile( Path, &FD );
  if ( hFind == INVALID_HANDLE_VALUE )
    return;
  BOOL Res = TRUE;
  while ( Res )
  {
    if ( strcmpi (FD.cFileName, "Variables.csv") == 0 )
    {
      Res = FindNextFile ( hFind, &FD );
      continue;
    }
    if ( strcmpi (FD.cFileName, "Тренды.csv") == 0 )
    {
      Res = FindNextFile ( hFind, &FD );
      continue;
    }
    Path.Prt ( "%sINI\\TRENDS\\%s", PROJECT_ROOT, FD.cFileName );
    AddVars( Path );
    Res = FindNextFile ( hFind, &FD );
  }
  FindClose( hFind );
#if 0
  Char<256>Name;
  for ( int n = 0; n < IBaseModel::kObjects; n++ )
    {
    IBaseModel * pModel = IBaseModel::AllObjects[n];
    if ( pModel->TypeObj == Y_Obj || pModel->TypeObj == RSU_Obj )
      continue;
    pModel->SetData ( sd_AddToTrends, &Trends );
/******************
//    if ( pModel->pMain )// не главная
//      continue;
    CExtern_Pnt * pPnt = (CExtern_Pnt*)GetFirst( &pModel->Points );
    while ( pPnt )
      {
      switch ( pPnt->Type[0] )
        {
        case 'A':
          {
          CAnalog * pA = (CAnalog*)pPnt;
          Name.Prt( "%s.%s", (char*)pModel->ObjName, pA->Point );
          Trends.Add( Name, 'D', &pA->pValue->dVal );
          }
        break;
        case 'D':
          {
          CDiscrete * pD = (CDiscrete*)pPnt;
          Name.Prt( "%s.%s", (char*)pModel->ObjName, pD->Point );
          Trends.Add( Name, 'I', &pD->pValue->nVal );
          }
        break;
        }
      pPnt = (CExtern_Pnt*)GetNext( );
      }
*************/
    }
#endif
}


int CTrendsW::SaveState()
{
  return 0;
}

int CTrendsW::RestoreState( char * StrName )
{
  return 0;
}

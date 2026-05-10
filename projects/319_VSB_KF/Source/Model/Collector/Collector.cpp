#include "stdafx.h"
#include "Collector.h"
#include "Err.h"
#include "CommProc.h"
//#include "Data.h"
#include "SetData.h"
#include "HydroStruct.h"

LPCSTR g_szHelpFileName = "Базовый коллектор.pdf";

CCollector::CCollector( char * _ObjName ) :	IBaseModel ( _ObjName ) 
, State ( "Состояние", this )
, Stable ( "поток", this, &Flow_Ctrl )
//#include "ACS_Constr.h"
//#include "ACS.h"
	{
  UNIMODEL
	ObjName = _ObjName;
	#include "IO_Clear.h"
	#include "Points.h"
    #include "Collector_Defects.h"
  pNodes = NULL;
  Omega = NULL;
//  kOmega = NULL;
  pFlow = NULL;
  kIO = 0;
//  NumbIn = -1;
//  NumbOut = -1;
  Pio = 0.;
  kStep = 0;
  Flow_IO = 0.;
  Defect = 0;
  T = 15.;
  P = 1.;
  dP_old = 0.;
  FlowSumm = NULL;
  PipeNames = NULL;
  CLEAR(NameIn)
  CLEAR(NameOut)
  }

CCollector::~CCollector()
	{
	}

int CCollector::Init( int )
	{
	int Res = 0;
	//#include "IO_Init.h"
	//#include "Points.h"
	//#include "ACS_Init.h"  
	//#include "ACS.h"
  Omega = new double[kIO];
//  kOmega = new double[kIO];
  pFlow = new CFlow*[kIO];
  Flow_kg = new double[kIO];
  pStateIn = new CSEps*[kIO];
  for ( int n = 0; n < kIO; n++ )
    {
    if ( pStateIn )
      {
      char Name[16];
      sprintf_s( Name, 16, "Вход %d", n );
      pStateIn[n] = new CSEps( Name, this );
      }
    Omega[n] = 10000.;
//    kOmega[n] = 1.;
    pFlow[n] = NULL;
    Flow_kg[n] = 0.;
    }
  NewComp( &ID_Comp, &pComp, "Состав в %s", (char*)ObjName );
  NewComp( &ID_CompZ, &pCompZ, "Заданный состав в %s", (char*)ObjName );
  if ( Состав[0] )
    pCompZ->Read( Состав );
  else
    pCompZ->Set( pN2 );
  P = Pz + 1.;
  T = Tz;
  h_Flow;
  pComp->Set( pCompZ );
  memmove ( IsComp, pCompZ->IsComp, LAST_GAS );
  memmove ( Cmol, pCompZ->Cmol, LAST_GAS * 8 );
  State.PT ( P, T, IsComp, Cmol );
  h_Flow = State.h;
//  ASS(Ctrl.AddStable( &P, "давление" ))
//  ASS(Ctrl.AddStable( &Flow_Ctrl, "поток" ));
  int Size = 5;
  for ( int n = 0; n < kIO; n++ )
    {
    CObjectPoint * pOP = pNodes[n].pOP;
    Size += lstrlen ( pOP->pPool->Name ) + 1;
    }
  PipeNames = NewArr ( char, Size );
  lstrcpy ( PipeNames, "Нет;" );
  int Pos = 4;
  for ( int n = 0; n < kIO; n++ )
    {
    CObjectPoint * pOP = pNodes[n].pOP;
    lstrcpy ( PipeNames + Pos, pOP->pPool->Name );
    Pos += lstrlen ( pOP->pPool->Name );
    PipeNames[Pos++] = ';';
    }
  return 0;
	}

int CCollector::Step0( )
	{
	return 0;
	}

int CCollector::Step1( )
	{
  if ( Состав[0] )
    {
    pCompZ->Read( Состав );
    }
#ifdef INIT_STATE
  P = 1.;  ??
#endif
  if ( Use_Comp )
    {
    pComp->Set( pCompZ );
    Use_Tz = true;
    memmove ( IsComp, pComp->IsComp, sizeof ( IsComp ));
    memmove ( Cmol, pComp->Cmol, sizeof ( Cmol ));
    To_kg = ::Mw( pComp );
    T = Tz;
    State.PT( P, T, pComp );
    h_Flow = State.h;
    if ( State.E > 0.2 )
      To_m3 = M3;
    else
      To_m3 = To_m3_liq ( T, pComp );
    }
  State.PT( P, T, IsComp, Cmol );
  Set_h_summ( &State, h_summ );
////////////////////////////////////////
//  if ( strstr ( ObjName, "Факельный коллектор"))
//    KKK();
  bool OK = false;
  if ( Summ_In > 0)
    {
    int n = Summ_In - 1;
    if ( NameIn[0] == 0 )
      {
      CObjectPoint * pOP = pNodes[n].pOP;
      strcpy_s ( NameIn, 64, pOP->pPool->Name );
      OK = true;
      }
    else
      {
      CObjectPoint * pOP = pNodes[n].pOP;
      if ( lstrcmp ( NameIn, pOP->pPool->Name ) == 0 )
        OK = true;
      }
    }
  if (NameIn[0] && !OK )
    {
    int N = -1;
    for ( int n = 0; n < kIO; n++ )
      {
      CObjectPoint * pOP = pNodes[n].pOP;
      if ( lstrcmp ( NameIn, pOP->pPool->Name ) == 0 )
        {
        N = n;
        break;
        }
      }
 //   ASS(N == Summ_In-1)
    Summ_In = N + 1;
    }
  //
  OK = false;
  if ( Summ_Out > 0)
    {
    int n = Summ_Out - 1;
    if ( NameOut[0] == 0 )
      {
      CObjectPoint * pOP = pNodes[n].pOP;
      strcpy_s ( NameOut, 64, pOP->pPool->Name );
      OK = true;
      }
    else
      {
      CObjectPoint * pOP = pNodes[n].pOP;
      if ( lstrcmp ( NameOut, pOP->pPool->Name ) == 0 )
        OK = true;
      }
    }
  if (NameOut[0] && !OK )
    {
    int N = -1;
    for ( int n = 0; n < kIO; n++ )
      {
      CObjectPoint * pOP = pNodes[n].pOP;
      if ( lstrcmp ( NameOut, pOP->pPool->Name ) == 0 )
        {
        N = n;
        break;
        }
      }
 //   ASS(N == Summ_Out-1)
    Summ_Out = N + 1;
    }
  return 0;
	}

int CCollector::StepT ( double dt )
	{
//  LARGE_INTEGER Start;
//  QueryPerformanceCounter( &Start );
  Calc ( dt );
//  LARGE_INTEGER Finish;
//  QueryPerformanceCounter( &Finish );
//  LONGLONG Delta = Finish.QuadPart - Start.QuadPart;
//  Timer += Delta;
//  Timer2 = Counter;
//  Ctrl.Calc( dt );
  kStep++;
	return 0;
	}

int CCollector::SetData( int TypeData, void * pData )
	{
  switch ( TypeData )
    {
    case sd_SetInitState:
      {
      memset( static_cast<_W*>(this), 0, sizeof ( _W ));
      P = 1.;
      T = 15.; 
      }
    break;
    case sd_ShowObjInfo:
      {
      Char<256>* pInfo = (Char<256>*)pData;
      pInfo->Prt( "%s: Fi=%4.2lfкг, P(и)=%4.2lf, T=%4.2lf", (char*)ObjName, _F_in, _P, _T );
      }
    break;
    case sd_ParamClicked:
      {
      CParamClicked * pPC = (CParamClicked*)pData;
      if ( lstrcmp ( pPC->ParamName, "Вход коллектора" ) == 0 )
        {
        int n = Summ_In - 1;
        if ( n >= 0 )
          {
          pPC->Command = CParamClicked::ВыделитьТрубу;
          lstrcpy ( pPC->Object, (char*)pNodes[n].pOP->pPool->Name );
          return 0;
          }
        return 1;
        }
      if ( lstrcmp ( pPC->ParamName, "Выход коллектора" ) == 0 )
        {
        int n = Summ_Out - 1;
        if ( n >= 0 )
          {
          pPC->Command = CParamClicked::ВыделитьТрубу;
          lstrcpy ( pPC->Object, (char*)pNodes[n].pOP->pPool->Name );
          return 0;
          }
        return 1;
        }
      for ( int n = 0; n < kIO ; n++ )
        {
        if ( strstr ( pPC->ParamName, (char*)pNodes[n].pOP->pPool->Name ))
          {
          pPC->Command = CParamClicked::ВыделитьТрубу;
          lstrcpy ( pPC->Object, (char*)pNodes[n].pOP->pPool->Name );
          return 0;
          }
        }
      }
    break;
	case sd_GetHelpPath:
		{
			lstrcpy((char*)pData, HELP_PATH);
			lstrcat((char*)pData, g_szHelpFileName);
			return 1;
		}
    }
	return 1;
	}

int CCollector::SaveState( )
{
	S_CLASS("W",_W)
		ASS(pSRFile->WStruct("FlowSumm", sizeof ( double ) * kIO, FlowSumm))
		return 0;
}

int CCollector::RestoreState( char * StrName )
{
	COPY_FIRST
	R_CLASS("W",_W)
	if ( lstrcmp ( StrName, "FlowSumm" ) == 0 )
	{
		ASS(pSRFile->RStruct( sizeof ( double ) * kIO, FlowSumm));
		return 0;
	}
	return 1;
}

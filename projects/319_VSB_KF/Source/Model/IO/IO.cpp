#include "stdafx.h"
#include "IO.h"
#include "Err.h"
//#include "CommProc.h"
#include "Connect.h"
#include "SetData.h"
#include "Work.h"

LPCSTR g_szHelpFileName = "Базовый источник.pdf";

CIO::CIO( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
, State("State",this)
//, Script ( this )
	{
  UNIMODEL
	ObjName = _ObjName;
    Model = "IO";
#include "IO_Clear.h"
  ClassRootNode = GetRootResource( );
  DWORD LastDefect = 0;
#undef MODEL
#define MODEL(ClassName,Default) \
    {\
    RootNode = NewDefect( );\
    ObjRoot = GetDefect( RootNode );\
    ObjRoot->Parent = ClassRootNode;\
    lstrcpy ( ObjRoot->Name, ObjName );\
    ObjRoot->Flags = CDef::Defects|CDef::Node|CDef::Active;\
    LastDefect = 0;\
    }
#include "Points.h"
  Type = None;
  pFlow_in = NULL;
  pFlow_out = NULL;
  pComp = NULL;
  ID_Comp = 0;
  Omega_Defect = 1.;
  FirstStepT = true;
  StateDir = 0;
  Закупорен = false;
  kStep = 0;
  SetFlow = true;
  Omega_min_Z = 0.;
  Omega_max_Z = 1000000. / mOmega;
  k_Omega_Z = 1.;
  CompDefect = 0;
  CalcPT = 0;
//  pSolid = NULL;
	}

CIO::~CIO()
	{
	}

int CIO::Init( int )
	{
	int Res = 0;
  NewComp( &ID_Comp, &pComp, "Состав из %s", ObjName.Str );
//  Reg_Flow_Z_Sol_kg.Work = UseSolidFlow;
	//#include "IO_Init.h"
	//#include "Points.h"
	//#include "ACS_Init.h"  
	//#include "ACS.h"
  //if ( Скрипт[0] )
  //  {
  //  Script.Init( Скрипт );
  //  }
  Mw_src = Mw_NH3;
	return 0;
	}

int CIO::Step0( )
	{
  if ( Type == IO )
    {
    Восстановление_работоспособности->On();
    Закупорка->On();
    Прекращение_подачи->Off();
    Возобновление_подачи->Off();
    Изменение_давления->Off();
    Изменение_температуры->Off();
    //if ( UseSolidFlow && UseSolid )
    //  {
    //  pSolid = new CSolid;
    //  }
    }
  if ( Type == In || Type == Atm )
    {
    Восстановление_работоспособности->Off();
    Закупорка->Off();
    Прекращение_подачи->On();
    Возобновление_подачи->On();
    Изменение_давления->On();
    Изменение_температуры->Off();
    }
  if ( Type == Out )
    {
    Восстановление_работоспособности->Off();
    Закупорка->Off();
    Прекращение_подачи->On();
    Возобновление_подачи->On();
    Изменение_давления->On();
    Изменение_температуры->On();
    }
	return 0;
	}

int CIO::Step1( )
	{
//  CompZ.Use = false;
//  if ( WorkAsSrc )
  SetComp( );
//  CompZ.Create( );
//!!!!!!!!!!!!!!!
//  if ( Omega > 100. )
//  Omega /= mOmega;
  CalcPT = 0;
	return 0;
	}

int CIO::StepT ( double dt )
	{
  Calc ( dt );
  kStep++;
	return 0;
	}

int CIO::SetData( int TypeData, void * pData )
	{
  switch ( TypeData )
    {
    case sd_ShowObjInfo:
      {
      Char<256>* pInfo = (Char<256>*)pData;
      pInfo->Prt( "%s: P(и)=%4.2lf, T=%4.2lf, F=%4.2lfкг", ObjName.Str, P_src-1., T_src, Flow_kg_in_out );
      }
    break;
    case sd_ParamClicked:
      {
      if ( Type != Out )
        return 1;
      CParamClicked * pPC = (CParamClicked*)pData;
      if ( lstrcmp ( pPC->ParamName, "Состав" ) == 0 )
        {
        pPC->Command = pPC->ОкноСоставов;
        return 0;
        }
      return 1; 
      }
    break;
    case sd_GetPressKIP:
      {
      *(double**)pData = &P_in;
      return 0;
      }
    break;
    case sd_GetCompInfo:
      {
      if ( Type != Out )
        return 0;
      CExtCompInfo * pI = (CExtCompInfo*)pData;
      pI->Cmol = Cmol;
      pI->IsComp = IsComp;
      pI->P = &P_src;
      pI->T = &T_src;
      pI->pLAST_GAS = &LAST_GAS;
      pI->pGas = pGas;
      pI->CompName = Composition;
//      pI->pSolid = pSolid;
      return sd_GetCompInfo;
      }
    break;
    case sd_SetCompData:
      {
      if ( Type != Out )
        return 0;
      CExtCompData * pD = (CExtCompData*)pData;
      memcpy ( Cmol, pD->Cmol, sizeof ( Cmol ));
      memcpy ( IsComp, pD->IsComp, sizeof ( IsComp ));
      pComp->Set( IsComp, Cmol );
      _Mw = Mw_src = ::Mw ( pComp );
      Set_PTE( );
      if ( pD->SaveComp )
        {
        if ( pD->CompName )
          pComp->Write ( pD->CompName );
        else
          pComp->Write ( Composition );
        }
      return sd_SetCompData;
      }
    break;
    case sd_SetConnections:
      {
      if ( Type == IO )
        *(int*)pData = 2;
      else
        *(int*)pData = 1;
      }
    break;
    }
	return 1;
	}

int CIO::SaveState( )
  {
	S_CLASS("W",_W)
//  CSolid::SaveData( "Solid", pSolid );
  return 0;
  }

int CIO::RestoreState( char * StrName )
  {
  COPY_FIRST
	R_CLASS("W",_W)
//  if ( CSolid::RestData( "Solid", StrName, &pSolid ))
//    return 0;
  return 1;
  }

int CIO::SetProp ( struct CObjProps * pProp )
  {
  //if ( pProp->PropName == "Скрипт" )
  //  {
  //  Скрипт = pProp->Value;
  //  }
  return 0;
  }

#include "IO_Props.h"
int CIO::GetProp( )
  {
//  PARM("Скрипт",'U',Скрипт)
  return 0;
  }
#ifdef LINUX
#define EXP Q_DECL_EXPORT
#endif

extern "C"
  {
  EXP CProps Properties[] = {
    'S', "Скрипт", "Скрипт", "",
    0, NULL, 
    };
  }

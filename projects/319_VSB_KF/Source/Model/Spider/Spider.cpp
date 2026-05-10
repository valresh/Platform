#include "stdafx.h"
#define DLL_Spider
#include "Spider.h"
#include "math.h"
#include "Err.h"
#include "Data.h"
#include "HydroStruct.h"

typedef 
struct IBaseModel* (*tCreateObject)( LPCTSTR ObjName, LPCTSTR Type );

IBaseModel * Load_Obj ( IBaseModel * pMain, char * ObjName, char * Model, char * SubType, char * DIR )
  {
  char * DLL = Model;
  HMODULE hLib = NULL;
  if( !strcmp( DIR, "SYSTEM"))
    hLib = (HMODULE)Load_Dll( DLL );
  else 
    hLib = (HMODULE)Load_Dll( DLL, DIR );
  if ( hLib == NULL )
    {
    ModelMsg( pMain, "Ошибка загрузки DLL '%s' для модели '%s'", DLL, Model );
    return NULL;
    }
  tCreateObject pCreate = (tCreateObject)::GetProcAddress( hLib, "CreateObject" );
  if ( pCreate == NULL )
    {
    ModelMsg( pMain, "Ошибочная структура DLL '%s'", DLL );
    return NULL;
    }
  IBaseModel * pModel = (*pCreate) ( ObjName, SubType );
  if ( pModel == NULL )
    {
    ModelMsg( pMain, "Ошибка создания объекта '%s'('%s')", ObjName, Model );
    return NULL;
    }
  pModel->Model = Model;
  return pModel;
  }

CSpider::CSpider( IBaseModel * _pMain, char * pAltObjName, char * pAltModelName, char * pAltSubType, char * pAltDIRName ) : IBaseModel ( "Spider", _pMain )
  {
  Model = "Spider";
  pAltModel = Load_Obj ( this, pAltObjName, pAltModelName, pAltSubType, pAltDIRName );
  }

int CSpider::Init( int )
  {
  for ( int n = 0; n < kPoints; n++ )
    {
    ppObjPoints[n]->pPool = ppMainPoints[n]->pPool;
    }
  return 0;
  }

int CSpider::Step0()
  {
  return 0;
  }

int CSpider::Step1()
  {
  return 0;
  }

int CSpider::PreStepT()
  {
  return 0;
  }


int CSpider::StepT(double dt)
  {
  return 0;
  }

#include "IO_Parms.h"
int CSpider::GetParams( char * StrName )
  {
  TAB("Параметры",1)
  ETAB
  return 0;
  }

int CSpider::SaveState( )
  {
  return 0;
  }

int CSpider::RestoreState ( char * StrName )
  {
  return 0;
  }

int CSpider::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( pAltModel == NULL || pAltModel->pUniModel == NULL )
    return 0;
  kPoints = kNodes;
  ppObjPoints = NewArr( CObjectPoint *, kPoints );
  ppMainPoints = NewArr( CObjectPoint *, kPoints );
  for ( int n = 0; n < kPoints; n++ )
    {
    CObjectPoint * pOP = new CObjectPoint( );
    memset ( pOP, 0, sizeof ( CObjectPoint ));
    pOP->ObjName = ppNodes[n]->ObjName;
    pOP->PntName = ppNodes[n]->PntName;
    ppObjPoints[n] = pOP;
    ppMainPoints[n] = ppNodes[n];
    }
  int Res = pAltModel->pUniModel->TestNodes( kNodes, ppObjPoints );
  if ( Res )
    {
    ModelMsg( pMain, "Ошибка проверки точек подключения '%s'('%s')", pAltModel->ObjName.Str, pAltModel->Model.Str );
    return 1;
    }
// Группы 
  for ( int n = 0; n < kPoints; n++ )
    {
    AddGroup( ppObjPoints[n]->PntGroup, ppObjPoints[n]->NumbInGroup );
    }
  for ( int n = 0; n < Groups.L; n++ )
    {
    CPntGroup & G = Groups[n];
    int N = G.kPoints;
    G.Var = NewArr(double, 2 * N );
    G.pFlow = NewArr(CFlow*, N );
    G.A = NewArr(double, 2 * N * N );
    G.B = NewArr(double, N );
    }
  Char<128>Name; 
  for ( int n = 0; n < kPoints; n++ )
    {
    CObjectPoint * pOP = ppObjPoints[n];
    for ( int g = 0; g < Groups.L; g++ )
      {
      CPntGroup & G = Groups[g];
      if ( G.PntGroup == pOP->PntGroup )
        {
        int nP = pOP->NumbInGroup;
        ASS( nP < G.kPoints )
        Name.Prt ( "%s.%s|Spider", pOP->ObjName.Str, pOP->PntName.Str );
        G.pFlow[nP] = (CFlow*)Create ( Name, CFlow::TypeID, nP );
        break;
        }
      }
    }
  return 0;
  }

int CSpider::SelectParams( )
  {
  if ( CParams::ReadProps || CParams::SaveAllParams )
    return 0;
  if ( CParams::GetParamsList )
    return 0;
  if ( GetAsyncKeyState( VK_CONTROL ) & 0x8000 )
    {
    GetParams( "" );
    return -1;
    }
  if ( GetAsyncKeyState( VK_SHIFT ) & 0x8000 )
    {
    if ( pAltModel )
      pAltModel->GetParams( "" );
    return -1;
    }
  return 0;
  }

int CSpider::SetMatr( void * pExternals, int PntGroup,  double dt, 
                    double A[/* kExkV */], double B[ /*kE*/ ] )   // A * X = B
  {
  if ( pAltModel == NULL || pAltModel->pUniModel == NULL )
    return 1;
  for ( int g = 0; g < Groups.L; g++ )
    {
    CPntGroup & G = Groups[g];
    if ( G.PntGroup == PntGroup )
      {
      pAltModel->pUniModel->SetMatr( pExternals, PntGroup,  dt, G.A, G.B );
      break;
      }
    }
  return 0;
  }


int CSpider::SetVar( void * pExternals, int PntGroup, double dt, 
                   // Переменные на входах
                   double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  if ( pAltModel == NULL || pAltModel->pUniModel == NULL )
    return 1;
  for ( int g = 0; g < Groups.L; g++ )
    {
    CPntGroup & G = Groups[g];
    if ( G.PntGroup == PntGroup )
      {
      CBaseStruct * pObjVar[1024];
      for ( int p = 0; p < G.kPoints; p++ )
        {
        CFlow * pFlow = (CFlow*)pVar[p];
        if ( pFlow ->Flow_mol > 0. )
          {
// В аппарат
          pObjVar[p] = pVar[p];
          }
        else
          pObjVar[p] = G.pFlow[p];
        }
      pAltModel->pUniModel->SetVar( pExternals, PntGroup, dt, Var,	pObjVar );
      break;
      }
    }
  return 0;
  }

void CSpider::AddGroup ( int PntGroup, int NumbInGroup )
  {
  for ( int n = 0; n < Groups.L; n++ )
    {
    CPntGroup & G = Groups[n];
    if ( G.PntGroup == PntGroup )
      {
      if ( NumbInGroup + 1 > G.kPoints )
        G.kPoints = NumbInGroup + 1;
      return;
      }
    }
  CPntGroup & G = Groups.push_back();
  G.PntGroup = PntGroup;
  G.kPoints = NumbInGroup + 1;
  }
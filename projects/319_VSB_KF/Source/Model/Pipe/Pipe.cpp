#include "stdafx.h"
#include "Pipe.h"
#include "Err.h"
#include "CommProc.h"
//#include "Data.h"
#include "SetData.h"

#ifdef LINUX
#else
#include "Work.h"
#include "Start.h"
LPCSTR g_szHelpFileName = "Базовый трубопровод.pdf";
#endif



//char CPipe::PipeMsg[128];

//double CPipe::M = 0.01;
bool CPipe::Очищать = false;

CPipe::CPipe( char * _ObjName ) :	IBaseModel ( _ObjName ) 
, S_State ( "State", this )
, S_Cool( "Cool", this )
, S_Razgerm("Разгерметизация",this)
#include "ACS_Constr.h"
#include "ACS.h"
	{
  UNIMODEL
  POOLCTRL
  int L = sizeof(*this);
  int Lw = sizeof(_W);
  int Lo = sizeof(Cmol_Out);
    ObjName = _ObjName;
  #include "IO_Clear.h"
  #include "Points.h"
  #include "Pipe_Defects.h"
  IsMF = false;
  kF = 0;
  Matr = NULL;
  pNodes = NULL;
  kStep = 0;
  TempCalcT = false;
  ID_Comp_out = 0;
  pComp_out = NULL;
  Model = "Труба";
  nFlow_C = 0;
  Razgerm = false;
  Omega_Razgerm = 0.;
  FlowQ = 0.;
  Vol = 0.;
  SaveFlow = false;
//  pBaseObj = NULL;
  //pControl_P = NULL;
  //pControl_F = NULL;
  //pControl_Stable = NULL;
  //Filtr_Flow = NULL;
  F_Razgerm_max = 0.;
  FlowNames = NULL;
//  Max_Delta_P = 0.;
  Out_Drain = false;
  pComp_out_drain = NULL; // С учетом отбора дренажами
  ID_Comp_out_drain = 0;
  h_Out_drain = 0.;
  UseEtalon = false;
  pEtalonComp = NULL;
  Test_Flow_mol = NULL;
  Alarm_Flow_mol = false;
#ifdef N2
  IsN2 = false;
#endif
	}

CPipe::~CPipe()
	{
	}

int CPipe::Init( int )
  {
  CNoErrGasID::NoErr = true;
//??  G_Сульфолан = GetGasID( "SULFOLANE" );
//??  G_Триэтиленгликоль = GetGasID( "TRIETHYLENE GLYCOL" );
  int Res = 0;
  NewComp( &ID_Comp_out, &pComp_out, "Труба %s", ObjName.Str );
  NewComp( &ID_Comp_out_drain, &pComp_out_drain, "Труба %s после дренажей", ObjName.Str );
  pComp_out->Set( pN2 );
  CLEAR(IsComp_In)
  CLEAR(Cmol_In)
  IsComp_In[G_N2] = true;
  Cmol_In[G_N2] = 1.;  
  To_kg = Mw(pComp_out);
 /* if ( Use_Filtr_Flow )
    {
    Filtr_Flow = NewArr(CFiltrFlow,kF);
    }*/
  ShowP.Init( this, &P );
  ShowFlow.Init ( this, &FlowSumm );
  for ( int n = 0; n < kF; n++ )
    {
    pShowFlow[n] = new CShowF();
    pShowFlow[n]->Init( this, NULL );
    pShowFlow[n]->Sign = Type[n];
    pShowFlow[n]->pMes = &ShowFlow.Mes;
    }
  Show.Init( this );
  P = 1.;
  T = T_air;
 	return 0;
	}

 
int CPipe::Step0( )
	{
  if ( UseEtalon )
    pEtalonComp = (IEtalonComp*)::GetProp( "Рабочие", "Эталонный состав" );
	return 0;
	}

int CPipe::Step1( )
	{
  CompZ.W = NULL;
	if( CompZ.W )
	  CompZ.W->Comp.Read(CompZ.W->CompName);
  ///########################################################################
  //Vol = 0.01;
  IsMF = false;
  //char * P = ObjName;
  //if ( *P == '_' )
  //  pBaseObj = IBaseModel::Find( ObjName + 1 );
  //if ( Test_P )
  //  pControl_P = new CAnalogControl ( "Давление", this, &_P, &P_test_nom, &P_test_delta );
  //if ( Test_F )
  //  {
  //  pControl_F = new CAnalogControl ( "Поток", this, &_F_kg, &F_test_nom, &F_test_delta );
  //  pControl_Stable = new CStableControl ( "Стабильность", this, &_F_mol );
  //  }
  FlowSumm.Flow_mol = F_mol;
  FlowSumm.P = P;   
  FlowSumm.T = T;   
  FlowSumm.pComp = pComp_out;
  FlowSumm.h_Flow = h_Flow;
  FlowSumm.ID_COMP = ID_Comp_out;
  extern int TypeFlow_C;
  TypeFlow_C = 0;
  if ( pEtalonComp )
    pEtalonComp->Init( );
  return 0;
	}

int CPipe::StepT ( double dt )
	{
  Calc ( dt );
  kStep++;
  //if ( pControl_P )
  //  pControl_P->Test();
  //if ( pControl_Stable )
  //  pControl_Stable ->Test();
	return 0;
	}

struct CGetPipeFlowData
  {
  double * pFlow;
  CComp * pComp;  
  };

extern char Alarm_Pipe[128];

int CPipe::SetData( int TypeData, void * pData )
	{
  //char * P1 = ObjName;
  //if ( pBaseObj == NULL && *P1 == '_' )
  //  pBaseObj = IBaseModel::Find( P1 + 1 );
  //if ( pBaseObj )
  //  {
  //  return pBaseObj->SetData( TypeData, pData );
  //  }

  if ( TypeData == sd_GetOverGasSource )
    {
    *(double**)pData = &FlowQ; 
    return sd_GetOverGasSource;
    }
  if ( TypeData == sd_SetParmsData )
    {
    if ( pEtalonComp && nEtalonFlow >= 0 )
      pEtalonComp->Refresh( pFlow[nEtalonFlow] );
    PrepareParams ( (CParams*)pData );
    return 0;
    }
  switch ( TypeData )
    {
    case sd_SetInitState:
      {
      P = 1.;
      T = 15.;
      if ( pComp_out )
        pComp_out->Set ( pN2 );
      return 0;
      }
    case sd_PipeState:
      {
        TempCalcT = true;
      Calc ( pSys->dt );
      }
    return 0;
    case sd_GetPipeFlow:
      {
      CGetPipeFlowData * pD = (CGetPipeFlowData *)pData;
      pD->pComp = pComp_out;
      pD->pFlow = &F_mol;
      return sd_GetPipeFlow;
      }
    case sd_ShowObjInfo:
      {
      Char<256>* pInfo = (Char<256>*)pData;
      if ( F_mol > MAX_FLOW )
        F_mol = MAX_FLOW;
      if ( F_mol < -MAX_FLOW )
        F_mol = -MAX_FLOW;
      if ( P < 0.0001 )
        P = 0.0001;
      if ( P > MAX_PRESS )
        P = MAX_PRESS;
      pInfo->Prt( "%s: P(и)=%4.2lf, T=%4.2lf, F=%4.2lfкг", ObjName.Str, P-1., T, F_mol * To_kg );
      }
    break;
    case sd_GetPipeFixed:
      {
      if ( pData == NULL )
        return 1;
      if ( Reg_T.IsOn() || Reg_P.IsOn() || CompZ.Use || ( UseEtalon && EtalonToOut > 0 ))
        {
        *(bool*)pData = true;
        break;
        }
#ifdef N2
      if ( IsN2 )
        {
        *(bool*)pData = true;
        break;
        }
#endif

#ifdef SPECIAL
      if ( Alarm_Flow_mol )
        {
        *(bool*)pData = true;
        break;
        }
#endif
      for ( int n = 0; n < kF; n++ )
        {
        CFlow * pF = pFlow[n];
        if ( pF == NULL )
          continue;
        if ( fabs ( pF->Flow_mol ) >= 490000. )
          {
          *(bool*)pData = true;
          break;
          }
        }
      }
    break;
    case sd_GetPipeVol:
      {
      if ( pData == NULL )
        return 1;
	  if(Vol > 0.01)
		  *(bool*)pData = true;      
      }
    break;
    case sd_ParamsWillShow:
      TempCalcT = true;
    break;
    case sd_GetCompInfo:
      {
      CExtCompInfo * pI = (CExtCompInfo*)pData;
      pI->Cmol = pComp_out->Cmol;
      pI->IsComp = pComp_out->IsComp;
      pI->P = &P;
      pI->T = &T;
      pI->CompName = NULL;
      return sd_GetCompInfo;
      }
    break;
    case sd_GetSummCompData:
    case sd_GetGasCompData:
    case sd_GetLiqCompData:
      {
      if ( pComp_out )
        *(double**)pData = pComp_out->Cmol;
      return sd_GetSummCompData;
      }
    break;
	  case sd_GetFlowInfo:
		  {
			CExtFlowInfo * pI = (CExtFlowInfo*)pData;
			for ( int n = 0; n < kF; n++ )
			  {
				if ( pNodes[n].ObjName.Str == NULL )
					continue;
				if(!lstrcmp(pNodes[n].ObjName, pI->szObjName))
				  {
					pI->Flow = Flow_mol[n];
					break;
				}
			}
			return sd_GetCompInfo;
		}
	break;
    case sd_GetComp:
      {
      *(void**)pData = pComp_out;
      return sd_GetComp;
      }
    break;
    case sd_SetEquipParam:
      {
      }
    break;
    case sd_SetConnections:
      {
      char * SrcName = *(char**)pData;
      if ( kF != 2 )
        return 1;
      if ( pNodes[0].ObjName == SrcName )
        {
        *(char**)pData = (char*)pNodes[1].ObjName;
        return sd_SetConnections;
        }
      if ( pNodes[1].ObjName == SrcName )
        {
        *(char**)pData = (char*)pNodes[0].ObjName;
        return sd_SetConnections;
        }
      return 2;
      }
    break;
#ifdef LINUX
#else
    case sd_Convert_1_2:
      {
        if(pConvert_1_2)
          pConvert_1_2(pComp_out->IsComp, pComp_out->Cmol, true);
        break;
      }
    case sd_GetHelpPath:
      {
        lstrcpy((char*)pData, HELP_PATH);
        lstrcat((char*)pData, g_szHelpFileName);
        return 1;
      }
#endif
    }
	return 1;
	}

int CPipe::SaveState( )
  {
	S_CLASS("W",_W)
  if ( pEtalonComp )
    pEtalonComp->SaveState( "EtalonComp" );
  return 0;
  }

int CPipe::RestoreState( char * StrName )
  {
  if ( pEtalonComp )
    {
    if ( pEtalonComp->RestState( StrName, "EtalonComp" ) == 0 )
      return 0;
    }
	R_CLASS("W",_W)
  return 1;
  }

//CMem<CStr,128,128>Коллектора;
//bool WasLoad = false;

bool IsCollector( char * ObjName )
  {
//  if ( !WasLoad )
//    {
//    Char<256>Path;
//    }
  return false;
  }
///////////////////////////////////////////////////////////////////
CShowPipeParams::CShowPipeParams()
  {
  pPipe = NULL;
  }

void CShowPipeParams::Init ( class CPipe * _pPipe )
  {
  pPipe = _pPipe;
  }

void CShowPipeParams::Show ( struct CParams * Param )
  {
  if ( pPipe )
    pPipe->PrepareParams ( Param );
  }


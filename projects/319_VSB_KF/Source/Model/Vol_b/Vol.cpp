#include "stdafx.h"
#include "Vol.h"
#include "Err.h"
//#include "CommData.h"
//#include "Data.h"
#include "SetData.h"
#include "Defects.h"
#include "Prop.h"
#include "Extensions.h"
#include "CommProc.h"

LPCSTR g_szHelpFileName = "Базовая емкость.pdf";

//CVisioModelMenu Menu[32];
//int kMenu;

int kVol;

CVol::CVol( char * _ObjName, bool _Calc_2F, bool FullMatr ) :	IBaseModel ( _ObjName ) 
, VolBase ( "Base", this )
, Heat ( "Нагреватель", this )
, Heat_2 ( "Нагреватель 2", this )
, Heat_3 ( "Нагреватель 3", this )
, SaveVar ( 8 )
, LevCtrl ( "уровень", this, &VolBase.Lev_liq )
, Stable ( "поток", this, &Flow_Ctrl )
#include "ACS_Constr.h"
#include "ACS.h"
	{
  UNIMODEL
  ObjName = _ObjName;
  #include "IO_Clear.h"
  #include "Points.h"
  #include "Vol_Defects.h"
  pNodes = NULL;
  kIO = 0;
  Calc_2F = _Calc_2F;
  Напорная = FullMatr;
  CalcCompGas = false;
  CalcCompLiq = false;
  pFlow_heat_in = NULL;
  pFlow_heat_out = NULL;
  pFlow_heat_in_2 = NULL;
  pFlow_heat_out_2 = NULL;
  pFlow_heat_in_3 = NULL;
  pFlow_heat_out_3 = NULL;
  Omega = NULL;
//  kOmega = NULL;
  pFlow = NULL;
  UseTopIn = NULL;
  Направление = NULL;
  KsiTop = 0.;
//  dPHydro = NULL;
  dPGydro_Static = NULL;
//  dP_Hydro = NULL;
  dP_node = NULL;
  HeatDefect = false;
  pFlowOutZ = NULL;
  Mw_Flow = NULL;
  Flow_No_This = NULL;
  dPHydro = NULL;
  dP_Hydro = NULL;
  kOmega = NULL;
  FlowQ = 0.;
  DefectUplot = false;
  kNodes = 0;
  Flow_Ctrl = 0.;
  //pHeat = NULL;
  pExtHeat = NULL;
  PseudoWater = false;
  pPseudoWater = NULL;
  kCommIO = 0;
  HaveProperties = true;
  Lev_old = -1;
  UseHeater = false;
  CLEAR(UseHeat)
  Omega_Heat = Omega_Heat_2 = Omega_Heat_3 = 1.;
  k_Heat = k_Heat_2 = k_Heat_3 = 100.;
  S_Heat = S_Heat_2 = S_Heat_3 = 20;
  Q_Heat_1 = Q_Heat_2 = Q_Heat_3 = 0.;
  Cond_Heat = Cond_Heat_2 = Cond_Heat_3 = false;
  Q_Heat = 50.;
  VolBase.Calc_2F = Calc_2F;
  Разгерметизация = false;
  ParamsUsed = true;
  Объема_2 = false;
  k_Cond = 1.;
  UseSumm = false;
  CompLiqGas = 0;
  CompMolMass = 0;
  CLEAR(ShowComp)
  Comp12 = 0;
  Comp2FMolMass = 0;
  CLEAR(ShowComp2F)
  Type_Flow = 0;
  nCompBalance = 0;
  CLEAR(Calc_C)
  OldCompLiqGas = 0;
  M_old = 0.;
  kVol++;
	}

CVol::~CVol()
	{
	}

int CVol::Init( int )
	{
	int Res = 0;
  KsiTop = 1.;
  if ( HaveExtensions )
    ReadExtension ( this );
  VolBase.InitData( kIO );
  ShowP.Init( this, &VolBase.P );
  DWORD NodeDefect = 0;
//  NODE("Фланцы")
  Char<256> Name;
//  Fsend("Trace.txt","0\n" );
  for ( int n = 0; n < kIO; n++ )
    NumbNode[n] = -1;
  for ( int nN = 0; nN < kNodes; nN++ )
    {
    int n = pNodes[nN].nIO;
    if ( n < 0 )
      continue;
    ASS( n >=0 && n < kIO )
    NumbNode[n] = nN;
    }
  for ( int n = 0; n < kIO; n++ )
    {
    ASS( NumbNode[n] >= 0 )
    }
  pFlowProps = NewArr(double,kIO);
  for ( int n = 0; n < kIO; n++ )
    {
    int nN = NumbNode[n];
    Omega[n] = 10000.;
    kOmega[n] = 1.;
    pFlow[n] = NULL;
    UseTopIn[n] = true;
    Направление[n] = 0;
    if ( pNodes[nN].pOP->PntName[0] == 't' )
      VolBase.H_node[n] = 1.;
    else
      VolBase.H_node[n] = 0.;
    KsiTop = 1.;
    dP_Hydro[n] = 0.;
	  dPHydro[n] = 0.;
	  dPGydro_Static[n] = 0.;
    dP_node[n] = 0.;
    pFlowProps[n] = 0;
//
    Name.Prt( "Состав входа '%s' %s", (char*)pNodes[nN].pOP->PntName, (char*)ObjName );
    VolBase.ID_Out[n] = Get_ID_Data ( Name, CComp::TypeID );
    VolBase.pComp_Out[n] = Get_Comp ( VolBase.ID_Out[n] );
    VolBase.pComp_Out[n] ->Set ( pN2 );
    }
  //VolBase.VolInit ( pN2, 1., T_air );
  pУровень = new CAnalogD ( this, "Уровень", "AO", "Уровень жидкости 0-100" );
  if ( Calc_2F )
    {
    pУровень_продукта = new CAnalogD ( this, "Уровень_продукта", "AO", "Уровень органической фазы 0-100" );;
    pУровень_воды = new CAnalogD ( this, "Уровень_воды", "AO", "Уровень водной фазы 0-100" );;
    }
  else
    {
    pУровень_продукта = NULL;
    pУровень_воды = NULL;
    }
  CompGas.Set ( pN2 );
  CompLiq.Set ( pWater );
  Show_Comp.Init( this );
  Show_2F.Init( this );
  Show_Flow.Init( this );
  return 0;
	}

int CVol::Step0( )
  {
	  VolBase.VolInit ( pN2, 1., T_air );
  return 0;
	}

int CVol::Step1( )
	{
  if ( VolBase.Vol2F > 0.1 )
    Объема_2 = true;
  else
    Объема_2 = false;
  //if ( strstr ( ObjName, "Е-101" ))
  //  KKK();
  //if ( Reg_L_1 > 0. )
  //  {
  //  VolBase.LevZ = 0.;
  //  Reg_L_1 = 0.;
  //  }
  if ( !_finite(Flow_summ_out))
    Flow_summ_out = 0.;
  //if ( lstrcmp ( ObjName, "Т-20а") == 0 || lstrcmp ( ObjName, "Т-20") == 0 )
  //  State = 1;
  //if ( lstrcmp ( ObjName, "Т-22") == 0 )
  //  State = 1;
  if ( State == 1 ) 
    {
    VolBase.Gas_Vol = 40.;// Времянка для 158 проекта
    VolBase.VolInit ( pN2, 1., T_air );
    }
  //if ( lstrcmp ( ObjName, "E-23") == 0 )
  //  {
  //  VolBase.VolInit ( pN2, 1., T_air );
  //  CompGas.Set ( pN2 );
  //  CompLiq.Set ( pWater );
  //  Show_Comp.Init( this );
  //  Show_2F.Init( this );
  //  Show_Flow.Init( this );
  //  }
	return 0;
	}

int CVol::StepT ( double dt_s )
	{
#ifdef INIT_STATE
  VolBase.VolInit ( pN2, 1., T_air );
#endif
  dt_w += 0.01 * ( dt_s - dt_w );
  double dt = dt_w;
  _dt = dt * 1e4;
  Calc ( dt );
  LevCtrl.Test();
  Stable.Test();
  VolBase.kStep = 0;
	return 0;
	}

#define LEV_50    0
#define LEV_80    1
#define CONN_ATM  2

char CompList[256] = {0};

int CVol::SetData( int TypeData, void * pData )
	{
  if ( TypeData == sd_SetParmsData )
    {
    return 1;
    }
  if ( TypeData == sd_SetInitState )
    {
    VolBase.VolInit ( pN2, 1., T_air );
	  OverFlow = false;
    for ( int i = 0; i < kIO; i++ )
      {
      VolBase.pComp_Out[i] ->Set ( pN2 );
      dPGydro_Static[i] = 0.;
      dP_node[i] = 0.;
      P_Slow[i] = 1.;
      Flow_No_This[i] = 0.;
      dPHydro[i] = 0.;
      dP_Hydro[i] = 0.;;
      kOmega[i] =1.;
      }
    return 0;
    }
  if ( TypeData == sd_ParamsWillShow )
    {
    ParamsUsed = true;
    return 0;
    }
  if ( TypeData == sd_GetOverGasSource )
    {
    *(double**)pData = &FlowQ; 
    return sd_GetOverGasSource;
    }
  if ( TypeData == sd_ParamClicked )
    {
    CParamClicked * pPC = (CParamClicked*)pData;
    if ( SelectNodePipe( pPC, Static_Out_gas, "Место вывода газа" ))
      return 0;
    if ( SelectNodePipe( pPC, Static_Out_1F, "Место вывода 1-ой(единственной) фазы" ))
      return 0;
    if ( SelectNodePipe( pPC, Static_Out_2F, "Место вывода 2-ой фазы" ))
      return 0;
    if ( SelectNodePipe( pPC, numbKlapReg, "Выход регулировки уровня" ))
      return 0;
    if ( SelectNodePipe( pPC, nFlow_summ, "Выход при переполнении" ))
      return 0;
    Char<128>Name;
    for ( int n = 0; n < kNodes; n++ )
      {
      Name.Prt("%s ", (char*)pNodes[n].pOP->PntName );
      if ( strstr ( pPC->ParamName, Name ))
        {
        pPC->Command = CParamClicked::ВыделитьТрубу;
        lstrcpy ( pPC->Object, pNodes[n].pOP->pPool->Name );
        return 0;
        }
      }
    return 1;
    }
  if ( TypeData == sd_ShowObjInfo )
    {
    Char<256>* pInfo = (Char<256>*)pData;
    if ( _P > -1. && _P < 1000. )
      pInfo->Prt( "%s: P(и)=%4.2lf, T=%4.2lf, L=%4.1lf%%", (char*)ObjName, _P, _T, _Lev_liq );
    else
      pInfo->Prt( "%s: P(и)=#, T=%4.2lf, L=%4.1lf%%", (char*)ObjName, _T, _Lev_liq );
    return 0;
    }
#if 0
  if ( TypeData == sd_AddToTrends )
    {
#include "IO_Trends.h"
    PARM_D( _P, "Давление(а)", 0. )
    PARM_D( _T, "Температура", 0. )
    PARM_D( _Lev_liq, "Уровень жидкости %", 0. )
    return 0;
    }
#endif
#undef IF
#define IF(Parm) if ( strcmp ( pP->Name, Parm ) == 0 ) {
#undef EIF
#define EIF return 0; }
  if ( TypeData == sd_SetEquipParam )
    {
    CEquipParam * pP = (CEquipParam*)pData;
    IF("#Заданный объем")
    if ( pP->Value[0] == 0 )
      return 0;
    double Vz = atof( pP->Value );
    if ( Vz != VolBase.Vol )
      {
      VolBase.VolZ = Vz;
      if ( VolBase.VolZ < 1. )
        VolBase.VolZ = 1.;
      }
    EIF
    }
  if ( TypeData == sd_GetCompInfo )
    {
    CExtCompInfo * pI = (CExtCompInfo*)pData;
    pI->IsComp = VolBase.IsComp;
    pI->P = &VolBase.P;
    pI->T = &VolBase.T;
    pI->CompName = ObjName;
    if ( GetAsyncKeyState ( VK_SHIFT ) && 0x8000 )
      pI->Cmol = VolBase.Cmol_Gas;
    else
      pI->Cmol = VolBase.Cmol_Liq;
    return sd_GetCompInfo;
    }
   if ( TypeData == sd_GetGasCompData )
     {
      *(double**)pData = VolBase.Cmol_Gas;
      return sd_GetGasCompData;
     }
   if ( TypeData == sd_GetSummCompData )
     {
      *(double**)pData = VolBase.CompVol.Cmol;
      return sd_GetSummCompData;
     }
   if ( TypeData == sd_GetLiqCompData )
      {
      *(double**)pData = VolBase.Cmol_Liq;
      return sd_GetLiqCompData;
      }
  if ( TypeData == sd_GetComp )
    {
    CGetComp * pC = (CGetComp*)pData;
    int nSt = pC->nSt;
    if ( nSt < 1 )
      nSt = 1;
    if ( nSt > 2 )
      nSt = 2;
    pC->nSt = nSt;
    memmove ( pC->IsComp, VolBase.IsComp, sizeof ( pC->IsComp ));
    memmove ( pC->Henry, VolBase.StateVol.Henry, sizeof ( pC->Henry ));
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( VolBase.IsComp[n] )
        pC->Faza[n] = 1;
      }
    pC->P = VolBase.P;
    pC->T = VolBase.T;
    if ( nSt == 1 )
      memmove ( pC->Cmol, VolBase.Cmol_Liq, sizeof ( pC->Cmol ));
    if ( nSt == 2 )
      memmove ( pC->Cmol, VolBase.Cmol_Gas, sizeof ( pC->Cmol ));
    return sd_GetComp;
    }
  if ( TypeData == sd_GetCompData )
    {

    return sd_GetCompData;
    }
  if (TypeData == sd_GetHelpPath)
  {
	  lstrcpy((char*)pData, HELP_PATH);
	  lstrcat((char*)pData, g_szHelpFileName);
	  return 1;
  }
  return 1;
	}



bool CVol::SelectNodePipe( CParamClicked * pPC, int nSelectedNode, char * ParamName )
  {
  if ( lstrcmp ( pPC->ParamName, ParamName ))
    return false;
  if ( nSelectedNode < 0 || nSelectedNode >= kNodeNames )
    return false;
//
/*
  Char<128>Name;
// Название точки
  char Names[1024];
  strcpy_s ( Names, 1024, NodeNames );
  char * Pb = Names;
  char * Pe = NULL;
  int n = nSelectedNode;
  while ( 1 )
    {
    Pe = strchr ( Pb, ';' );
    if ( Pe == NULL )
      return false;
    if ( n == 0 )
      break;
    n--;
    Pb = Pe + 1;
    }
  *Pe = 0;
  char NodeName[32];
  strcpy_s ( NodeName, 32, Pb );
  for ( int n = 0; n < kNodes; n++ )
    {
    if ( lstrcmp ( NodeName, (char*)pNodes[n].pOP->PntName ) == 0 )
      {
      pPC->Command = CParamClicked::ВыделитьТрубу;
      lstrcpy ( pPC->Object, pNodes[n].pOP->pPool->Name );
      return true;
      }
    }
  return false;
*/  
  int N = NodeNumbs[nSelectedNode];
  pPC->Command = CParamClicked::ВыделитьТрубу;
  lstrcpy ( pPC->Object, pNodes[N].pOP->pPool->Name );
  return true;
  }

#undef S_DOUBLE
#define S_DOUBLE( Variable, kElements ) \
  ASS(Variable)\
  ASS(pSRFile->WStruct( #Variable, sizeof ( double ) * (kElements), Variable ));

#undef R_DOUBLE
#define R_DOUBLE( Variable, kElements ) \
	if ( lstrcmp ( StrName, #Variable ) == 0 )\
		{\
    ASS(Variable)\
		ASS(pSRFile->RStruct( sizeof ( double ) * (kElements), Variable ));\
		return 0;\
		}


int CVol::SaveState( )
  {
	S_CLASS("W",_W)
  if ( VolBase.pReactions )
    VolBase.pReactions->SaveState( "Реакции" );
//  SaveVar.Save("DATA");
  LevCtrl.SaveState( );
//  Stable.SaveState( );
  S_DOUBLE( Mw_Flow, kIO)
  S_DOUBLE( Flow_No_This, kIO)
  //S_DOUBLE( dPHydro, kIO)
  S_DOUBLE( dP_Hydro, kIO)
  S_DOUBLE( kOmega, kIO)
  return 0;
  }

int CVol::RestoreState( char * StrName )
  {
  COPY_FIRST
  R_CLASS("W",_W)
  if ( VolBase.pReactions && lstrcmp ( StrName, "Реакции" ) == 0 )
    VolBase.pReactions->RestoreState( StrName, "Реакции" );
  if ( LevCtrl.RestoreState( StrName ) == 0 )
    return 0;
  R_DOUBLE( Mw_Flow, kIO)
  R_DOUBLE( Flow_No_This, kIO)
  //R_DOUBLE( dPHydro, kIO)
  R_DOUBLE( dP_Hydro, kIO)
  R_DOUBLE( kOmega, kIO)
  //if ( lstrcmp ( StrName, "DATA") == 0 )
  //  return 1;//SaveVar.Rest();
  //if ( Stable.RestoreState( StrName ) == 0 )
  //  return 0;
  return 1;
  }

#include "MM.h"

int CVol::DstMM( char * DataName, struct IMM * pMM )// Вызов получателя сигнала
  {
  if ( strncmp( DataName, "Нагрев", 6 ) == 0 )
    {
    ASS( pMM->ID == 0x2A3BE4FF )
    pExtHeat = (double*)pMM->pData;
    return 0;
    }
  return 1;
  }

CShowVolComp::CShowVolComp()
  {
  pVol = NULL;
  }
void CShowVolComp::Init ( class CVol * _pVol )
  {
  pVol = _pVol;
  }

void CShowVolComp::Show ( struct CParams * Param )
  {
  if ( pVol )
    pVol->SetShowComp();
  }

CShow2FComp::CShow2FComp()
  {
  pVol = NULL;
  }
void CShow2FComp::Init ( class CVol * _pVol )
  {
  pVol = _pVol;
  }

void CShow2FComp::Show ( struct CParams * Param )
  {
  if ( pVol )
    pVol->Set2FComp();
  }

void CShowVolFlow::Init ( class CVol * _pVol )
  {
  pVol = _pVol;
  }

void CShowVolFlow::Show ( struct CParams * Param )
  {
  if ( pVol )
    pVol->SetFlowProps();
  }

CShowVolFlow::CShowVolFlow()
  {
  pVol = NULL;
  }

void CVol::LoadReactions( )
  {
  }

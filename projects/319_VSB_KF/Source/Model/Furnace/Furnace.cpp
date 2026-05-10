#include "stdafx.h"
#include "Furnace.h"
#include "Err.h"
#include "CommProc.h"
//#include "Data.h"
#include "SetData.h"

CFurnace::CFurnace( char * _ObjName, char * Param ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
	{
  UNIMODEL
	lstrcpy ( ObjName, _ObjName );
	#include "IO_Clear.h"
	#include "Points.h"
    #include "Furnace_Defects.h"
  pTube = NULL;
  pNodeTube = NULL;
  kProduct = 1;
  pTopl_Gas = NULL;
  pPilot = NULL;
  pAdd_Gas = NULL;
  pMazut = NULL;
  pMazutSteam = NULL;
  pSteam = NULL;
  pAir = NULL;
  pSmoke = NULL;
  kFors = 15;
  IsProgar = false;
  nProgar = 0;
  kGasVent = 1;
  kGas2Vent = 1;
  kPilotVent = 1;
  kStep = 0;
  pFirstPnt = NULL;
  Открыть_пилот = NULL;
  Розжиг_пилот = NULL;
  W_Tube = NULL;
//  CLEAR(Gas) 
//  CLEAR(Pilot) 
  Fuel[0] = 2.; 
  Fuel[1] = 4.; 
  Fuel[2] = 6.; 
  Fuel[3] = 8.; 
  Eff[0] = Eff[1] = Eff[2] = Eff[3] = 1.;
  kFlowIn = 1.;
  Конц_откр_газ = NULL;  
  Конц_закр_газ = NULL; 
  if ( Param && Param[0] )
    {
    char * P = strstr ( Param, "kF=");
    if ( P )
      kFors = atoi ( P + 3 );
    }
	}

CFurnace::~CFurnace()
	{
	}

int CFurnace::Init( int Regim )
	{
	int Res = 0;
	//#include "IO_Init.h"
	//#include "Points.h"
	//#include "ACS_Init.h"  
	//#include "ACS.h"
  //if ( Version == Коксование )
  //  {
  //  if ( strstr ( ObjName, "П-101" ) || strstr ( ObjName, "П-102" ))
  //    {
  //    kProduct = 1;
  //    kFors = 30;
  //    }
  // if ( strstr ( ObjName, "П-103" ))
  //    {
  //    kProduct = 4;
  //    kFors = 15;
  //    }
  //  if ( strstr ( ObjName, "П-201" ))
  //    {
  //    kProduct = 4;
  //    kFors = 4;
  //    }
  //  }
  //if ( Version == ВБ )
  //  {
  //  if ( strstr ( ObjName, "П-101" ))
  //    {
  //    kProduct = 2;
  //    kFors = 11;
  //    }
  //  }
  char Name[256];
  double W_1 = 1. / (double)kProduct;
  for ( int n = 0; n < kProduct; n++ )
    {
    sprintf_s ( Name, 255, "Труба %d", n + 1 );
    pTube[n] = new CRadTube ( Name, this );
    pTube[n]->pPech = this;
    W_Tube[n] = W_1;
    }
  pFire = NewArr(CRadFors*,kFors)
  Горит = NewArr(CDiscrete *,kFors); 
  Горит_пилот = NewArr(CDiscrete*,kFors); 
  Отказ_пилот = NewArr(CDiscrete*,kFors);
  Отказ_пламя = NewArr(CDiscrete*,kFors);
  Интенсивность_пламени = NewArr(CAnalog*,kFors);
  Интенсивность_пилот = NewArr(CAnalog*,kFors);
  Температура_на_выходе_труб = NewArr(CAnalog*,kProduct);
  Температура_змеевика = NewArr(CAnalog*,kProduct);
  Открыть_пилот = NewArr(CDiscrete *,kFors); 
  Розжиг_пилот = NewArr(CDiscrete *,kFors); 
  Конц_откр_газ = NewArr(CDiscrete *,kFors);  
  Конц_закр_газ = NewArr(CDiscrete *,kFors);   
  //if ( Version == АВТ_4 )
  //  {
  //  Открыть_пилот = NewArr(CDiscrete *,kFors); 
  //  Розжиг_пилот = NewArr(CDiscrete *,kFors); 
  //  }
  //if ( Version == ГО || Version == NH3_CHE )
  //  {
  //  Розжиг_пилот = NewArr(CDiscrete *,kFors); 
  //  }
  int N = 1;
  for ( int n = 0; n < kFors; n++ )
    {
    sprintf_s ( Name, 255, "%s Форсунка %d", ObjName, N++ );
    pFire[n] = new CRadFors ( Name, this );
    ASS ( ::GiveCommData( Name, CForsunka::TypeID, (void**)&pFire[n]->Logic.pFors ) == 0 );
    pFire[n]->pPech = this;
    pFire[n]->Logic.Init ( Name );
    pFire[n]->Logic.pFors->VentGas = 100.;
    pFire[n]->Logic.pFors->ShibGas = 80.;
    pFire[n]->Logic.pFors->VentMazut = 0.;
    //pFire[n]->pFors->VentMazut2 = 0.;
    //pFire[n]->pFors->ShibMazut = 0.;
    //pFire[n]->pFors->VentSteam = 0.;
    pFire[n]->Logic.pFors->VentSteam = 0.;
    pFire[n]->Logic.pFors->VentPilot = 100.;
    //pFire[n]->pFors->VentPilot2 = 100.;
//
    sprintf_s ( Name, 255, "Датчик_пламени_%d", n + 1 );
    Горит[n] = new CDiscrete( this, Name, "DO" );
    sprintf_s ( Name, 255, "Горит_пилот_%d", n + 1 );
    Горит_пилот[n] = new CDiscrete( this, Name, "DO" );
    sprintf_s ( Name, 255, "Открыть_пилот_%d", n + 1 );
    Открыть_пилот[n] = new CDiscrete( this, Name, "DI" ); 
    sprintf_s ( Name, 255, "Розжиг_пилот_%d", n + 1 );
    Розжиг_пилот[n] = new CDiscrete( this, Name, "DI" ); 
    //if ( Version == ГО || Version == NH3_CHE )
    //  {
    //  sprintf_s ( Name, 255, "Розжиг_пилот_%d", n + 1 );
    //  Розжиг_пилот[n] = new CDiscrete( this, Name, "DI" ); 
    //  }
    sprintf_s ( Name, 255, "Отказ_пилот_%d", n + 1 );
    Отказ_пилот[n] = new CDiscrete( this, Name, "DO" );
    *Отказ_пилот[n] = 0;
    sprintf_s ( Name, 255, "Отказ_пламя_%d", n + 1 );
    Отказ_пламя[n] = new CDiscrete( this, Name, "DO" );
    *Отказ_пламя[n] = 0;
    sprintf_s ( Name, 255, "Интенсивность_пламени_%d", n + 1 );
    Интенсивность_пламени[n] = new CAnalog( this, Name, "AO" );
    sprintf_s ( Name, 255, "Интенсивность_пилот_%d", n + 1 );
    Интенсивность_пилот[n] = new CAnalog( this, Name, "AO" );
    sprintf_s ( Name, 255, "Конц_откр_газ_%d", n + 1 );
    Конц_откр_газ[n] = new CDiscrete( this, Name, "DO" );
    sprintf_s ( Name, 255, "Конц_закр_газ_%d", n + 1 );
    Конц_закр_газ[n] = new CDiscrete( this, Name, "DO" );
    }
  for ( int n = 0; n < kProduct; n++ )
    {
    sprintf_s ( Name, 255, "Т на выходе трубы %d", n + 1 );
    Температура_на_выходе_труб[n] = new CAnalog( this, Name, "AO" );
    sprintf_s ( Name, 255, "Т змеевика %d", n + 1 );
    k_Zm[n]  = 0.2;
    Температура_змеевика[n] = new CAnalog( this, Name, "AO" );
    }
/////////////////
  //kT_Zm = 20;
  //T_Zm = NewArr(CAnalogD*,kT_Zm);
  //rnd_T_Zm = NewArr(double,kT_Zm);
  //for ( int n = 0; n < kT_Zm; n++ )
  //  {
  //  sprintf_s ( Name, 255, "T_Zm1_%d", n + 1 );
  //  T_Zm[n] = new CAnalogD( this, Name, "AO" );
  //  rnd_T_Zm[n] = 1. + 0.02 * ( 1. - 2. * r01());
  //  }
/////////////////
  NewComp(&ID_CompSmoke,&pCompSmoke," Дым из %s", ObjName );
  pCompSmoke->Set( pN2 );
  NewComp(&ID_CompGasZ,&pCompGasZ," Газ в %s", ObjName );
  pCompGasZ->Set( pN2 );
	return 0;
	}

int CFurnace::Step0( )
	{
  Mw_air = pAirFlow->To_kg;
	return 0;
	}

int CFurnace::Step1( )
	{
  for ( int n = 0; n < kFors; n++ )
    {
//    pFire[n]->Logic.pFors->VentGas = 100.;
    pFire[n]->Logic.pFors->VentGas2 = 100.;
    pFire[n]->Logic.pFors->VentSteam = 100.;
    pFire[n]->Logic.pFors->VentSteam2 = 100.;
//    pFire[n]->Logic.pFors->VentPilot = 100.;
//    pFire[n]->Logic.pFors->VentPilot2 = 100.;
    }
  if ( Calc_T4 )
    {
    Calc_T_Wall = false;
    double S = S_Fors / kFors;
    for ( int n = 0; n < kFors; n++ )
      {
      pFire[n]->S_Fors = S;
      pFire[n]->CreateRad( );
      }
    S = S_Trub / kProduct;
    for ( int n = 0; n < kProduct; n++ )
      {
      pTube[n]->S_Trub = S;
      }
    }
  CalcCoef( );
  for ( int n = 0; n < kProduct; n++ )
    {
    pTube[n]->TZ = T_Out_Z;
    pTube[n]->Calc_H = Calc_H;
    pTube[n]->Cp_Tube = Cp_Trub;
    }
  //for ( int n = 0; n < kProduct; n++ )
  //  {
  //  pTube[n]->Q_ext_Rad = 0.;
  //  pTube[n]->Q_Rad = 0.;
  //  }
  //KPD_fire = 100. - KPD_fire;
  if ( Comp_GasZ[0] )
    pCompGasZ->Read( Comp_GasZ );
  return 0;
	}

int CFurnace::StepT ( double dt )
	{
  Calc ( dt );
  kStep++;
	return 0;
	}

int CFurnace::SetData( int TypeData, void * pData )
	{
  //if ( ShowNode.Select( TypeData, pData ))
  //  return 0;
  if ( TypeData == sd_SetInitState )
    {
    T_Pereval = T_air;
    for ( int n = 0; n < kFors; n++ )
      {
      pFire[n]->Logic.pFors->VentGas = 0.;
      pFire[n]->Logic.pFors->ShibGas = 0.;
      pFire[n]->Logic.pFors->VentMazut = 0.;
      pFire[n]->Logic.pFors->VentMazut2 = 0.;
      pFire[n]->Logic.pFors->ShibMazut = 0.;
      pFire[n]->Logic.pFors->VentSteam = 0.;
      pFire[n]->Logic.pFors->VentSteam = 0.;
      pFire[n]->Logic.pFors->VentPilot = 0.;
      pFire[n]->Logic.pFors->VentPilot2 = 0.;
      }
    for ( int n = 0; n < kProduct; n++ )
      {
      pTube[n]->T_in = T_air;
      pTube[n]->T_out = T_air;
      pTube[n]->h_Out_Gas = 0.;
      pTube[n]->Eps_Gas = 0.;
      pTube[n]->Q_ext_Rad = 0.;
      pTube[n]->Q_Rad = 0.;
      pTube[n]->Q_wall = 0.;
      }
    return 0;
    }
  if ( TypeData == sd_GetOverGasSource )
  {
	  *(double**)pData = &FlowQ; 
	  return sd_GetOverGasSource;
  }
  return 1;
	}

int CFurnace::SaveState( )
  {
	S_CLASS("W",_W)
  S_CLASS("Разряжение",CDepression)
  char Name[256];
  int N = 1;
  for ( int n = 0; n < kFors; n++ )
    {
    sprintf_s ( Name, 255, "Форсунка %d", N++ );
    S_STRUCT( Name,CForsunkaW, pFire[n]->Logic.pFors );
    }
  return 0;
  }

int CFurnace::RestoreState( char * StrName )
  {
  COPY_FIRST
	R_CLASS("W",_W)
  R_CLASS("Разряжение",CDepression)
  char Name[256];
  int N = 1;
  for ( int n = 0; n < kFors; n++ )
    {
    sprintf_s ( Name, 255, "Форсунка %d", N++ );
    R_STRUCT( Name,CForsunkaW, pFire[n]->Logic.pFors );
    }
  return 1;
  }

#include "IO_Props.h"
//BEGIN_PROPS(CFurnace)
//PROP('I', "kFors", "Число форсунок", "15", kFors )
////PROP('I', "kProduct", "Число нагреваемых потоков", "4", kProduct )
//PROP('I', "kGasVent", "Число вентилей на основном газу", "1", kGasVent )
//PROP('I', "kGas2Vent", "Число вентилей на дополнительном газу", "1", kGas2Vent )
//PROP('I', "kPilotVent", "Число вентилей на пилотном газу", "1", kPilotVent )
//END_PROPS

  extern "C"
  {
  EXP int GetProps( CProps Properties[], IBaseModel * pModel )
  {
  CFurnace * pObj = (CFurnace*)pModel;
  int N = 0;

#undef PROP
#define PROP(_Type,_Name,_Label,_Default,Param) \
  ASS(N<64)\
  Properties[N].Type = _Type;\
  Properties[N].Name = _Name;\
  Properties[N].Label = _Label;\
  Properties[N].Default = _Default;\
  Properties[N].Addr = NULL;\
  if ( pObj ) Properties[N].Addr = &(pObj->Param);\
  N++;

PROP('I', "kFors", "Число форсунок", "15", kFors )
//PROP('I', "kProduct", "Число нагреваемых потоков", "4", kProduct )
PROP('I', "kGasVent", "Число вентилей на основном газу", "1", kGasVent )
PROP('I', "kGas2Vent", "Число вентилей на дополнительном газу", "1", kGas2Vent )
PROP('I', "kPilotVent", "Число вентилей на пилотном газу", "1", kPilotVent )
END_PROPS

CExtern_Pnt * CFurnace::GetPoint( const char * PntName )
  {
  CExtern_Pnt * pPnt = IBaseModel::GetPoint( PntName );
  if ( pPnt == NULL )
    {
    if ( strncmp ( PntName, "Т_зм", 4 ) == 0 )
      {
// Змеевики
      CSpecPnt * pZ = new CSpecPnt;
      pZ->pNext = pFirstPnt;
      pFirstPnt = pZ;
      pZ->pA = new CAnalogD( this, (char*)PntName, "AOD" );
      pZ->rnd = 1. + 0.02 * ( 1. - 2. * r01());
      pZ->Type = 1;
      char * P = strchr ( (char*)PntName + 4, '_' );
      if ( P )
        {
        pZ->n1 = atoi ( P + 1 );
        P = strchr ( P + 1, '_' );
        if ( P )
          pZ->n2 = atoi ( P + 1 );
        }
      return pZ->pA;
      }
    }
  return pPnt;
  }
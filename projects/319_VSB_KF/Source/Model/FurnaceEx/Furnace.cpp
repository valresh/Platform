#include "stdafx.h"
#include "Furnace.h"
#include "Err.h"
//#include "CommProc.h"
//#include "Data.h"
#include "SetData.h"


CFurnace::CFurnace( char * _ObjName, LPCTSTR Params ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
	{
  UNIMODEL
	lstrcpy ( ObjName, _ObjName );
	#include "IO_Clear.h"
	#include "Points.h"
  pTube = NULL;
  pNodeTube = NULL;
  kProduct = 0;
  pTopl_Gas = NULL;
  pPilot = NULL;
//  pAdd_Gas = NULL;
  pMazut = NULL;
  pMazutSteam = NULL;
  pSteam = NULL;
  pAir = NULL;
  pSmoke = NULL;
  pFlow_Smoke_in = NULL; 
  kFors = 0;
  kForsZ = -1;
  IsProgar = false;
  nProgar = 0;
  kGasVent = 0;
  kPilotVent = 0;
  kStep = 0;
  pFirstPnt = NULL;
  Открыть_пилот = NULL;
  Розжиг_пилот = NULL;
  W_Tube = NULL;
//  CLEAR(Gas) 
//  CLEAR(Pilot) 
  kIn_Product = 0;
  kOut_Product = 0;
  k_Gas = 0;
  k_Smoke = 0;
  k_Smoke_in = 0;
  k_Pilot = 0;
  k_Air = 0;
  k_Add = 0;
  k_Mazut = 0;
  k_Steam = 0;
  //
  MatrixModel = false;
  N_Matrix = 0;
  p_Gas = 0;
  p_Smoke = 0;
  p_Pilot = 0;
  p_Air = 0;
  p_Add = 0;
  p_Mazut = 0;
  p_Steam = 0;
//
  Mu[0] = 28.;
  Mu[1] = 28.;
  Mu[2] = 28.;
  Mu[3] = 28.;
  Mu[4] = 28.;
  Mu[5] = 28.;
  Mu[6] = 28.;
  CLEAR(NumbFlow)
  CLEAR(Omega)
  kFlowIn = 1.;
//  pBurnAll = NULL;
  FlowQ = 0.;
  NewCalc = false;
  //pCreatePyrolysis = NULL;
  if ( Params && Params[0] )
    {
    if ( strstr ( Params, "CHEM" ))
      HaveExtensions = true;
    }

  //  {
  //  char * P = (char*)strstr ( Params, "kF=" );
  //  if ( P )
  //    kForsZ = atoi ( P + 3 );
  //  P = (char*)strstr ( Params, "Пиролиз:" );
  //  if ( P )
  //    {
  //    P += 8;
  //    pCreatePyrolysis = (ICreatePyrolysis*)GetExtension ( P );
  //    }
  //  }

	}

CFurnace::~CFurnace()
	{
	}

int CFurnace::Init( int Regim )
	{
//  if ( Model == "Печь пиролиза" )
  int i = G_H2S;
  G_H2S = GetGasID( "HYDROGEN SULFIDE" );
  G_SO2 = GetGasID( "SULFUR DIOXIDE" );
  G_SO3 = GetGasID( "SULFUR TRIOXIDE" );
  i = G_H2S;
  if ( kFors == 0 )
    return 0;
  if ( HaveExtensions )
    {
    pCreatePyrolysis = (ICreatePyrolysis*)ReadExtension( this );
    }
	int Res = 0;
  char Name[256];
  double W_1 = 1. / (double)kProduct;
  for ( int n = 0; n < kProduct; n++ )
    {
    sprintf_s ( Name, 255, "Труба %d", n + 1 );
    pTube[n] = new CRadTube ( Name, this );
    pTube[n]->pPech = this;
    W_Tube[n] = W_1;
    if ( pCreatePyrolysis )
      {
      sprintf_s ( Name, 255, "Пиролиз в трубе %d", n + 1 );
      pTube[n]->pBasePyrolysis = pCreatePyrolysis->CreateObject( Name, this );
      pTube[n]->pPyrolysis = pCreatePyrolysis->GetInterface ( pTube[n]->pBasePyrolysis );
      }
    }
  pFire = NewArr(CRadFors*,kFors)
  Горит = NewArr(CDiscrete *,kFors); 
  Горит_пилот = NewArr(CDiscrete*,kFors); 
  Отказ_пилот = NewArr(CDiscrete*,kFors);
  Отказ_пламя = NewArr(CDiscrete*,kFors);
  Интенсивность_пламени = NewArr(CAnalog*,kFors);
  Интенсивность_пилот = NewArr(CAnalog*,kFors);
  Открыть_пилот = NewArr(CDiscrete *,kFors); 
  Розжиг_пилот = NewArr(CDiscrete *,kFors); 
  int N = 1;
  for ( int n = 0; n < kFors; n++ )
    {
    sprintf_s ( Name, 255, "%s Форсунка %d", ObjName, N++ );
    pFire[n] = new CRadFors ( Name, this );
    ::GiveCommData( Name, CForsunka::TypeID, (void**)&pFire[n]->Logic.pFors );
    ASS (pFire[n]->Logic.pFors );
    pFire[n]->pPech = this;
    pFire[n]->Logic.Init ( Name );
    //pFire[n]->Logic.pFors->VentGas = 100.;
    //pFire[n]->Logic.pFors->ShibGas = 80.;
    //pFire[n]->Logic.pFors->VentMazut = 0.;
    //pFire[n]->Logic.pFors->VentSteam = 0.;
    //pFire[n]->Logic.pFors->VentPilot = 100.;
//
    sprintf_s ( Name, 255, "Датчик_пламени_%d", n + 1 );
    Горит[n] = new CDiscrete( this, Name, "DO" );
    sprintf_s ( Name, 255, "Горит_пилот_%d", n + 1 );
    Горит_пилот[n] = new CDiscrete( this, Name, "DO" );
    sprintf_s ( Name, 255, "Открыть_пилот_%d", n + 1 );
    Открыть_пилот[n] = new CDiscrete( this, Name, "DI" ); 
    sprintf_s ( Name, 255, "Розжиг_пилот_%d", n + 1 );
    Розжиг_пилот[n] = new CDiscrete( this, Name, "DI" ); 
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
    }
/////////////////
  NewComp(&ID_CompSmoke,&pCompSmoke," Дым из %s", ObjName );
  pCompSmoke->Set( pN2 );
	return 0;
	}

int CFurnace::Step0( )
	{
  if ( kFors == 0 )
    return 0;
  //if ( Fire_From_Ext )
  //  {
  //  char Name[256];
  //  pBurnAll = NULL;
  //  sprintf_s ( Name, 255, "%s_BURN_ALL", (char*)ObjName );
  //  GiveCommData ( Name, CHs::TypeID, (void**)&pBurnAll );
  //  ASS( pBurnAll );
  //  }
	return 0;
	}

int CFurnace::Step1( )
	{
  if ( kFors == 0 )
    return 0;
  //for ( int n = 0; n < kFors; n++ )
  //  {
  //  pFire[n]->Logic.pFors->VentGas = 100.;
  //  pFire[n]->Logic.pFors->VentGas2 = 100.;
  //  pFire[n]->Logic.pFors->VentPilot = 100.;
  //  pFire[n]->Logic.pFors->VentPilot2 = 100.;
  //  }
  //Omega[N_SMOKE] = 10.;//Omega_Smoke;
  //Omega[N_AIR] = 100.;//Omega_Air;
  //Omega[N_GAS] = 0.001;//Omega_Topl_Gas;
  //Omega[N_PILOT] = 0.001;//Omega_Pilot;
  //Omega[N_MAZUT] = Omega_Mazut;
  //Omega[N_STEAM] = Omega_Mazut_Steam;
  //k_Air_Gas = 0.;
  Omega[N_SMOKE] = Omega_Smoke;
  Omega[N_AIR] = Omega_Air;
  Omega[N_GAS] = Omega_Topl_Gas;
  Omega[N_PILOT] = Omega_Pilot;
  Omega[N_MAZUT] = Omega_Mazut;
  Omega[N_STEAM] = Omega_Mazut_Steam;
  Omega[N_SMOKE_IN] = Omega_Smoke_in;
	return 0;
	}

int CFurnace::StepT ( double dt )
	{
  if ( kFors == 0 )
    return 0;
  Calc ( dt );
  kStep++;
	return 0;
	}

int CFurnace::SetData( int TypeData, void * pData )
	{
  if ( TypeData == sd_SetInitState )
    {
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
    return 0;
    }
  if ( TypeData == sd_GetOverGasSource )
    {
	  *(double**)pData = &FlowQ; 
	  return sd_GetOverGasSource;
    }
  if ( TypeData == sd_ParamClicked )
    {
    for ( int n = 0; n < kProduct; n++ )
      {
      if ( pTube[n] && pTube[n]->pBasePyrolysis )
        {
        int Res = pTube[n]->pBasePyrolysis->SetData ( sd_ParamClicked, pData );
        if ( Res == 0 )
          return 0;
        }
      }
    return 1;
    }
  return 1;
	}

int CFurnace::SaveState( )
  {
	S_CLASS("W",_W)
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
//  IGNORE
  COPY_FIRST
	R_CLASS("W",_W)
  char Name[256];
  int N = 1;
  for ( int n = 0; n < kFors; n++ )
    {
    sprintf_s ( Name, 255, "Форсунка %d", N++ );
    R_STRUCT( Name,CForsunkaW, pFire[n]->Logic.pFors );
    }
  return 1;
  }

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

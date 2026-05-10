#include "stdafx.h"
#include "Fan.h"
#include "Err.h"
//#include "CommProc.h"
//#include "Data.h"
#include "SetData.h"

LPCSTR g_szHelpFileName = "Базовый дымосос.pdf";

CFan::CFan( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
, StateIn( "Вход", this )
, Охлаждение( "Охлаждение", this )
, Hmax ( &Размерность ) 
, H_real ( &Размерность ) 
, H_fact ( &Размерность ) 
, H_zad ( &Размерность ) 
{
	UNIMODEL
	lstrcpy ( ObjName, _ObjName );
#include "IO_Clear.h"
#include "Points.h"
#include "Fan_Defects.h"
	Defect = 0;
	Defect_Pow = 1.;
	_def_Вибраций = 1.;
	kStep = 0;
	kOilSens = 8;
	kAirSens = 4;
	kSens_Vibr = 4;
	pMotor = NULL;
	MotorData.pOn = &On;
	MotorData.pRelOborot = &Oborot;
	pAnalogPipe = NULL;
	CLEAR(Обороты);
	CLEAR(Напор);
	CLEAR(Напор);
	CLEAR(Характеристика);
		///
	Hmax.Set( 500. * 1e-4 );
	kОН = 8;
	kПХ = 6;
	for ( int n = 0; n < kОН; n++ )
	{
		Обороты[n] = 0.2 * n;
		Напор[n] = Обороты[n] * Обороты[n];
	}
	for ( int n = 0; n < kПХ; n++ )
	{
		Поток[n] = 0.2 * n;
		Характеристика[n] = 1. - Поток[n] * Поток[n];
	}
	H_zad.Set( 0. );
	P_kdg = 0.;
	IsElectro = true;
}

CFan::~CFan()
	{
	}

int CFan::Init( int Regim )
	{
	int Res = 0;
	//#include "IO_Init.h"
	//#include "Points.h"
	//#include "F_Init.h"  
	//#include "ACS.h"
//  ASS(GiveCommData( ObjName,  CPumpData::TypeID,	(void**)&pPump)==0);
//  Смазка.DefectNode = NULL;
//  Смазка.DefectName = "Перегрев";
  Охлаждение.DefectNode = NULL;
  Охлаждение.DefectName = "Перегрев";
//  Смазка.Init( "Т подшипника %d", kOilSens );
  Охлаждение.Init( "Т %d", kAirSens );
  if ( kSens_Vibr > 0 )
    {
    Char<128>Name;
    pVibro = NewArr(CAnalog*,kSens_Vibr);
    rndVibrSens = NewArr(double,kSens_Vibr);
    for ( int n = 0; n < kSens_Vibr; n++ )
      {
      Name.Prt ( "Вибрация %d", n + 1 );
      pVibro[n] = new CAnalog ( this, Name, "AO");
      rndVibrSens[n] = 0.5 * Max_Vibration * ( 1. + r01());
      }
    }
  Mw_gas = 28.;
  pos_Х = 0;
	return 0;
	}


int CFan::Step0( )
	{
  To_m3_in = RP * 293.15;
	return 0;
	}

int CFan::Step1( )
	{
  CalcParams( );
  DWORD LastDefect = 0;
  if ( Тип == 0 )
    {
// Дымосос
	CDef* pDef = GetDefect(ClassRootNode);
	lstrcpy(pDef->Name, "Дымососы");
	lstrcpy(Падение_мощности_вентилятора->Name, "Падение мощности дымососа");
    }
  if ( Тип == 1 )
    {
// Вентилятор
	CDef* pDef = GetDefect(ClassRootNode);
	lstrcpy(pDef->Name, "Вентиляторы");
	lstrcpy(Падение_мощности_вентилятора->Name, "Падение мощности вентилятора");
    }
  if (pMotor == NULL)
    {
    DEFECT(Останов_вентилятора,"Останов двигателя")
    DEFECT(Выход_двигателя_из_строя,"Выход двигателя из строя")
    }
  Hmax.Update ( NULL );
  H_real.Update ( NULL );
  H_fact.Update ( NULL );
  H_zad.Update ( NULL );
  Calc_K_P2_P1( );
  if ( Use_X )
    СпадРаб( );
  else
    SetРаб ( );
  Смазка = new CHeatOil( "Смазка", this, &Oborot );
  return 0;
	}

int CFan::StepT ( double dt )
	{
  kStep++;
  Calc ( dt );
	return 0;
	}

int CFan::SetData( int TypeData, void * pData )
	{
  switch ( TypeData )
    {
    case sd_GetPow:
      {
      *(double**)pData = &Pow; 
      return sd_GetPow;
      }
	case sd_GetHelpPath:
		{
			lstrcpy((char*)pData, HELP_PATH);
			lstrcat((char*)pData, g_szHelpFileName);
			return 1;
		}
    }
	return 1;
	}

int CFan::SaveState( )
  {
	S_CLASS("W",_W)
  return 0;
  }

int CFan::RestoreState( char * StrName )
  {
  COPY_FIRST
  if ( lstrcmp ( StrName, "W" ) == 0 )
  {
  ASS(pSRFile->RStruct( sizeof ( _W ), static_cast<_W*>(this)));
  if(!_finite(H))
	  H = 0;
  return 0;
  }
  return 1;
  }


#include "IO_Props.h"
int CFan::GetProp( )
  {
  PARM( "Датчики_Т_подш", 'I', &kOilSens );
  PARM( "Датчики_Т_апп",  'I', &kAirSens );
  PARM( "Датчики вибрации",  'I', &kSens_Vibr );
  return 0;
  }
/*
extern "C"
  {
  EXP CProps Properties[] = {
    'I', "Датчики_Т_подш", "Датчики Т подш", "4",
    'I', "Датчики_Т_апп", "Датчики Т апп", "4",
    'I', "Датчики_доп", "Датчики доп", "0",
    0, NULL, NULL,
    };
  }
*/
BEGIN_PROPS(CFan)
PROP('I', "Датчики_Т_подш", "Датчики Т подш", "6", kOilSens )
PROP('I', "Датчики_Т_апп", "Датчики Т апп", "4", kAirSens )
PROP('I', "Датчики вибрации", "Датчики вибрации", "2", kSens_Vibr )
END_PROPS



int CFan::DstMM( char * DataName, struct IMM * pMM )
{
	if (!strcmp( DataName, "Motor"))
	{
		ASS( pMM->ID == CMotorData::TypeID );
		pMotor = (CMotorData*)pMM->pData;
		*pMotor->ppPower = &P_kdg;
		strcpy_s ( Мотор, 31, (char*)pMM->ObjName );
		return 0;
	}
	return 1;
}

int CFan::SrcMM( char * DataName, struct IMM * pMM )
{
	if ( lstrcmp( DataName, "Motor" ) == 0 )
	{
		pMM->ID = CMotorData::TypeID;
		pMM->pData = &MotorData;
		pMM->ObjName = ObjName;
		return 0;
	}
	return 1;
}
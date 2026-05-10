#include "stdafx.h"
#include "MultiSens.h"
#include "Err.h"
//#include "CommProc.h"
//#include "Data.h"
#include "HydroStruct.h"
#include "MM.h"

CMultiSens::CMultiSens( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
	{
  UNIMODEL
	ObjName = _ObjName;
	#include "IO_Clear.h"
	#include "Points.h"
    #include "MultiSens_Defects.h"
  kVibroSens = 8;
  kShiftSens = 8;
  kTSens = 8;
  pVibro = NULL;
  pShift = NULL;
  pT = NULL;
  rndShftSens_Curr = NULL;
  rndShftSens_Lim = NULL;
  rndShftSens_Vel = NULL;
  rndVibrSens = NULL;
  rndTSens = NULL;
  nVibroSens = 0;
  nShiftSens = 0;
  nTSens = 0;
  kDefectSens = -1;
  eDefect = DEFECT_NO;
	}

CMultiSens::~CMultiSens()
	{
	}

int CMultiSens::Init( int Regim )
	{
	int Res = 0;
	//#include "IO_Init.h"
	//#include "Points.h"
	//#include "ACS_Init.h"  
	//#include "ACS.h"
  Char<128>Name;
  if ( kVibroSens < 4 )
    kVibroSens = 4;
  if ( kVibroSens > 0 )
    {
    pVibro = NewArr(CAnalogD*,kVibroSens);
    rndVibrSens = NewArr(double,kVibroSens);
    for ( int n = 0; n < kVibroSens; n++ )
      {
      Name.Prt( "Вибрация_%d", n + 1 );
      pVibro[n] = new CAnalogD( this, Name, "AO" );
      }
    }
  if ( kShiftSens > 0 )
    {
    pShift = NewArr(CAnalogD*,kShiftSens);
    rndShftSens_Curr = NewArr(double,kShiftSens);
    rndShftSens_Lim = NewArr(double,kShiftSens);
    rndShftSens_Vel = NewArr(double,kShiftSens);
    for ( int n = 0; n < kShiftSens; n++ )
      {
      Name.Prt( "Сдвиг_%d", n + 1 );
      pShift[n] = new CAnalogD( this, Name, "AO" );
      }
    }
  if ( kTSens > 0 )
    {
    pT = NewArr(CAnalogD*,kTSens);
    rndTSens = NewArr(double,kTSens);
    for ( int n = 0; n < kTSens; n++ )
      {
      Name.Prt( "Температура_%d", n + 1 );
      pT[n] = new CAnalogD( this, Name, "AO" );
      }
    }
	return 0;
	}

int CMultiSens::Step0( )
	{
	return 0;
	}

int CMultiSens::Step1( )
	{
  //
  for ( int n = 0; n < kVibroSens; n++ )
    {
    rndVibrSens[n] = 0.6 + 0.4 * r01();
    }
  for ( int n = 0; n < kShiftSens; n++ )
    {
    rndShftSens_Curr[n] = rndShftSens_Lim[n] = ( 1. - 2. * r01()) * Max_Shift;
    rndShftSens_Vel[n] = 1 / Tau;
    }
  for ( int n = 0; n < kTSens; n++ )
    {
    rndTSens[n] = T_min + (T_max - T_min ) * r01();
    }
	return 0;
	}

int CMultiSens::StepT ( double dt )
	{
  Calc ( dt );
	return 0;
	}

int CMultiSens::SetData( int TypeData, void * pData )
	{
	return 1;
	}

int CMultiSens::SaveState( )
  {
	S_CLASS("W",_W)
  return 0;
  }

int CMultiSens::RestoreState( char * StrName )
  {
	R_CLASS("W",_W)
  return 1;
  }

int CMultiSens::SetProp ( struct CObjProps * pProp )
  {
  if ( pProp->PropName == "Датчики_вибрации" )
    {
    if ( pProp->Value == NULL )
      return 2;
    kVibroSens = atoi ( pProp->Value ); 
    if ( kVibroSens < 1 || kVibroSens > 32 )
      return 3;
    return 0;
    }
  if ( pProp->PropName == "Датчики_сдвига" )
    {
    if ( pProp->Value == NULL )
      return 2;
    kShiftSens = atoi ( pProp->Value ); 
    if ( kShiftSens < 1 || kShiftSens > 32 )
      return 3;
    return 0;
    }
  if ( pProp->PropName == "Датчики_температуры" )
    {
    if ( pProp->Value == NULL )
      return 2;
    kTSens = atoi ( pProp->Value ); 
    if ( kTSens < 1 || kTSens > 32 )
      return 3;
    return 0;
    }
  return 0;
  }

#include "IO_Props.h"
int CMultiSens::GetProp( )
  {
  PARM( "Датчики_вибрации", 'I', &kVibroSens );
  PARM( "Датчики_сдвига", 'I', &kShiftSens );
  return 0;
  }


int CMultiSens::DstMM( char * DataName, struct IMM * pMM )
{
	if (!strcmp( DataName, "Motor"))
	{
		ASS( pMM->ID == CMotorData::TypeID );
		pMotor = (CMotorData*)pMM->pData;
		return 0;
	}
	return 1;
}

int CMultiSens::SrcMM( char * DataName, struct IMM * pMM )
{
	//int Res = IBaseModel::SrcMM( DataName, pMM );
	//if ( Res == 0 )
	//	return 0;
	//if ( Lstrcmp( DataName, "Рост температуры подшипников" ) == 0 )
	//{
	//	pMM->ID = 0x2B8C4C64;
	//	pMM->pData = Рост_температуры_подшипников;
	//	pMM->ObjName = ObjName;
	//	return 0;
	//}
	//if ( Lstrcmp( DataName, "Рост вибрации" ) == 0 )
	//{
	//	pMM->ID = 0x2B8C4C64;
	//	pMM->pData = Рост_вибрации;
	//	pMM->ObjName = ObjName;
	//	return 0;
	//}
	//if ( Lstrcmp( DataName, "Рост сдвига" ) == 0 )
	//{
	//	pMM->ID = 0x2B8C4C64;
	//	pMM->pData = Рост_сдвига;
	//	pMM->ObjName = ObjName;
	//	return 0;
	//}
	return 2;
}

/*
extern "C"
  {
  EXP CProps Properties[] = {
    'I', "Датчики_вибрации", "Датчики вибрации", "0",
    'I', "Датчики_сдвига", "Датчики сдвига", "0",
    0, NULL, 
    };
  }
*/

BEGIN_PROPS(CMultiSens)
PROP('I', "Датчики_вибрации", "Датчики вибрации", "0", kVibroSens )
PROP('I', "Датчики_сдвига", "Датчики сдвига", "0", kShiftSens )
PROP('I', "Датчики_температуры", "Датчики температуры", "0", kTSens )
END_PROPS


/*
CExtern_Pnt * CMultiSens::FindPoint( const char * PntName )
  {
  if ( PntName[0] == 'В' && nVibroSens < kVibroSens )
    {
    return pVibro[nVibroSens++];
    }
  if ( PntName[0] == 'С' && nShiftSens < kShiftSens )
    {
    return pShift[nShiftSens++];
    }
  if ( PntName[0] == 'Т' && nTSens < kTSens )
    {
    return pT[nTSens++];
    }
  return NULL;
  }
*/

#include "HydroStruct.h"
#include "TestNodes.h"

NODE_LIST
MOTOR_IN ( 1, "i_motor","Обороты аппарата")
END_LIST

STD_TEST

int CMultiSens::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  int Res = ::StdTestNodes( ObjName, kNodes, ppNodes );
  if ( Res )
    return Res;
  return 0;
  }

int CMultiSens::SetMatr( void * pExternals, int PntGroup,  double dt, 
                    double A[/* kExkV */], double B[ /*kE*/ ] )   // A * X = B
  {
  ASS(0)
  return 0;
  }

int CMultiSens::SetVar( void * pExternals, int PntGroup, double dt, 
					   // Переменные на входах
					   double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
{
	return 0;
}

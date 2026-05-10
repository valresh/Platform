#include "stdafx.h"
#include "CoolObj.h"
#include "Err.h"
//#include "CommProc.h"
#include "MM.h"
#include "HydroStruct.h"
#include "SetDataTypes.h"

LPCSTR g_szHelpFileName = "Базовое охлаждение.pdf";

extern "C"
  {
  int GetProps( struct CProps * pProps, struct IBaseModel * pObj );
  };

CCoolObj::CCoolObj( char * _ObjName, IBaseModel* pMain /*= NULL*/) :	IBaseModel (_ObjName, pMain) 
#include "ACS_Constr.h"
#include "ACS.h"
, In("Вход",this)
, Out("Выход",this)
	{
  UNIMODEL
  ObjName = _ObjName;
  Model = "CoolObj";
    #include "IO_Clear.h"
	#include "Points.h"
    #include "CoolObj_Defects.h"
  kSens = 16;
  pSens = NULL;
//  NumbSens = 0;
  Defect = 0;
  nSens = -1;
  Pow = 0.;
  W = 0.;
  Set_Norm = false;
  On = false;
  Oborot = 0.;
  pGetProps = GetProps;
  pMotor = NULL;
	}

CCoolObj::~CCoolObj()
	{
	}

int CCoolObj::Init( int Regim )
	{
	int Res = 0;
  Char<32>Name;
  pSens = NewArr(CAnalog*,kSens);
  Tz = NewArr(double,kSens);
  kTz = NewArr(double,kSens);
  for ( int n = 0; n < kSens; n++ )
    {
    Tz[n] = T_min + r01() * ( T_max - T_min );
    kTz[n] = 0.;
    Name.Prt ( "Температура_%d", n + 1 );
	pSens[n] = new CAnalog ( pMain ? pMain : this, Name, "AO" );
    }
	return 0;
	}

int CCoolObj::Step0( )
	{
	return 0;
	}

int CCoolObj::Step1( )
	{
	return 0;
	}

int CCoolObj::StepT ( double dt )
	{
  Calc ( dt );
	return 0;
	}

int CCoolObj::SetData( int TypeData, void * pData )
{
	switch ( TypeData )
	{		
	case sd_GetHelpPath:
		{
			lstrcpy((char*)pData, HELP_PATH);
			lstrcat((char*)pData, g_szHelpFileName);
			return 1;
		}
	}
	return 1;
}

int CCoolObj::SaveState( )
  {
	S_CLASS("W",_W_CoolObj)
  return 0;
  }

int CCoolObj::RestoreState( char * StrName )
  {
  IGNORE
	R_CLASS("W",_W_CoolObj)
  return 1;
  }

int CCoolObj::SetProp ( struct CObjProps * pProp )
  {
  if ( pProp->PropName != "Число_датчиков" )
    return 1;
  if ( pProp->Value == NULL )
    return 2;
  kSens = atoi ( pProp->Value ); 
  if ( kSens < 1 || kSens > 32 )
    return 3;
  return 0;
  }

#include "IO_Props.h"
int CCoolObj::GetProp( )
  {
  PARM( "Число_датчиков", 'I', &kSens );
  return 0;
  }


int CCoolObj::DstMM( char * DataName, struct IMM * pMM )
{
	if (!lstrcmp( DataName, "Motor"))
	{
		ASS( pMM->ID == CMotorData::TypeID );
		pMotor = (CMotorData*)pMM->pData;
		return 0;
	}
	return 1;
}

/*
CExtern_Pnt * CCoolObj::FindPoint( const char * PntName )
  {
  if ( PntName[0] != 'Т' )
    return NULL;
  if ( NumbSens >= kSens )
    return NULL;
  return pSens[NumbSens++];
  }
*/

#include "IO_Props.h"
BEGIN_PROPS(CCoolObj)
PROP('I', "Число_датчиков", "Число датчиков температуры", "15", kSens )
END_PROPS

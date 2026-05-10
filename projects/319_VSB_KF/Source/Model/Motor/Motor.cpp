#include "stdafx.h"
#include "Motor.h"
#include "Err.h"
//#include "CommProc.h"
//#include "Data.h"
#include "SetData.h"
#include "MM.h"

CMotor::CMotor( char * _ObjName, IBaseModel* pMain /*= NULL*/) :	IBaseModel (_ObjName, pMain) 
#include "ACS_Constr.h"
#include "ACS.h"
{
	UNIMODEL
    ObjName = _ObjName;
    Model = "Motor";
	#include "IO_Clear.h"
	#include "Points.h"
	#include "Motor_Defects.h"
	Defect_Pow = 1.;
	kAvar = 0;
	kStep = 0;
	//  pCalcTок_1 = NULL;
	//  pCalcTок_2 = NULL;
	memset(DrivePow, 0, sizeof(DrivePow[0]) * MAX_DRIVE);
	MotorData.pOn = &On;
	MotorData.pRelOborot = &Oborot;
	IsElectro = true;
}

CMotor::~CMotor()
	{
	}

int CMotor::Init( int )
	{
	int Res = 0;
	//#include "IO_Init.h"
	//#include "Points.h"
	//#include "ACS_Init.h"  
	//#include "ACS.h"
#if 0
  if ( BaseModelТок )
    {
    pCalcTок_1 = new I_BaseТок;
    pCalcTок_1->Init();
    }
  else
    pCalcTок_1 = NULL;
  if ( NewModelТок )
    {
    pCalcTок_2 = new I_StartТок;
    pCalcTок_2->Init();
    }
  else
    pCalcTок_2 = NULL;
#endif
	return 0;
	}

int CMotor::Step0( )
	{
	return 0;
	}

int CMotor::Step1( )
	{
  bool RSU = Задание.Use();
  bool FLD = Задание_по_месту.Use();
  if ( RSU && !FLD )
    Управление_по_месту = false;
  if ( !RSU && FLD )
    Управление_по_месту = true;
  if ( On )
    {
    //Работает = 1;
    //Готовность = 1;
    //Пуск_возможен = 1;
    //Фаза_пуска_5 = 1;
    //ЧП = 1;
    //Пуск_зеленый = 1;
    Ток = 100.;
    }
  else
    Работает = 0;
return 0;
	}

int CMotor::StepT ( double dt )
	{
  Calc ( dt );
	return 0;
	}

int CMotor::SetData( int TypeData, void * pData )
	{
  switch ( TypeData )
    {
    case sd_SetInitState:
      {
      memset( static_cast<_W_Motor_b*>(this), 0, sizeof ( _W_Motor_b ));
      }
    break;
    }
	return 0;
	}

int CMotor::SaveState( )
  {
	S_CLASS("W",_W_Motor_b)
  return 0;
  }

int CMotor::RestoreState( char * StrName )
  {
  COPY_FIRST
	R_CLASS("W",_W_Motor_b)
  return 1;
  }

int CMotor::ResetVariable()
{
	return 0;
}

int CMotor::SrcMM( char * DataName, struct IMM * pMM )
{
    if ( Lstrcmp( DataName, "Motor" ) == 0 )
	{
		pMM->ID = CMotorData::TypeID;
		int i;
		for(i = 0; i <  MAX_DRIVE; i++)
			if(!DrivePow[i])
				break;
		MotorData.ppPower = &DrivePow[i];
		pMM->pData = &MotorData;
		pMM->ObjName = ObjName;
		return 0;
	}		
	return 1;
}

#include "SysDataTypes.h"

char * CExternalCondition::ClassName = "Условия";

int CExternalCondition::Init()
  {
	INIT(CExternalCondition)
//
	T_Air = 15.;	// температура окружающего воздуха в градусах Цельсия
	WorkUPS = true;
	PressAirKip = 5.;
	WorkDCS = true;
//
	IsElectro_380 = true;
	IsElectro_6000 = true;
	IsPumpSbros = false;
	IsBlk = false;
	CtrlMinMax = true;
  IsEmptyVol = false;
	return 0;
	}

void CExternalCondition::BuildList()
  {
  CExternalCondition hs; CExternalCondition sh;
  SValueDef list[] =
    {
      { "T воздуха", SHIFT_OBJ(T_Air), enumValueDbl },
      { "P КИП", SHIFT_OBJ(PressAirKip), enumValueDbl },
      { "380", SHIFT_OBJ(IsElectro_380), enumValueBol },
      { "6000", SHIFT_OBJ(IsElectro_6000), enumValueBol },
      { "Блокировки", SHIFT_OBJ(IsBlk), enumValueBol },
    };
  //FILL_VALUE
  }

int CExternalCondition::SaveState( )
  {
  return 0;
  }

int CExternalCondition::RestoreState( char * StrName )
  {
	ASS(FALSE);
  return 1;
  }

int CExternalCondition::GetParams(  )
	{
	int N = 0;
	WorkUPS = true;
	WorkDCS = true;
	IsElectro_380 = true;
	IsElectro_6000 = true;
	return 0;
  }


#include "DataTypes.h"
#include "SR.h"
#include "Param.h"

char * CGasAnalyz::ClassName = "Анализатор";

int CGasAnalyz::Init()
  {
	INIT(CGasAnalyz)
  //
  Busy = NULL;
	memset( C, 0, sizeof ( C ));
	return 0;
	}

void CGasAnalyz::BuildList()
  {
  CGasAnalyz hs; CShGasAnalyz sh;
//##  static DWORD dwLog = CEventLogType::etGasAnaliz;
  SValueDef list[] =
    {
	{ "SaveData", SHIFT_OBJ(SaveData), enumValueBol },
	{ "ClearSave", SHIFT_OBJ(ClearSave), enumValueBol },
    };
//  FILL_VALUE
  }

int CGasAnalyz::SaveState( )
  {
  return 0;
  }

int CGasAnalyz::RestoreState(  char * StrName )
  {
	ASS(FALSE);
  return 0;
  }

int CGasAnalyz::GetParams(  )
  {
    PARM(bNewStructIsUsed,"#NewStruct")
  	PARM(SaveData,"#SaveData")
	PARM(ClearSave,"#ClearSave")
	PARM(UseCurve,"#UseCurve")
	return 0;
  }


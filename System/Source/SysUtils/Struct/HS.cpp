#include "DataTypes.h"
#include "SR.h"
#include "Param.h"

char * CHs::ClassName = "HS";

int CHs::Init()
  {
	INIT(CHs)
  mOn = false;
  Busy = 0;
	return 0;
	}

void CHs::BuildList()
  {
  CHs hs; CShHS sh;
  SValueDef list[] =
    {
    { "MV", SHIFT_OBJ(mOn), enumValueBol },
    { "State_Flags", SHIFT_OBJ(State_Flags), enumValueInt },
	{ "Квитировать"          , SHIFT_OBJ(Ctrl_Flags       ), enumValueInt , CHs::ACK  },//
	{ "НеКвитирован"          , SHIFT_OBJ(State_Flags       ), enumValueInt , CHs::IS_NOACK  },//
    };
//  FILL_VALUE
  }

int CHs::SaveState( )
  {
  SAVE_W("F",CFlagsW)
  SAVE_W("W",CHsW)
  return 0;
  }

int CHs::RestoreState( char * StrName )
  {
  REST_W("F",CFlagsW)
  REST_W("W",CHsW)
  return 0;
  }

#define PARM_STRUCT
#include "IO_Parms.h"

int CHs::GetParams(  )
	{
	PARM(mOn,"#mOn")
	PARM(State_Flags,"#State_Flags")
	PARM(Ctrl_Flags,"#Ctrl_Flags")
	return 0;
  }

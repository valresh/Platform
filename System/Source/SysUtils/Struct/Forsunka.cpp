#include "DataTypes.h"
#include "BaseType.h"
#include "SR.h"
#include "Param.h"

char * CForsunka::ClassName = "Форсунка";

int CForsunka::Init()
  {
  INIT(CForsunka)
//
  isFire = false;
  canFire = false;
  setFire = false;
  VentGas = 0.;
  ShibGas = 0.;
  VentMazut = 0.;
  ShibMazut = 0.;
  VentSteam = 0.;
  Busy = 0;
  dKPD = 0.;
  return 0;
  }

void CForsunka::BuildList()
  {
  CForsunka hs; CShForsunka sh;
  SValueDef list[] =
    {
    { "Вентиль газа"    , SHIFT_OBJ(VentGas    ), enumValueDbl },
    { "Вентиль газа2"    , SHIFT_OBJ(VentGas2    ), enumValueDbl },
    { "Шибер газа"      , SHIFT_OBJ(ShibGas    ), enumValueDbl },
	{ "Шибер газа2"      , SHIFT_OBJ(ShibGas2    ), enumValueDbl },
    { "Вентиль мазута"  , SHIFT_OBJ(VentMazut  ), enumValueDbl },
	{ "Вентиль мазута2"  , SHIFT_OBJ(VentMazut2  ), enumValueDbl },
    { "Шибер мазута"    , SHIFT_OBJ(ShibMazut  ), enumValueDbl },
    { "Паровой вентиль" , SHIFT_OBJ(VentSteam  ), enumValueDbl },
	{ "Паровой вентиль2" , SHIFT_OBJ(VentSteam2  ), enumValueDbl },
    { "Пилотный вентиль", SHIFT_OBJ(VentPilot  ), enumValueDbl },
    { "Пилотный вентиль2", SHIFT_OBJ(VentPilot2  ), enumValueDbl },
//  { "Мода"            , SHIFT_OBJ(charMode   ), enumValueChr },
    { "Зажигание"       , SHIFT_OBJ(setFire    ), enumValueBol },
	{ "Зажигание_Пилот" , SHIFT_OBJ(State_Flags), enumValueInt  , CForsunka::IS_ZAPAL_FIRE  },//
	{ "ОсновнойГорит" , SHIFT_OBJ(State_Flags), enumValueInt  , CForsunka::IS_WORK_FIRE  },//
//  { "Зажигание", SHIFT_VALUE(setFire  ), SHIFT_VAL(isFire  ), enumValueBol},
    };
//???  FILL_VALUE
  }

int CForsunka::SaveState( )
  {
  SAVE_W("Ф",CFlagsW)
  SAVE_W("W",CForsunkaW)
  return 0;
  }


int CForsunka::RestoreState( char * StrName )
  {
  Busy = 0;
  REST_W("Ф",CFlagsW)
  REST_W("W",CForsunkaW)
  return 1;
  }
  
#define PARM_STRUCT
#include "IO_Parms.h"

int CForsunka::GetParams(  )
  {
  PARM(isFire, "isFire")  // Действительное горение форсунки
  PARM(canFire, "canFire" ) // Физическая возможность загорания форсунки
  PARM(setFire, "setFire" ) // Желание оператора зажечь форсунку
  PARM(VentGas, "VentGas" )   // Процент открытия вентиля газа (0-100)
  PARM(VentGas2, "VentGas2" )   // Процент открытия вентиля газа (0-100)
  PARM(ShibGas, "ShibGas" )   // Процент открытия шибера газа (0-100)
  PARM(ShibGas2, "ShibGas2" )   // Процент открытия шибера газа (0-100)
  PARM(VentMazut, "VentMazut" ) // Процент открытия вентиля мазута(0-100)
  PARM(VentMazut2, "VentMazut2" ) // Процент открытия вентиля мазута(0-100)
  PARM(ShibMazut, "ShibMazut" ) // Процент открытия шибера мазута(0-100)
  PARM(VentSteam, "VentSteam" ) // Процент открытия парового вентиля (0-100)??
  PARM(VentPilot, "VentPilot" ) // Процент открытия пилотного вентиля (0-100)??
  PARM(VentSteam2, "VentSteam2" ) // Процент открытия парового вентиля (0-100)??
  PARM(VentPilot2, "VentPilot2" ) // Процент открытия пилотного вентиля (0-100)??
  PARM(State_Flags, "StateFlags" )
  return 0;
  }

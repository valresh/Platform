#include "stdafx.h"
#include "CV.h"
#include "Err.h"
#include "Param.h"

static char * TypeSP = "Массовый расход;Объемный расход н.у.;Объемный расход;Давление на входе;Давление на выходе;Ссылка;";

bool CCV::IsOn()
  {
  char * Pos = TypeSP;
  for ( int n = 0; n < Reg_PV; n++ )
    {
    Pos = strchr ( Pos, ';' );
    if ( Pos == NULL )
      break;
    Pos++;
    }
  ISet::Descr = Pos;
  return Reg_On;
  }
void CCV::Off()
  {
  char * Pos = TypeSP;
  for ( int n = 0; n < Reg_PV; n++ )
    {
    Pos = strchr ( Pos, ';' );
    if ( Pos == NULL )
      break;
    Pos++;
    }
  ISet::Descr = Pos;
  Reg_On = false;
  }

void CCV::On()
  {
  char * Pos = TypeSP;
  for ( int n = 0; n < PV; n++ )
    {
    Pos = strchr ( Pos, ';' );
    if ( Pos == NULL )
      break;
    Pos++;
    }
  ISet::Descr = Pos;
  Reg_On = true;
  }

#include "IO_Parms.h"
void CCV::GetParam()
  {
TAB("Регулятор", 1)
  PARM( Reg_On, "Включен" )
  PARM( Reg_PV_, "#PV=" )
  PARM( Reg_MV_, "#MV=" )
  PARM_L( Reg_PV, "PV", "Массовый расход;Объемный расход н.у.;Объемный расход;Давление на входе;Давление на выходе;Ссылка;")
  PARM_S( Reg_Ref, 128, "Ссылка", "" )
  PARM( Reg_SP, "Задание" )
  PARM_D( Reg_P, "P", 1000. )
  PARM_D( Reg_I, "I", 50. )
	PARM_B( Reg_Dir, "Прямой", true )
	PARM_D( Reg_SH, "Масштаб PV", 10. )
	PARM_D( Reg_MH, "Масштаб MV", 100. )
	PARM_D( Reg_Tau, "Инерционность PV, c", 0. )
	PARM_B( Reg_Vent, "Крутить вентиль", true )
  PARM_D( Reg_dMV_, "dMV=", 0. )
ETAB
  }

void CCV::OutValue ( char _Txt[64] )
  {
  char * Txt = _Txt;
  *Txt = '®';
  Txt++;
	if ( !_finite( Reg_SP ) || fabs( Reg_SP ) > 1e10 )
		{
    lstrcpy ( Txt, "NaN" );
		return;
		}
	if ( Reg_SP == 0. )
		lstrcpy ( Txt, "0" );
  else
	if ( fabs ( Reg_SP ) < 1e-5 )
		sprintf_s ( Txt, 64, "%13.5le", Reg_SP );
  else
	if ( fabs ( Reg_SP ) < 1e-4 )
		sprintf_s ( Txt, 64, "%11.9lf", Reg_SP );
  else
	if ( fabs ( Reg_SP ) < 1e-3 )
		sprintf_s ( Txt, 64, "%10.8lf", Reg_SP );
  else
	if ( fabs ( Reg_SP ) < 1e-2 )
		sprintf_s ( Txt, 64, "%9.7lf", Reg_SP );
  else
	if ( fabs ( Reg_SP ) < 1e-1 )
		sprintf_s ( Txt, 64, "%8.6lf", Reg_SP );
  else
	if ( fabs ( Reg_SP ) < 1. )
		sprintf_s ( Txt, 64, "%7.5lf", Reg_SP );
  else
	if ( fabs ( Reg_SP ) < 10. )
		sprintf_s ( Txt, 64, "%6.4lf", Reg_SP );
  else
	if ( fabs ( Reg_SP ) < 100. )
		sprintf_s ( Txt, 64, "%5.3lf", Reg_SP );
  else
	if ( fabs ( Reg_SP ) < 1000. )
		sprintf_s ( Txt, 64, "%5.2lf", Reg_SP );
  else
	if ( fabs ( Reg_SP ) < 10000. )
		sprintf_s ( Txt, 64, "%5.1lf", Reg_SP );
  else
	if ( fabs ( Reg_SP ) > 10000000. )
		sprintf_s ( Txt, 64, "%e", Reg_SP );
  else
		sprintf_s ( Txt, 64, "%5.0lf", Reg_SP );
  }

void CCV::Set ( double Parm )
  {
  if ( Parm != 0. ) 
    { 
    Reg_On = true; 
    Reg_SP = Parm; 
    }  
  }

#include "stdafx.h"
#define DLL_Chem
#include "Chem.h"
#include "math.h"
#include "Data.h"
#include "Param.h"

CSolid::CSolid()
  {
  Ro_A = 1000.; // кг/м3
  Ro_B = 0.;
  Cp_A = 4.18;  // кДж/кг
  Cp_B = 0.;
  }

void CSolid::Init() //0 
  {
  }

double CSolid::Cp ( double TC )	// Cp кДж/кг C
  {
  double dT = TC - 25.;
  return Cp_A + Cp_B * dT;
  }

double CSolid:: H ( double TC )	// Cp * T кДж/кг
  {
  double dT = TC - 25.;
  return ( Cp_A + 0.5 * Cp_B * dT ) * dT;
  }

double CSolid::Ro( double TC ) // Ro кг/м3
  {
  double dT = TC - 25.;
  return Ro_A / ( 1. + Ro_B * dT );
  }
#define this pMain
#include "IO_Parms.h"
void CSolid::GetParams ( IBaseModel * pMain )
  {
  Char<256> Txt;
  Txt.Prt( "/%s", Name );
  TAB( Txt, 1 )
    Txt.Prt( "|%s|Ro A", Name );
  PARM( Ro_A, Txt );
  Txt.Prt( "|%s|Ro B", Name );
  PARM( Ro_B, Txt );
  Txt.Prt( "|%s|Cp A", Name );
  PARM( Cp_A, Txt );
  Txt.Prt( "|%s|Cp B", Name );
  PARM( Cp_B, Txt );
  ETAB
  }
#undef this

int CSolid::ReadData( _HANDLE hFile )
  {
  CSolidDBData Data;
  DWORD Lr;
  ReadFile ( hFile, &Data, sizeof ( Data ), &Lr, NULL );
  Ro_A = Data.Ro_A;
  Cp_A = Data.Cp_A;
  Ro_B = Data.Ro_B;
  Cp_B = Data.Cp_B;
  strcpy_s ( Name, 64, Data.Name );
  return 0;
  }

#include "stdafx.h"
#include "Sens.h"
#include "Err.h"
#include "CommProc.h"
#include "SetData.h"

const int kCol = 4;
const int Len = 64;


struct CBaseCalc
  {
  static CComp * pComp;
  static bool * IsComp;
  static double * Cmol;
  static double Cmass[K_GAS];
  static CTBP * pTBP;
  static int kPnt_TBP;
  static double * V_TBP;
  static double * T_TBP;
  static bool IsTBP;
  static bool UseTBP;
//
  char Name[Len];
  virtual CBaseCalc * Get ( char Cols[kCol][Len] ) = 0;
  int GetID( const char * Name )
    {
    int ID = -1;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( lstrcmp ( pGas[c]->Name, Name ) == 0 ||
           ( pGas[c]->Alt_Name && lstrcmp ( pGas[c]->Alt_Name, Name ) == 0 ) )
        {
        ID = c;
        break;
        }
      }
    if ( ID < 0 )
      KKK();
    return ID;
    }
  virtual double Calc ( ) = 0;
  double T_PROC( double Proc );
  double V_PROC( double T );
  double M_PROC( double T );
  double V_PROC( double Tb, double Te );
  NEW
  };

CComp * CBaseCalc::pComp = NULL;
bool * CBaseCalc::IsComp = NULL;
double * CBaseCalc::Cmol = NULL;
double CBaseCalc::Cmass[K_GAS];
CTBP * CBaseCalc::pTBP = NULL;
int CBaseCalc::kPnt_TBP = 0;
double * CBaseCalc::V_TBP = NULL;
double * CBaseCalc::T_TBP = NULL;
bool CBaseCalc::IsTBP = false;
bool CBaseCalc::UseTBP = false;

struct CalcTPnt : public CBaseCalc
  {
  double Vol;
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    CBaseCalc::UseTBP = true;
    CalcTPnt * pC = new CalcTPnt;
    strcpy_s ( pC->Name, Len, Cols[0] );
    pC->Vol = atof ( Cols[2] );
    return pC;
    }
  double Calc ( )
    {
    double T = T_PROC( Vol );
    return T;
    }
  NEW
  };

struct CalcConst : public CBaseCalc
  {
  double Const;
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    CalcConst * pC = new CalcConst;
    strcpy_s ( pC->Name, Len, Cols[0] );
    pC->Const = atof ( Cols[2] );
    return pC;
    }
  double Calc ( )
    {
    return Const;
    }
  NEW
  };

struct CalcMass : public CBaseCalc
  {
  int ID;
  double M;
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    CalcMass * pC = new CalcMass;
    strcpy_s ( pC->Name, Len, Cols[0] );
    pC->ID = -1;
    if ( lstrcmp ( Cols[2], "СуммаS" ) == 0 )
      {
      pC->ID = LAST_GAS - 1;
      }
    if ( pC->ID < 0 && lstrcmp ( Cols[2], "СуммаH2S+RSH" ) == 0 )
      {
      pC->ID = K_GAS + 1;
      }
    if ( pC->ID < 0 )
      {
      pC->ID = GetID( Cols[2] );
      }
    if( pC->ID < 0 )
      pC->ID = G_N2;
    pC->M = atof ( Cols[3] );
    ASS( pC->M > 0.)
    if ( ID == LAST_GAS - 1 )
      M *= 32. / 190.05;
    return pC;
    }
  double Calc ( )
    {
    double C = 0.;
    if ( IsComp[ID] )
      C = Cmass[ID] * M;
    return C;
    }
  NEW
  };

struct CalcVPnt : public CBaseCalc
  {
  double Tmax;
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    CBaseCalc::UseTBP = true;
    CalcVPnt * pC = new CalcVPnt;
    strcpy_s ( pC->Name, Len, Cols[0] );
    pC->Tmax = atof ( Cols[2] );
    return pC;
    }
  double Calc ( )
    {
    double V = M_PROC( Tmax );
    return V;
    }
  NEW
  };

const int max_NP = 20;

void AddComp ( char * Formula, int & k, int * ID )
{

}

struct CalcMassNP : public CBaseCalc
  {
  int ID[max_NP];
  int kNP;
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    CalcMassNP * pC = new CalcMassNP;
    strcpy_s ( pC->Name, Len, Cols[0] );
    pC->kNP = 0;
    CLEAR(pC->ID)
    AddComp ( "C2H4", pC->kNP, pC->ID );
    AddComp ( "C3H6", pC->kNP, pC->ID );
    AddComp ( "C4H8", pC->kNP, pC->ID );
    AddComp ( "C5H10", pC->kNP, pC->ID );
    AddComp ( "C6H12", pC->kNP, pC->ID );
    ASS(pC->kNP<max_NP)
    return pC;
    }
  double Calc ( )
    {
    double C = 0.;
    for ( int n = 0; n < kNP; n++ )
      {
      int c = ID[n];
      if ( IsComp[c] )
        C += Cmass[c];
      }
    return C * 100.;
    }
  NEW
  };

struct CalcMassC2C4NP : public CBaseCalc
  {
  int ID[max_NP];
  int kNP;
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    CalcMassC2C4NP * pC = new CalcMassC2C4NP;
    strcpy_s ( pC->Name, Len, Cols[0] );
    pC->kNP = 0;
    CLEAR(pC->ID)
    AddComp ( "C2H4", pC->kNP, pC->ID );
    AddComp ( "C3H6", pC->kNP, pC->ID );
    AddComp ( "C4H8", pC->kNP, pC->ID );
    ASS(pC->kNP<max_NP)
    return pC;
    }
  double Calc ( )
    {
    double C = 0.;
    for ( int n = 0; n < kNP; n++ )
      {
      int c = ID[n];
      if ( IsComp[c] )
        C += Cmass[c];
      }
    return C * 100.;
    }
  NEW
  };

const int max_C = 30;

struct CalcMassC3 : public CBaseCalc
  {
  int ID[max_C];
  int kC;
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    CalcMassC3 * pC = new CalcMassC3;
    strcpy_s ( pC->Name, Len, Cols[0] );
    pC->kC = 0;
    CLEAR(pC->ID)
    AddComp ( "C3H6", pC->kC, pC->ID );
    AddComp ( "C3H8", pC->kC, pC->ID );
    ASS(pC->kC<max_C)
    return pC;
    }
  double Calc ( )
    {
    double C = 0.;
    for ( int n = 0; n < kC; n++ )
      {
      int c = ID[n];
      if ( IsComp[c] )
        C += Cmass[c];
      }
    return C * 100.;
    }
  NEW
  };

struct CalcMassC4 : public CBaseCalc
  {
  int ID[max_C];
  int kC;
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    CalcMassC4 * pC = new CalcMassC4;
    strcpy_s ( pC->Name, Len, Cols[0] );
    pC->kC = 0;
    CLEAR(pC->ID)
    AddComp ( "C4H8", pC->kC, pC->ID );
    AddComp ( "C4H10", pC->kC, pC->ID );
    ASS(pC->kC<max_C)
    return pC;
    }
  double Calc ( )
    {
    double C = 0.;
    for ( int n = 0; n < kC; n++ )
      {
      int c = ID[n];
      if ( IsComp[c] )
        C += Cmass[c];
      }
    return C * 100.;
    }
  NEW
  };

struct CalcMassC4butene : public CBaseCalc
  {
  int ID[max_C];
  int kC;
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    CalcMassC4butene * pC = new CalcMassC4butene;
    strcpy_s ( pC->Name, Len, Cols[0] );
    pC->kC = 0;
    CLEAR(pC->ID)
    AddComp ( "C4H8", pC->kC, pC->ID );
    ASS(pC->kC<max_C)
    return pC;
    }
  double Calc ( )
    {
    double C = 0.;
    for ( int n = 0; n < kC; n++ )
      {
      int c = ID[n];
      if ( IsComp[c] )
        C += Cmass[c];
      }
    return C * 100.;
    }
  NEW
  };

struct CalcMassC5 : public CBaseCalc
  {
  int ID[max_C];
  int kC;
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    CalcMassC5 * pC = new CalcMassC5;
    strcpy_s ( pC->Name, Len, Cols[0] );
    strcpy_s ( pC->Name, Len, Cols[0] );
    pC->kC = 0;
    CLEAR(pC->ID)
    AddComp ( "C5H10", pC->kC, pC->ID );
    AddComp ( "C5H12", pC->kC, pC->ID );
    ASS(pC->kC<max_C)
    return pC;
    }
  double Calc ( )
    {
    double C = 0.;
    for ( int n = 0; n < kC; n++ )
      {
      int c = ID[n];
      if ( IsComp[c] )
        C += Cmass[c];
      }
    return C * 100.;
    }
  NEW
  };

struct CalcMassC5plus : public CBaseCalc
  {
  int ID[max_C];
  int kC;
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    CalcMassC5plus * pC = new CalcMassC5plus;
    strcpy_s ( pC->Name, Len, Cols[0] );
    pC->kC = 0;
    CLEAR(pC->ID)
    AddComp ( "C5H10", pC->kC, pC->ID );
    AddComp ( "C5H12", pC->kC, pC->ID );
    AddComp ( "C6H12", pC->kC, pC->ID );
    AddComp ( "C6H14", pC->kC, pC->ID );
    AddComp ( "C6H6", pC->kC, pC->ID );
    ASS(pC->kC<max_C)
    return pC;
    }
  double Calc ( )
    {
    double C = 0.;
    for ( int n = 0; n < kC; n++ )
      {
      int c = ID[n];
      if ( IsComp[c] )
        C += Cmass[c];
      }
    return C * 100.;
    }
  NEW
  };

struct CalcMass_H2S_RSH : public CBaseCalc
  {
  int ID[2];
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    CalcMass_H2S_RSH * pC = new CalcMass_H2S_RSH;
    strcpy_s ( pC->Name, Len, Cols[0] );
    pC->ID[0] = GetGasID ( "Сероводород" );
    pC->ID[1] = G_СуммаS;
    return pC;
    }
  double Calc ( )
    {
    double C = 0.;
    for ( int n = 0; n < 1; n++ )
      {
      int c = ID[n];
      if ( c >= 0 && IsComp[c] )
        C += Cmass[c];
      }
    return C * 100.;
    }
  NEW
  };


CBaseCalc * Get ( char Cols[kCol][Len] )
  {
  CalcMass * pC = new CalcMass;
  strcpy_s ( pC->Name, Len, Cols[0] );
  return pC;
  }

struct CalcMol : public CBaseCalc
  {
  int ID;
  double M;
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    CalcMol * pC = new CalcMol;
    strcpy_s ( pC->Name, Len, Cols[0] );
    pC->ID = -1;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( lstrcmp ( pGas[c]->Name, Cols[2] ) == 0 ||
           ( pGas[c]->Alt_Name && lstrcmp ( pGas[c]->Alt_Name, Cols[2] ) == 0 ))
        {
        pC->ID = c;
        break;
        }
      }
    ASS(pC->ID>=0)
    pC->M = atof ( Cols[3] );
    ASS(pC->M>0.)
    return pC;
    }
  double Calc ( )
    {
    double C = 0.;
    if ( IsComp[ID] )
      C = Cmol[ID] * M;
    return C;
    }
  NEW
  };

struct CalcMolDry : public CBaseCalc
  {
  int ID;
  double M;
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    CalcMolDry * pC = new CalcMolDry;
    strcpy_s ( pC->Name, Len, Cols[0] );
    pC->ID = GetID( Cols[2] );
    ASS(pC->ID>=0)
    pC->M = atof ( Cols[3] );
    ASS(pC->M>0.)
    return pC;
    }
  double Calc ( )
    {
    double C = 0.;
    double C_H2O = 0.;
    if ( IsComp[G_H2O] )
      C_H2O = Cmol[G_H2O];
    if ( C_H2O > 0.99 )
      C_H2O = 0.99;
    if ( IsComp[ID] )
      C = Cmol[ID] * M / ( 1. - C_H2O );
    return C;
    }
  NEW
  };

struct Calc_мг_м3 : public CBaseCalc
  {
  int ID;
  double M;
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    Calc_мг_м3 * pC = new Calc_мг_м3;
    strcpy_s ( pC->Name, Len, Cols[0] );
    pC->ID = GetID( Cols[2] );
    ASS(pC->ID>=0)
    pC->M = atof ( Cols[3] );
    ASS(pC->M>0.)
    return pC;
    }
  double Calc ( )
    {
    double C = 0.;
    double C_H2S = 0.;
    if ( G_H2S >= 0 && IsComp[G_H2S] )
      C_H2S = Cmol[G_H2S] * pGas[G_H2S]->Mw;
// 1 моль = 22.4 м3
    double M = C_H2S * 1e6; //мг
    const double V = 22.4;
    return M / V;
    }
  NEW
  };

struct Calc_Ar_x : public CBaseCalc
  {
  int ID;
  double M;
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    Calc_Ar_x * pC = new Calc_Ar_x;
    strcpy_s ( pC->Name, Len, Cols[0] );
    pC->ID = GetID( Cols[2] );
    ASS(pC->ID>=0)
    pC->M = atof ( Cols[3] );
    ASS(pC->M>0.)
    return pC;
    }
  double Calc ( )
    {
    double C = 0.;
    if ( IsComp[ID] )
      C = Cmol[ID];
    double Ar = 0.;
    if ( IsComp[G_AR] )
      Ar = Cmol[G_AR];
    return ( Ar + C ) * M;
    }
  NEW
  };

struct CalcRo : public CBaseCalc
  {
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    CalcRo * pC = new CalcRo;
    strcpy_s ( pC->Name, Len, Cols[0] );
    return pC;
    }
  double Calc ( )
    {
    double Mw = ::Mw ( pComp );
    double To_m3 = To_m3_liq ( 20., pComp );
    double Ro = 0.;
    if ( To_m3 > 0. )
      Ro = Mw / To_m3;
    if ( Ro < 100. )
      {
      Ro = Mw / 293.15;
      }
    return Ro;
    }
  NEW
  };

struct MW : public CBaseCalc
  {
  double M;
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    MW * pC = new MW;
    strcpy_s ( pC->Name, Len, Cols[0] );
	pC->M = atof ( Cols[3] );
    //ASS( pC->M > 0.)
    return pC;
    }
  double Calc ( )
    {
    double Mw = ::Mw ( pComp ) * M;
    return Mw;
    }
  NEW
  };

struct CalcRo15 : public CBaseCalc
  {
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    CalcRo15 * pC = new CalcRo15;
    strcpy_s ( pC->Name, Len, Cols[0] );
    return pC;
    }
  double Calc ( )
    {
    double Mw = ::Mw ( pComp );
    double To_m3 = To_m3_liq ( 15., pComp );
    double Ro = 0.;
    if ( To_m3 > 0. )
      Ro = Mw / To_m3;
    if ( Ro < 100. )
      {
      Ro = Mw / 288.15;
      }
    return Ro;
    }
  NEW
  };

struct Calc_Text : public CBaseCalc
{
	CBaseCalc * Get ( char Cols[kCol][Len] )
	{
		Calc_Text * pC = new Calc_Text;
		int nMaxLen = lstrlen(Cols[0]) + 1 + lstrlen(Cols[2]) + 1;
		ASS(nMaxLen <= 64)
        sprintf(pC->Name, "%s$%s", Cols[0], Cols[2]);
		return pC;
	}
	double Calc ( )
	{
		return 0.;
	}
	NEW
};

struct TBP_Vol : public CBaseCalc
  {
  double Tb, Te;
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    CBaseCalc::UseTBP = true;
    TBP_Vol * pC = new TBP_Vol;
    strcpy_s ( pC->Name, Len, Cols[0] );
    pC->Tb = atof ( Cols[2] );
    pC->Te = atof ( Cols[3] );
    return pC;
    }
  double Calc ( )
    {
    double dV = V_PROC( Tb, Te );
    return dV;
    }
  NEW
  };

struct TBP_Temp : public CBaseCalc
  {
  double Vz;
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    CBaseCalc::UseTBP = true;
    TBP_Temp * pC = new TBP_Temp;
    strcpy_s ( pC->Name, Len, Cols[0] );
    pC->Vz = atof ( Cols[2] );
    if ( pC->Vz == 0. )
      pC->Vz = 2.;
    return pC;
    }
  double Calc ( )
    {
    double T = T_PROC( Vz );
    return T;
    }
  NEW
  };

struct CalcPz : public CBaseCalc
  {
  double Tz;
  CBaseCalc * Get ( char Cols[kCol][Len] )
    {
    CalcPz * pC = new CalcPz;
    strcpy_s ( pC->Name, Len, Cols[0] );
    pC->Tz = atof ( Cols[2] );
    return pC;
    }
  double Calc ( )
    {
    double P = Pz ( Tz, pComp );
    return ( P - 1. ) * iMPa_atm;
    }
  NEW
  };

struct CalcSummKsilols: public CBaseCalc
{
	double M;
	int ID[3];
	int nID;
	CBaseCalc * Get ( char Cols[kCol][Len] )
	{
		CalcSummKsilols * pC = new CalcSummKsilols;
		pC->nID = 0;
		strcpy_s ( pC->Name, Len, Cols[0] );
		pC->M = atof ( Cols[3] );
		for(int i = 0; i < LAST_GAS; i++)
		{
			if(!lstrcmp(pGas[i]->Name, "o-XYLENE") || !lstrcmp(pGas[i]->Name, "m-XYLENE") || !lstrcmp(pGas[i]->Name, "p-XYLENE"))
				pC->ID[pC->nID++] = i;
		}
		return pC;
	}
	double Calc ( )
	{
		double C = 0.0;
		for ( int i = 0; i < nID; i++ )
		{
			int c = ID[i];
			if ( IsComp[c] )
				C += Cmass[c];
		}		
		return C * M;
	}
	NEW

};

struct CalcParafinNaften: public CBaseCalc
{
	double M;
	int ID[5];
	int nID;
	CBaseCalc * Get ( char Cols[kCol][Len] )
	{
		CalcParafinNaften * pC = new CalcParafinNaften;
		pC->nID = 0;
		strcpy_s ( pC->Name, Len, Cols[0] );
		pC->M = atof ( Cols[3] );
		for(int i = 0; i < LAST_GAS; i++)
		{
			if(!lstrcmp(pGas[i]->Name, "o-XYLENE") || !lstrcmp(pGas[i]->Name, "m-XYLENE") || !lstrcmp(pGas[i]->Name, "p-XYLENE") || 
				!lstrcmp(pGas[i]->Name, "BENZENE") || !lstrcmp(pGas[i]->Name, "TOLUENE"))
				pC->ID[pC->nID++] = i;
		}
		return pC;
	}
	double Calc ( )
	{
		double C = 0.0;
		for ( int i = 0; i < nID; i++ )
		{
			int c = ID[i];
			if ( IsComp[c] )
				C += Cmass[c];
		}		
		return (1.0 - C - Cmass[G_N2]) * M;
	}
	NEW

};

struct CalcSumm: public CBaseCalc
{
	double M;
	int ID[K_GAS];
	int nID;
	CBaseCalc * Get ( char Cols[kCol][Len] )
	{

		CalcSumm * pC = new CalcSumm;
		return pC;
	}
	double Calc ( )
	{
		double C = 0.0;
		for ( int i = 0; i < nID; i++ )
		{
			int c = ID[i];
			if ( IsComp[c] )
				C += Cmass[c];
		}		
		return C * M;
	}
	NEW

};

struct CCalc
  {
  char * Name;
  CBaseCalc * pCalc;
  };

#undef CALC
#define CALC(Name,Class) Name, NULL,

CCalc Calculator[] = {
CALC("TBP", CalcTPnt)
CALC("Константа", CalcConst)
CALC("Масс%", CalcMass)
CALC("Масс%<T", CalcVPnt)
CALC("Масс%непредельные", CalcMassNP)
CALC("Масс%С2-С4непредельные", CalcMassC2C4NP)
CALC("Масс%С3", CalcMassC3)
CALC("Масс%С4", CalcMassC4)
CALC("Масс%С4бутены", CalcMassC4butene)
CALC("Масс%С5", CalcMassC5)
CALC("Масс%С5+", CalcMassC5plus)
CALC("Моль%", CalcMol)
CALC("Плотность20", CalcRo)
CALC("Моль%Сухие", CalcMolDry)
CALC("мг/м3", Calc_мг_м3)
CALC("Ar+N2", Calc_Ar_x)
CALC("Ar+CH4", Calc_Ar_x)
CALC("Текст", Calc_Text)
CALC("Масс%СуммаH2S+RSH", CalcMass_H2S_RSH )
CALC("Интервал", TBP_Vol )
CALC("Перегон", TBP_Temp )
CALC("Плотность15", CalcRo15)
CALC("Давление", CalcPz)
CALC("Сумма ксилолов", CalcSummKsilols)
CALC("Парафины+нафтены", CalcParafinNaften)
CALC("Сумма", CalcSumm)
CALC("МолВес", MW)
NULL };

#undef CALC
#define CALC(Name,Class) if ( lstrcmp ( Name, CalcName ) == 0 ) return new Class;

CBaseCalc * CreateCalc( char * CalcName )
  {
  CALC("TBP", CalcTPnt)
  CALC("Константа", CalcConst)
  CALC("Масс%", CalcMass)
  CALC("Масс%<T", CalcVPnt)
  CALC("Масс%непредельные", CalcMassNP)
  CALC("Масс%С2-С4непредельные", CalcMassC2C4NP)
  CALC("Масс%С3", CalcMassC3)
  CALC("Масс%С4", CalcMassC4)
  CALC("Масс%С4бутены", CalcMassC4butene)
  CALC("Масс%С5", CalcMassC5)
  CALC("Масс%С5+", CalcMassC5plus)
  CALC("Моль%", CalcMol)
  CALC("Плотность20", CalcRo)
  CALC("Моль%Сухие", CalcMolDry)
  CALC("мг/м3", Calc_мг_м3)
  CALC("Ar+N2", Calc_Ar_x)
  CALC("Ar+CH4", Calc_Ar_x)
  CALC("Текст", Calc_Text)
  CALC("Масс%СуммаH2S+RSH", CalcMass_H2S_RSH )
  CALC("Интервал", TBP_Vol )
  CALC("Перегон", TBP_Temp )
  CALC("Плотность15", CalcRo15)
  CALC("Давление", CalcPz)
  CALC("Сумма ксилолов", CalcSummKsilols)
  CALC("Парафины+нафтены", CalcParafinNaften)
  CALC("Сумма", CalcSumm)
  CALC("МолВес", MW)
  return NULL;
  }

#define MAX_PROC 80
static CBaseCalc * CalcProc[MAX_PROC];
static bool SetProc = false;
static int kProc = 0;

void CSens::NewPrepareAnalyz( )
{
	if ( !Анализатор )
		return;
	if ( !SetProc )
	{
		SetProc = true;       
		Char<512>Path;
		Path.Prt( "%sDATA\\Equipment\\Анализаторы\\Компоненты.csv", PROJECT_ROOT );
		FILE * F = fopen ( Path, "rt" );
		if ( F == NULL )
			return;
		CLEAR(UseAnalysComp)
			char Str[256];
		fgets( Str, 255, F );
		char Cols[kCol][Len];
		while ( Split<kCol,Len> ( F, (char*)Cols)>0)
		{
			//
			int n = GetGasID( Cols[0] );
			if ( n >= 0 )
				UseAnalysComp[n] = true;
		}
		fclose ( F );
	}
	if ( CBaseCalc::UseTBP )
	{
		CBaseCalc::pTBP = new CTBP;
		CBaseCalc::kPnt_TBP = 100;
		CBaseCalc::V_TBP = NewArr(double,CBaseCalc::kPnt_TBP);
		CBaseCalc::T_TBP = NewArr(double,CBaseCalc::kPnt_TBP);
		CoefTBP = NULL;
		kCoefTBP = 0;
		Char<512>Path;
		Path.Prt( "%sDATA\\Equipment\\Анализаторы\\К-ты_%s.csv", PROJECT_ROOT, (char*)ObjName );
		FILE * F = fopen ( Path, "rt" );
		if ( F )
		{
			kCoefTBP = 21; 
			CoefTBP = NewArr(double,kCoefTBP);
			for ( int n = 0; n < kCoefTBP; n++ )
				CoefTBP[n] = 0.;
			char Str[256];
			fgets( Str, 255, F );
			if ( Str[1] == '+' )
				AddCoefTBP= true;
			else
				AddCoefTBP= false;
			char Cols[1][Len];
			int n = 0;
			while ( Split<1,Len> ( F, (char*)Cols)>0)
			{
				if ( n >= kCoefTBP )
					break;
				CoefTBP[n++] = atof( Cols[0] );
			}
			fclose ( F );
		}
	}


    GiveCommData ( ObjName, CGasAnalyz::TypeID, (void**)&pAnalyz );

	CLEAR(pAnalyz->Table);
	CLEAR(pAnalyz->Main_Curve);
	CLEAR(pAnalyz->Save_Curve);
	CLEAR(pAnalyz->Name);
	pAnalyz->bNewStructIsUsed = true;
	pAnalyz->CalcData = false; // Рассчитывать Table, Main_Curve т.к. окно стало выводиться
	pAnalyz->UseTable = false;  // Table, Main_Curve рассчитаны и могут использоваться
	pAnalyz->UseCurve = false;
	pAnalyz->SaveData = false; // Нажата кнопка сохранить кривую (сбрасывается в модели)
	pAnalyz->UseSave = false;  // Save_Curve рассчитана и может использоваться
	pAnalyz->ClearSave = false;  // Save_Curve больше рассчитывать не надо
}

CBaseCalc * CreateCalc( char * CalcName );

void CSens::PrepareAnalyz( )
  {
  if ( !Анализатор )
    return;
  if ( !SetProc )
    {
    SetProc = true;       
    Char<512>Path;
    Path.Prt( "%sDATA\\Equipment\\Анализаторы\\Анализаторы.csv", PROJECT_ROOT );
    FILE * F = fopen ( Path, "rt" );
    if ( F == NULL )
      return;
    char Str[256];
    fgets( Str, 255, F );
    char Cols[kCol][Len];
    while ( Split<kCol,Len> ( F, (char*)Cols)>0)
      {
NO_READ:
  //
      int n = 0;
      CBaseCalc * pC = NULL;
      while ( Calculator[n].Name )
        {
        if ( lstrcmp ( Cols[1], Calculator[n].Name ) == 0 )
          {
          if ( Calculator[n].pCalc == NULL )
            Calculator[n].pCalc = CreateCalc( Calculator[n].Name );
		  if(!lstrcmp(Calculator[n].Name, "Сумма"))
		  {
			  pC = Calculator[n].pCalc->Get( Cols );
			  CalcSumm* pCs = (CalcSumm*)pC;
			  strcpy_s ( pCs->Name, Len, Cols[0] );
			  pCs->M = atof ( Cols[3] );
			  while ( Split<kCol,Len> ( F, (char*)Cols)>0)
			  {
				  if(Cols[0][0] != 0)
				  {
					  if ( pC )
						  CalcProc[kProc++] = pC;
					  goto NO_READ;
				  }
				  pCs->ID[pCs->nID++] = GetGasID(Cols[2]);

			  }
			  
		  }
		  else
			  pC = Calculator[n].pCalc->Get( Cols );
          break;
          }
        n++;
        }
//      ASS(pC)
      if ( pC )
        CalcProc[kProc++] = pC;
      }
    fclose ( F );
    }
  }

void CSens::SetAnalyzData( )
  {
  kAnalyzData = 0;
  if ( !Анализатор )
    return;
  Char<512>Path;
  Path.Prt( "%sDATA\\Equipment\\Анализаторы\\%s.csv", PROJECT_ROOT, (char*)ObjName );
  FILE * F = fopen ( Path, "rt" );
  if ( F == NULL )
    {
//    ASS(0)
    return;
    }
  GiveCommData ( ObjName, CGasAnalyz::TypeID, (void**)&pAnalyz );
  CLEAR(pAnalyz->Name)
  CLEAR(pAnalyz->DecimalPlace)
  pAnalyz->bNewStructIsUsed = false;
  CalcAnalyz = NewArr(int,MAX_ANALYZ_POINTS);
  kAnalyzData = 0;
  char Cols[kCol][Len];
  while ( Split<kCol,Len> ( F, (char*)Cols) > 0 )
    {
    ASS(kAnalyzData<MAX_ANALYZ_POINTS)
    if ( kAnalyzData >= MAX_ANALYZ_POINTS )
      break;
    for ( int n = 0; n < kProc; n++ )
      {
      if ( memcmp ( CalcProc[n]->Name, Cols[0], lstrlen(Cols[0]) ) == 0 )
        {
        CalcAnalyz[kAnalyzData] = n;
        lstrcpy ( pAnalyz->Name[kAnalyzData], CalcProc[n]->Name );
        pAnalyz->DecimalPlace[kAnalyzData] = atoi ( Cols[1] );
        kAnalyzData++;
        break;
        }
      }
    }
  fclose ( F );
  }

void CSens::Analyz( )
  {
  if ( pAnalyz == NULL || kAnalyzData <= 0 )
    return;
  CBaseCalc::pComp = pF->Get_Comp();
  if ( CBaseCalc::pComp == NULL )
    return;
  CBaseCalc::IsComp = CBaseCalc::pComp->IsComp;
  CBaseCalc::Cmol = CBaseCalc::pComp->Cmol;
  CLEAR(CBaseCalc::Cmass)
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( CBaseCalc::pComp->IsComp[c] )
      CBaseCalc::Cmass[c] = CBaseCalc::pComp->Cmol[c] * pGas[c]->Mw;
    }
  Norm( CBaseCalc::pComp->IsComp, CBaseCalc::Cmass );
  //
  for ( int n = 0; n < kAnalyzData; n++ )
    {
    double V = CalcProc[CalcAnalyz[n]]->Calc();
    //if ( V == 0. )
    //  pAnalyz->C[n] = k_Analyz[n];
    //else
    pAnalyz->C[n] = V * k_Analyz[n];
    }
  }

double CSens::CalcCoefTBP( double V )
  {
  if ( CoefTBP == NULL )
    return 1.;
  int n = (int)( V / 5 );
  if ( n >= 20 )
    n = 19;
  double Vb = n * 5.;
  double C = CoefTBP[n] + ( V - Vb ) * ( CoefTBP[n+1] - CoefTBP[n] ) * 0.2;
  return 1. + C;
  }

void CSens::NewAnalyz( )
{
	if ( pAnalyz == NULL || pAnalyz->Busy == 0 )
		return;
	CBaseCalc::pComp = pF->Get_Comp();
	if ( CBaseCalc::pComp == NULL )
		return;
	CBaseCalc::IsComp = CBaseCalc::pComp->IsComp;
	CBaseCalc::Cmol = CBaseCalc::pComp->Cmol;
	CLEAR(CBaseCalc::Cmass)
		if ( МассовыеДоли )
		{
			for ( int c = 0; c < LAST_GAS; c++ )
			{
				if ( CBaseCalc::pComp->IsComp[c] )
					CBaseCalc::Cmass[c] = CBaseCalc::pComp->Cmol[c] * pGas[c]->Mw;
			}
			Norm( CBaseCalc::pComp->IsComp, CBaseCalc::Cmass );
		}
		if ( pAnalyz->SaveData )
		{
			pAnalyz->SaveData = false;
			memmove ( pAnalyz->Save_Curve, pAnalyz->Main_Curve, sizeof ( pAnalyz->Save_Curve) );
			pAnalyz->UseSave = true;
		}
		if ( pAnalyz->ClearSave )
		{
			pAnalyz->ClearSave = false;
			pAnalyz->UseSave = false;
		}
		int n = 0;
		for ( int c = 0; c < LAST_GAS; c++ )
		{
			if ( !UseAnalysComp[c] )
				continue;
			double V = 0.;
			if ( CBaseCalc::IsComp[c] )
			{
				if ( МассовыеДоли )
					V = CBaseCalc::Cmass[c] * 100.;
				else
					V = CBaseCalc::Cmol[c] * 100.;
			}
			pAnalyz->Table[n++] = V;
		}
		pAnalyz->UseTable = true;
		//
		if ( CBaseCalc::UseTBP )
		{
			CBaseCalc::kPnt_TBP = 100;
			CBaseCalc::IsTBP = CBaseCalc::pTBP->Calc ( CBaseCalc::pComp, CBaseCalc::kPnt_TBP, CBaseCalc::V_TBP, CBaseCalc::T_TBP );
			if ( CBaseCalc::IsTBP )
			{
				double V_max = CBaseCalc::V_TBP[99];
				double kV_max = 100. / V_max;
				for ( int n = 0; n < CBaseCalc::kPnt_TBP; n++ )
				{
					double V = CBaseCalc::V_TBP[n] * kV_max;
					pAnalyz->Main_Curve[0][n] = V;
					if ( AddCoefTBP )
						pAnalyz->Main_Curve[1][n] = CBaseCalc::T_TBP[n] + CalcCoefTBP( V );
					else
						pAnalyz->Main_Curve[1][n] = CBaseCalc::T_TBP[n] * CalcCoefTBP( V );
				}
				pAnalyz->Main_Curve[0][99] = 100.;
				pAnalyz->UseCurve = true;
			}
			else
				pAnalyz->UseCurve = false;
		}
		else
			pAnalyz->UseCurve = false;
}

double CBaseCalc::T_PROC( double Proc )
  {
  if ( !IsTBP )
    return 0.;
  double P = Proc;
  int nb = 0;
  for ( int n = 1; n < kPnt_TBP; n++ )
    {
    if ( P <= V_TBP[n+1] )
      {
      nb = n;
      break;
      }
    }
  double Vb = V_TBP[nb];
  double Ve = V_TBP[nb+1];
  if ( Ve - Vb < 1e-5 )
    {
    double Tb = T_TBP[nb];
    return Tb;
    }
  double Tb = T_TBP[nb];
  double Te = T_TBP[nb+1];
  double T = Tb + ( Te - Tb ) * ( P - Vb ) / ( Ve - Vb );
  return T;
  }

double CBaseCalc::M_PROC( double T )
  {
  double M_LT = 0.;
  double M = 0.;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( IsComp[c] )
      {
      if ( pGas[c]->Tb < T )
        M_LT += Cmol[c] * pGas[c]->Mw;
      M += Cmol[c] * pGas[c]->Mw;
      }
    }
  double V = M_LT / M * 100.;
  return V;
  }

double CBaseCalc::V_PROC( double T )
  {
  if ( !IsTBP )
    return 0.;
  int nb = 0;
  for ( int n = 1; n < kPnt_TBP; n++ )
    {
    if ( T <= T_TBP[n+1] )
      {
      nb = n;
      break;
      }
    }
  if ( nb == 0 )
    return 100.;
  double Tb = T_TBP[nb];
  double Te = T_TBP[nb+1];
  if ( Te - Tb < 1e-5 )
    {
    double Vb = V_TBP[nb];
    return Vb;
    }
  double Vb = V_TBP[nb];
  double Ve = V_TBP[nb+1];
  double V = Vb + ( Ve - Vb ) * ( T - Tb ) / ( Te - Tb );
  return V;
  }

double CBaseCalc::V_PROC( double Tb, double Te )
  {
  if ( Tb == 0. )
    return V_PROC( Te );
  double Vb = V_PROC( Tb );
  double Ve = V_PROC( Te );
  return Ve - Vb;
  }


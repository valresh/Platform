#include "stdafx.h"
#include "Chem.h"
#include "math.h"
#include "Err.h"
#include "Data.h"



struct CCaCl2 : public CGas 
{
	CCaCl2();
	double dHobr( double Tc );
	double dSobr( double Tc );
	double dGobr( double Tc );
	void Init();          //+
	double H_Chem ( double TC );
	double dH_Chem ( );
	double G_IG( double P, double TC );  //+
	double H_vap ( double TC );
	double C_IG ( double TC );
	double H_IG ( double TC );
	double S_IG( double P, double TC );
	void S_IG( double P, double TC, double &S, double & dSdT );
	double H_v ( double TC );
	double H_l ( double TC );
	double  dH_1_dTC (double TC);
	double C_Liq ( double TC ); //+
	double Pz ( double T_C );
	double dPz_dT ( double T_C );
	bool Ro_liq ( double T_C, double & Ro );
	bool To_m3_liq ( double TC, double & To_m3 );
	void GetParams ( IBaseModel * pMain );
#ifdef LINUX
    virtual int ReadData( QFile * hFile );
#else
    virtual int ReadData( _HANDLE hFile );
#endif
};


CCaCl2 CaCl2;
CGas * pCaCl2;

const double Cp_CaCl2 = 1. * 72.63; // 1кДж/кг 
const double Ro_CaCl2 = 2512.0;

CGas * Create_CaCl2()
{
	return &CaCl2;
}

CCaCl2::CCaCl2()
{
	lstrcpy ( Name, "CaCl2" );
	pCaCl2 = this;
	Tcr = 1000.;
}

double CCaCl2::C_IG ( double TC )
{
	return Cp_CaCl2;
}

double CCaCl2::H_IG ( double TC )
{
	return Cp_CaCl2 * ( TC - 25.);
}

double CCaCl2::S_IG( double P, double TC )
{
	return 0.;
}

void CCaCl2::S_IG( double P, double TC, double &S, double & dSdT )
{
	ASS(0)
}

double CCaCl2::H_v ( double TC )
{
	return Cp_CaCl2 * ( TC - 25.);
}

double CCaCl2::H_l ( double TC )
{
	return Cp_CaCl2 * ( TC - 25.);
}


double  CCaCl2::dH_1_dTC (double TC)
{
	return Cp_CaCl2;
}


double CCaCl2::C_Liq ( double TC )
{
	return Cp_CaCl2;
}

double CCaCl2::Pz ( double T_C )
{
	return 0.0001;
}

double CCaCl2::dPz_dT ( double T_C )
{
	return 0.;
}

bool CCaCl2::Ro_liq ( double T_C, double & Ro )
{
	Ro = Ro_CaCl2;
	return true;
}
bool CCaCl2::To_m3_liq ( double TC, double & To_m3 )
{
	To_m3 = Mw / Ro_CaCl2;
	return true;
}

double CCaCl2::dHobr( double TC )
{
	ASS(0)
		return 0.;
}

double CCaCl2::dSobr( double Tc )
{
	ASS(0)
		return 0.;
}

double CCaCl2::dGobr( double Tc )
{
	ASS(0)
		return 0.;
}

void CCaCl2::Init()
{
	return;
}

double CCaCl2::H_Chem ( double TC )
{
	ASS(0)
		return Cp_CaCl2 * ( TC - 25.);
}

double CCaCl2::dH_Chem ( )
{
	ASS(0)
		return 0.;
}

double CCaCl2::G_IG( double P, double TC )
{
	ASS(0)
		return 0.;
}

double CCaCl2::H_vap ( double TC )
{
	return 0.;
}

void CCaCl2::GetParams ( IBaseModel * pMain )
{
	ASS(0)
}

int CCaCl2::ReadData( _HANDLE hFile )
  {
	// Ключ уже прочитан
	DWORD Lr;
	CRealDBData DBData;
	ReadFile ( hFile, &DBData, sizeof ( DBData ), &Lr, NULL );
	lstrcpy ( Name, "CaCl2" );
	Mw = 110.984;
	Tcr = 1000.;
	return 0;
  }

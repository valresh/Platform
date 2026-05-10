#include "stdafx.h"
#include "Chem.h"
#include "math.h"
#include "Err.h"
#include "Data.h"

//MALEIC ANHYDRIDE
#if 0
struct CMAN : CReal
  {
  static double P0, T0, P1, T1;
  static double A, B;
  static double kdH;
  static bool Use_AB;
  static void CalcAB();
  virtual void Init();
  virtual double H_vap ( double TC );
  virtual double  Pz ( double Tc );
  };

double CMAN::P0 = 0.;
double CMAN::T0 = 98.;
double CMAN::P1 = 0.;
double CMAN::T1 = 193.;
double CMAN::A = 0.;
double CMAN::B = 0.;
double CMAN::kdH = 1.;
bool CMAN::Use_AB = false;


CGas * Create_MAN()
  {
  return new CMAN();
  }

struct IMANParams : public IChemParams
  {
  IMANParams();
  void GetParams( struct IBaseModel * pMain );
  void UpdateParam( struct CParams & Param );
  };

IMANParams::IMANParams()
  {
  pNextChemParam = pFirsrtChemParam;
  pFirsrtChemParam = this;
  }

#include "IO_Parms.h"
#define this pMain
void IMANParams::GetParams( struct IBaseModel * pMain )
  {
  TAB("MAN",1)
    PARM( CMAN::Use_AB, "MAN:Использовать данные" )
    PARM( CMAN::P0, "MAN:P0" )
    PARM( CMAN::T0, "MAN:T0" )
    PARM( CMAN::P1, "MAN:P1" )
    PARM( CMAN::T1, "MAN:T1" )
    PARM( CMAN::kdH, "MAN:kdH" )
  ETAB
  if ( pNextChemParam )
    pNextChemParam->GetParams( pMain );
  }
#undef this

void IMANParams::UpdateParam( struct CParams & Param )
  {
  if ( pNextChemParam )
    pNextChemParam->UpdateParam( Param );
  CMAN::CalcAB();
  }

void CMAN::CalcAB()
  {
  double L0 = log ( P0 );
  double L1 = log ( P1 );
  double dL = L0 - L1;
  double dT = 1. / ( T0 + TK ) - 1. / ( T1 + TK );
  B = dL / dT;
  A = L0 - B / ( T0 + TK );
  }

IMANParams * pMANParams;
void CMAN::Init()
  {
  pMANParams = new IMANParams;
  CReal::Init();
  }

double  CMAN::H_vap ( double TC )
  {
  double dH = CReal::H_vap( TC );
  return dH * kdH;
  }

// P - атм(а), Tc - С
double CMAN::Pz ( double Tc )
  {
  if ( Use_AB )
    {
    if ( A == 0. )
      CalcAB();
    double P = exp ( A + B / ( Tc + TK ));
    return P;
    }
  if ( P0 == 0.)
    P0 = CReal::Pz ( T0 );
  if ( P1 == 0.)
    P1 = CReal::Pz ( T1 );
//
  double P = CReal::Pz ( Tc );
// Если закомментировать предыдущую строку и написать ниже любую другую формулу, то она будет работать
//  double P = 0.9869*exp(38.5001 - 7473.2/(Tc + 273.15) - 3.6993*log(Tc + 273.15) + 3.5517e-18*pow((Tc + 273.15), 6));
  return P;
  }

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//DIBUTYL PHTHALATE
struct CDBPH : CReal
  {
  static double P0, T0, P1, T1;
  static double A, B;
  static double kdH;
  static bool Use_AB;
  static void CalcAB();
  virtual void Init();
  virtual double H_vap ( double TC );
  virtual double  Pz ( double Tc );
  };

double CDBPH::P0 = 0.;
double CDBPH::T0 = 98.;
double CDBPH::P1 = 0.;
double CDBPH::T1 = 193.;
double CDBPH::A = 0.;
double CDBPH::B = 0.;
double CDBPH::kdH = 1.;
bool CDBPH::Use_AB = false;

CGas * Create_DBPH()
  {
  return new CDBPH();
  }

struct IDBPHParams : public IChemParams
  {
  IDBPHParams();
  void GetParams( struct IBaseModel * pMain );
  void UpdateParam( struct CParams & Param );
  };

IDBPHParams::IDBPHParams()
  {
  pNextChemParam = pFirsrtChemParam;
  pFirsrtChemParam = this;
  }

#include "IO_Parms.h"
#define this pMain
void IDBPHParams::GetParams( struct IBaseModel * pMain )
  {
  TAB("DBPH",1)
    PARM( CDBPH::Use_AB, "DBPH:Использовать данные" )
    PARM( CDBPH::P0, "DBPH:P0" )
    PARM( CDBPH::T0, "DBPH:T0" )
    PARM( CDBPH::P1, "DBPH:P1" )
    PARM( CDBPH::T1, "DBPH:T1" )
    PARM( CDBPH::kdH, "DBPH:kdH" )
  ETAB
  if ( pNextChemParam )
    pNextChemParam->GetParams( pMain );
  }
#undef this

void IDBPHParams::UpdateParam( struct CParams & Param )
  {
  if ( pNextChemParam )
    pNextChemParam->UpdateParam( Param );
  CDBPH::CalcAB();
  }

void CDBPH::CalcAB()
  {
  double L0 = log ( P0 );
  double L1 = log ( P1 );
  double dL = L0 - L1;
  double dT = 1. / ( T0 + TK ) - 1. / ( T1 + TK );
  B = dL / dT;
  A = L0 - B / ( T0 + TK );
  }

IDBPHParams * pDBPHParams;
void CDBPH::Init()
  {
  pDBPHParams = new IDBPHParams;
  CReal::Init();
  }

double  CDBPH::H_vap ( double TC )
  {
  double dH = CReal::H_vap( TC );
  return dH * kdH;
  }

// P - атм(а), Tc - С
double  CDBPH::Pz ( double Tc )
  {
  if ( Use_AB )
    {
    if ( A == 0. )
      CalcAB();
    double P = exp ( A + B / ( Tc + TK ));
    return P;
    }
  if ( P0 == 0.)
    P0 = CReal::Pz ( T0 );
  if ( P1 == 0.)
    P1 = CReal::Pz ( T1 );
  //
  double P = CReal::Pz ( Tc );
  // Если закомментировать предыдущую строку и написать ниже любую другую формулу, то она будет работать
  //double P = 0.001316*exp(152.675 - 10754.0/(Tc + 273.15) - 51.17*log(Tc + 273.15) + 0.016933*(Tc + 273.15) + 2.4948e-14*pow((Tc + 273.15), 2));
  return P;
  }
#endif
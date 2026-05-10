#pragma once
#ifndef LINUX
#pragma warning ( disable : 4996 )
#include "Interface/CompInterface.h"
#else
#include "WinFiles.h"
#endif

#include "ChemBase.h"

//////////////////////////////////////////////////////////////////
double IN_DLL dH_Chem( struct CComp * pComp );
double IN_DLL dH_Chem( double Cmol[], bool IsComp[] );
double IN_DLL Norm ( bool IsComp[K_GAS], double Cmol[K_GAS] );
double IN_DLL Norm ( int K, double Cmol[K_GAS] );
double IN_DLL Norm ( CComp * pComp );
double IN_DLL Mw ( bool IsComp[K_GAS], double Cmol[K_GAS] );
double IN_DLL Mw ( CComp * pComp );
double IN_DLL Pz ( double Tz, CComp * pComp );
double IN_DLL Tz ( double Pz, CComp * pComp, double T0 = 100. );
double IN_DLL Tz ( double Pz, int ID, double T0 = 100. );
double IN_DLL Cp_IG ( double Tc, bool IsComp[K_GAS], double Cmol[K_GAS] );
double IN_DLL Cp_IG ( double Tc, CComp * pComp );
double IN_DLL H_IG ( double Tc, bool IsComp[K_GAS], double Cmol[K_GAS] );
double IN_DLL H_IG ( double Tc, CComp * pComp );
double IN_DLL H_Liq ( double Tc, CComp * pComp );
double IN_DLL H_Liq ( double Tc, bool IsComp[K_GAS], double Cmol[K_GAS] );
double IN_DLL Cp_Liq ( double Tc, bool IsComp[K_GAS], double Cmol[K_GAS] );
double IN_DLL Lam_Liq ( double Tc, bool IsComp[K_GAS], double Cmol[K_GAS] );
double IN_DLL Vis_Liq ( double Tc, bool IsComp[K_GAS], double Cmol[K_GAS] );
double IN_DLL Lam_Gas ( double P, double Tc, bool IsComp[K_GAS], double Cmol[K_GAS] );
double IN_DLL Vis_Gas ( double P, double Tc, bool IsComp[K_GAS], double Cmol[K_GAS] );

//double IN_DLL H_IG_W ( double P, double Tc, bool IsComp[K_GAS], double Cmol[K_GAS] );
//double IN_DLL H_IG_W ( double P, double Tc, CComp * pComp );
IN_DLL double To_m3_liq ( double TC, CComp * pComp );
IN_DLL double Ro_liq ( double TC, CComp * pComp ); // жидкость
IN_DLL double Ro_liq ( double TC, bool * IsComp, double * Cmol ); // жидкость
IN_DLL double Ro_mix ( double TC, CComp * pComp ); // жидкость + твердое
IN_DLL double To_m3_liq ( double TC, bool IsComp[K_GAS], double Cmol[K_GAS] );
bool IN_DLL ReadComp ( char * CompName, struct CComp * pComp );
bool IN_DLL WriteComp ( char * CompName, struct CComp * pComp );
bool IN_DLL SetCompList( int & kComp, char CompName[100][256] );
void SetS_G( int ID, double S_298 );
//////////////////////////////////////////////////////////////////

#include "Mixture.h"


struct IN_DLL CChem_W
  {
  double O;
  CChem_W();
  };

class IN_DLL CChem	: public IBaseModel, public CChem_W
  {
  public:
    bool UseNH3_H2O;
    double GammaN2;
    bool WasInit;
    CChem( char * Name );
    int Init( int );
    int Step0();
    int Step1();
    int GetParams(  char * StrName );
    int UpdateParam( struct CParams & Param );
    int SaveState ( );
    int RestoreState ( char * StrName );
    void InitCompID(  );
    };

////////////////////////////////////////////////
extern IN_DLL CGas ** pGas;
extern IN_DLL CSolid ** pSolid;
extern IN_DLL CVirtual ** pVirtual;
//////////////////////////////////////////////////////////////////
IN_DLL extern char * PrefListComp;
IN_DLL extern char * ListComp;
IN_DLL extern char NameGasComp[K_GAS][64]; // Расположены по компонентам
IN_DLL extern int ID_GasComp[K_GAS]; // ID компонент в алфавитном порядке
IN_DLL extern int N_GasComp[K_GAS];  // Порядковые номера компонент
IN_DLL extern double MwGasComp[K_GAS];
int IN_DLL FindGasComp ( char * Name );
int IN_DLL AltFindGasComp ( char * Name );
int IN_DLL GetGasID( char * Name );
int IN_DLL GetSolidID( char * Name );
int IN_DLL GetVirtID( char * Name );

struct IN_DLL CNoErrGasID
  {
  static bool NoErr;
  CNoErrGasID()
    {
    NoErr = true;
    }
  ~CNoErrGasID()
    {
    NoErr = false;
    }
  };

struct CGetComp
  {
  int nSt;
  double P, T;
  bool IsComp[K_GAS];
  double Cmol[K_GAS];
  double Henry[K_GAS];
  int Faza[K_GAS];
  };

// struct IN_DLL CСompList
//   {
// //  IBaseModel * pMain;
//   int N;
//   int ID;
// //
// //  CСompList( IBaseModel * pMain );
//   CСompList( );
//   virtual bool SetValue ( const char * Value );
//   virtual void ChangeVal( double Delta );
//   virtual void Out ( char Txt[256] );
//   virtual bool IsWrite();
//   virtual int Len();
//   virtual void * Data();
//   virtual bool SetData( int Len, const void * pData );
//   virtual operator int ( ) { return ID; }
//   virtual void operator = ( int ID );
//   };

// struct IN_DLL IChemParams
//   {
//   static IChemParams * pFirsrtChemParam;
//   IChemParams * pNextChemParam;
//   virtual void GetParams( struct IBaseModel * pMain ) = 0;
//   virtual void UpdateParam( struct CParams & Param ) = 0;
//   };

struct CLinApprCp
  {
  int kData;
  double * H0;
  double * T0;
  double * a;
  double * b;
  double T_min, T_max, Step;
  double H_norm;
  double Cp_max;
  NEW
  virtual double  C_IG ( double TC );
  virtual double  H_IG ( double TC );
  CLinApprCp( );
  bool Init( double _T_min, double _T_max, double _Step, double Cp[] );
  };

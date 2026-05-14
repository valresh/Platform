#pragma once

#ifndef LINUX
#pragma warning ( disable : 4996 )
#endif


class IN_DLL CIG_Mix
  {
  public:
  bool * IsComp;
  double * Cmol;
  double Mw;
	double T_min, T_max;
	bool Mazut_liq;
//
  CIG_Mix();
  void Set( double Cmol[K_GAS], bool IsComp[K_GAS] );
  void Set( struct CComp * pComp );
  void dH_Cp( double T_C, double & H, double & Cp, bool CalcCp );
  double dH ( double T_out, double T_in  );
  double H ( double T_C );
  double Cp ( double T_C );
//  double S( double T_C, double P );
  double TH ( double H, double T_0 = 100. );
  double M();
  double Norm();
  double Set( double Cmol[K_GAS] );
  };

struct IN_DLL CMixture
  {
  double * Cmol;//[K_GAS];
	bool   * IsComp;//[K_GAS];
  double * pTatm;
  double * Patm;//[K_GAS];
	bool NormOK;
	//
	CMixture();
	CMixture( bool NoLink );
  CMixture( double* pT, double* pP );
	void Set ( struct CComp * pComp );
	void Set ( CMixture * pMix );
//	
//	void Set ( class IN_DLL CSummFlow * pSumm );
	double & operator[] ( int N ) { return Cmol[N]; };
	double M( );
	double Ro_liq ( double T = 25. );
//
	double MolToM3 ( double FlowMol, double T );
	double MolToKG ( double FlowMol );
	double M3ToMol ( double FlowM3, double T );
	double M3ToKG ( double FlowM3, double T );
	double KGToMol ( double FlowKG );
	double KGToM3 ( double FlowKG, double T );
	void Cmass ( double Cmass[] );
//
	double H ( double T, double P, double Eps );
	void HC ( double T, double P, double Eps, double & H, double & C );
	double H ( double T, double P );
	double Hv ( double T );
	double Hl ( double T );
  double dHl ( double T1, double T2 );
  double dHv ( double T1, double T2 );
  double Risp ( double T );
	double Hv ( double T, double Y[K_GAS]  );
	double Hl ( double T, double X[K_GAS] );
	double Cv ( double T );
	double Cl ( double T );
//	double Cp_kg ( double T );
	double Pz ( double T );
  void Pz ( double T, double & P, double Y_gas[] );
	void CalcPatm( double T );
	void CalcPlim ( double T, double & Pmin, double & Pmax );
	double CalcP ( double T, double Y[K_GAS] = NULL );
	double CalcPe1 ( double T );
	void SetXY ( double T, double E, double * Xmol, double * Ymol, double Poil = 0. );
	double CalcEps ( double T, double P, double Vz_Vo, double * Pi = NULL,
		               double * Xmol = NULL, double * Ymol = NULL );
	void CalcEpsTP ( double T, double P, double & E, double * Pi = NULL,
		               double * Xmol = NULL, double * Ymol = NULL );
	double CalcPEps ( double T, double Eps, double Vz_Vo, double P0 = -1., 
										double * Pi = NULL );
	double CalcFurnTP ( double T, double P, double & E );
  CMixture & operator = ( struct CMixture & Mol );
//	double Vol( double X[] );
  bool Norm();
	void MassToMol();
	bool ReadFromDB( LPSTR Name );
	bool WriteToDB( LPSTR Name );
	void Clear();
	void Copy( CMixture * pMix );
// This += pMix * W 
	void Add ( CMixture * pMix, double W );
	void Add ( double CMol[], double W );
// This = This * W + pMix
	void Add ( double W, CMixture * pMix );
	void CalcY ( double P, double Pcomp[], double Eps,
	             CMixture * pX, CMixture * pY ); 
	double CalcPeff ( double T );
	void CalcMinMax ( double T, double & Pmin, double & Pmax, 
		                double Pcomp[] ); 
	void Clear( int nB, int nE );
	int Test();// 0 - пусто, -1 - ошибка, 1 - ОК
	bool SetComp ( struct CComp * pComp );
#undef new
  NEW
	};

struct CMixVol_W
  {
  bool IsComp[K_GAS];
  double Cmol[K_GAS];
  };

class IN_DLL CMixVol : public CMixVol_W, public IBaseModel
  {
  public:
    double M;
    CMixVol( char * Name, IBaseModel * pMain );
    void Add ( double dM, bool * IsComp, double * Cmol );
    void Add ( double dM, CComp * pComp );
    void Add ( double dM, double * Cmol );
    void Add ( int ID, double dM );
	void Get ( CComp * pComp );
    void Set ( CComp * pComp );
    void SetAir( );
    void SetWater( );
    void SetN2( );
    void Cmol_0( ){ memset ( Cmol, 0, CMOL_GAS ); };
    void IsComp_0( ){ memset ( IsComp, 0, ISCOMP_GAS ); };
    double Norm();
    double operator[] ( int N ) { return Cmol[N]; };
    double * operator & ( ) { return Cmol; };
    operator double * ( ) { return Cmol; };
    operator bool * ( ) { return IsComp; };
    void operator << ( int N ) { IsComp[N] = true; };
    void operator += ( int N ) { IsComp[N] = true; };
    void operator << ( double * From ) { memcpy ( Cmol, From, CMOL_GAS ); };
    void operator << ( bool * From ) { memcpy ( IsComp, From, ISCOMP_GAS ); };
    void operator << ( CMixVol & From ) 
      { memcpy ( IsComp, From.IsComp, ISCOMP_GAS );
    memcpy ( Cmol, From.Cmol, CMOL_GAS );};
    void operator >> ( double * To ) { memcpy ( To, Cmol, CMOL_GAS ); };
    void operator >> ( bool * To ) { memcpy ( To, IsComp, ISCOMP_GAS ); };
    void operator >> ( CMixVol & To ) 
      { memcpy ( To.IsComp, IsComp, ISCOMP_GAS );
    memcpy ( To.Cmol, Cmol, CMOL_GAS );};
    double dH_Chem( ) { return ::dH_Chem( Cmol, IsComp );	};
    double H_IG( double TC ) { return ::H_IG( TC, IsComp, Cmol );	};
    int SaveState ( );
    int RestoreState ( char * StrName );
  };


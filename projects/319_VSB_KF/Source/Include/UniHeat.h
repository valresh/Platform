#pragma once

#include "Chem.h"
#include "EpsVol.h"
#include "CommProc.h"
#include "Data.h"

#ifdef LINUX
#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif
#else
#undef IN_DLL
#ifdef DLL_UniHeat
#undef IN_DLL
#define IN_DLL __declspec(dllexport)
#else
#define IN_DLL __declspec(dllimport)
#endif
#endif


struct IN_DLL CTubeVolHeat_W
	{
// параметры
	double k_Cp_Trub, k_Cp_Vol, oTau_Trub, oTau_Vol;
///////////////////////////////////////////////////
// задавать на входе
	double KS;
// work
	double Cp_Trub, Cp_Vol;
	double T_in_Trub, T_in_Vol;
	double T_out_Trub, T_out_Vol;
//
  double T_in_Vol_Air;
	CTubeVolHeat_W();
	};

class IN_DLL CTubeVolHeat : public CTubeVolHeat_W, public IBaseModel
	{
	public:
//
	CSEps	S_Trub_In;
  CSEps	S_Vol_In_Air;
	CSEps	S_Vol_In;
	CSEps	S_Trub_Out;
	CSEps	S_Vol_Out;
//
	bool IterH_Trub;
	bool IterH_Vol;
	double kGas_Trub;
	double kGas_Vol;
	double Q;
  double Lam_Trub;
  double Lam_Vol;
	double Cp_Corp_Trub;
	double Cp_Corp_Vol;
	double kCp_Trub;
	double kCp_Vol;
  double KS_Vol_Air;
	double Ksi_Trub;
	double Ksi_Vol;
	double dT_Corp_Trub;
	double dT_Corp_Vol;
	double Q_cal, Q_dg;
	CComp * pComp_Trub;
	CComp * pComp_Vol;
  bool Static;
//
	CTubeVolHeat( char * Name, IBaseModel * pMain );
	struct CFlow * pIn_Trub;
	struct CFlow * pOut_Trub;
	struct CFlow * pIn_Vol;
	struct CFlow * pOut_Vol;
	void Calc( double dt );
// Интерфейс IBaseModel
	int GetParams( char * );
  int SaveState ( );
  int RestoreState ( char * StrName );
  int SetData( int TypeData, void * pData );
	};

struct IN_DLL C2zmHeat_W
	{
// параметры
	double k_Cp_0, k_Cp_12, Tau_0, Tau_12;
///////////////////////////////////////////////////
// задавать на входе
	double KS;
// work
	double Cp_0, Cp_1, Cp_2;
	double T_in_0, T_in_1, T_in_2;
	double T_out_0, T_out_1, T_out_2;
//
	double h_in_0, h_in_1, h_in_2;
	double h_out_0, h_out_1, h_out_2;
	C2zmHeat_W();
	};

class IN_DLL C2zmHeat : public C2zmHeat_W, public IBaseModel
	{
	public:
	CSEps	In_0;
	CSEps	In_1;
	CSEps	In_2;
	CSEps	Out_0;
	CSEps	Out_1;
	CSEps	Out_2;
	double Q_cal, Q_dg;
	double Lam_0, Lam_1, Lam_2;
	C2zmHeat( char * Name, IBaseModel * pMain );
	struct CFlow * pIn_0;
	struct CFlow * pOut_0;
	struct CFlow * pIn_1;
	struct CFlow * pOut_1;
	struct CFlow * pIn_2;
	struct CFlow * pOut_2;
	void Calc( double dt );
// Интерфейс IBaseModel
	int GetParams( char * );
  int SaveState ( );
  int RestoreState ( char * StrName );
  int SetData( int TypeData, void * pData );
	};

struct IN_DLL CAntiFlow_W
	{
// параметры
	double k_Cp_0, k_Cp_1, oTau_0, oTau_1;
///////////////////////////////////////////////////
// задавать на входе
	double KS;
// work
	double Cp_0, Cp_1;
	double T_in_0, T_in_1;
	double T_out_0, T_out_1;
//
	double h_in_0, h_in_1;
	double h_out_0, h_out_1;
//
  double T_in_0_Air, T_in_1_Air;
//
  double F_0, F_1; 
  CAntiFlow_W();
	};

class IN_DLL CAntiFlow : public CAntiFlow_W, public IBaseModel
	{
	public:
//
  CSEps	In_0_Air;
	CSEps	In_0;
	CSEps	In_1_Air;
  CSEps	In_1;
	CSEps	Out_0;
	CSEps	Out_1;
//
	bool IterH_0;
	bool IterH_1;
	double kGas_0;
	double kGas_1;
  double Lam_0;
  double Lam_1;
	double Cp_Corp_0;
	double Cp_Corp_1;
  double kCp_0;
  double kCp_1;
	double Ksi_0;
	double Ksi_1;
  double KS_0_Air;
  double KS_1_Air;
	double dT_Corp_0;
	double dT_Corp_1;
	double Q_cal, Q_dg;
	CAntiFlow( char * Name, IBaseModel * pMain );
	struct CFlow * pIn_0;
	struct CFlow * pOut_0;
	struct CFlow * pIn_1;
	struct CFlow * pOut_1;
	void Calc( double dt );
// Интерфейс IBaseModel
	int GetParams( char * );
  int SaveState ( );
  int RestoreState ( char * StrName );
  int SetData( int TypeData, void * pData );
	};

struct IN_DLL CCool_W
	{
// параметры
	double k_Cp_0, k_Cp_1, Tau_0, Tau_1;
///////////////////////////////////////////////////
// задавать на входе
	double KS;
// work
	double M_0, T_in_0, T_0, h_0, P_0;
	double T_in_1, T_out_1, Cp_1;
//
	double h_in_0, h_in_1;
	double h_out_0, h_out_1;
	double Q_Heat_0;
	CCool_W();
	};

class IN_DLL CCool : public CCool_W, public IBaseModel
	{
// 0 - охладитель
	public:
	CSEps	In_0;
	CSVol	State_0;
	CSEps	In_1;
	CSEps	Out_1;
	bool IterH_1;
	double kGas_0;
	double kGas_1;
	double M_Corp_t;
	double Q_cal, Q_dg;
	double Lam_1;
	CCool( char * Name, IBaseModel * pMain );
	struct CFlow * pIn_0;
	struct CFlow * pOut_0;
	struct CFlow * pIn_1;
	struct CFlow * pOut_1;
	void Calc( double dt );
// Интерфейс IBaseModel
	int GetParams( char * );
  int SaveState ( );
  int RestoreState ( char * StrName );
  int SetData( int TypeData, void * pData );
	};

const double M_cal = 2.38845896627496E-1;

struct IN_DLL CAntiFlowCool_W
	{
// параметры
	double k_Cp_0, k_Cp_1, oTau_0, oTau_1;
///////////////////////////////////////////////////
// задавать на входе
	double KS;
	double KCool_0, TCool_0;
	double KCool_1, TCool_1;
// work
	double Cp_0, Cp_1;
	double T_in_0, T_in_1;
	double T_out_0, T_out_1;
//
	double h_in_0, h_in_1;
	double h_out_0, h_out_1;
	CAntiFlowCool_W();
	};

class IN_DLL CAntiFlowCool : public CAntiFlowCool_W, public IBaseModel
	{
	public:
	CSEps	In_0;
	CSEps	In_1;
	CSEps	Out_0;
	CSEps	Out_1;
	bool IterH_0;
	bool IterH_1;
	double kGas_0;
	double kGas_1;
  double Lam_0;
  double Lam_1;
  double E_0;
  double E_1;
  double D_0;
  double D_1;
  double Z_0;
  double Z_1;
	double Cp_Corp_0;
	double Cp_Corp_1;
	double Ksi_0;
	double Ksi_1;
	double dT_Corp_0;
	double dT_Corp_1;
	double Q_cal, Q_dg;
	double Q_cool_0, Q_cool_1;
	double KPD_0, KPD_1;
  double T_min_0, T_min_1, T_max_0, T_max_1;
	CAntiFlowCool( char * Name, IBaseModel * pMain );
	struct CFlow * pIn_0;
	struct CFlow * pOut_0;
	struct CFlow * pIn_1;
	struct CFlow * pOut_1;
	void Calc( double dt );
// Интерфейс IBaseModel
	int GetParams( char * );
  int SaveState ( );
  int RestoreState ( char * StrName );
  int SetData( int TypeData, void * pData );
	};

struct IN_DLL CHeat_W
	{
///////////////////////////////////////////////////
//
	double Cp;
//
	CHeat_W();
	};

class IN_DLL CHeat : public CHeat_W, public IBaseModel
	{
// 0 - охладитель
	public:
	CSEps	In;
	CSEps	Out;
	double Lam;
	double Q_MWt, Q_cal,Q_dg;
// задавать на входе
	double Lev;
	double T_ext;
// параметры
	double KS;
	bool IterH;
	double k_Cp;
	double Lev_Zm;
  bool Inverse_Lev;
//
	double T_in, T_in_eff, Alfa_in_eff, T_out;
	double h_in, h_out;
  double Alfa, T_Alfa; 
// выход
	double Q_Heat;
  double Q_HeatZ;
  double k_Q_HeatZ;
  bool Use_Q_HeatZ;
  double dQ_Heat;
  double Q_Heat_F;
//
	CHeat( char * Name, IBaseModel * pMain );
	struct CFlow * pIn;
	struct CFlow * pOut;
	void Calc( double dt );
// Интерфейс IBaseModel
	int GetParams( char * );
  int SaveState ( );
  void Init( );
  int RestoreState ( char * StrName );
  int SetData( int TypeData, void * pData );
	};

struct IN_DLL CCooler_W
	{
// параметры
	double k_To_Air, T_max, Cp_Corp, KS;
	double k_Cp_Cool;
///////////////////////////////////////////////////
// work
	double Q_Heat;
	double T_corp;
	double T_in, T_out;
	double Cp_Cool;
//
	double h_in;
	double h_out;
	CCooler_W();
	};

class IN_DLL CCooler : public CCooler_W, public IBaseModel
	{
//
	public:
	CSEps	In;
	CSEps	Out;
//
	CCooler( char * Name, IBaseModel * pMain );
	struct CFlow * pIn;
	struct CFlow * pOut;
	void Calc( double dt );
	void CalcCool( double dt );
// Интерфейс IBaseModel
  int SaveState ( );
  int RestoreState ( char * StrName );
	};

struct IN_DLL CHeat_1_flow_W
  {
  ///////////////////////////////////////////////////
  // задавать на входе
  double KS;
  // work
  double T_in_Work, T_in_Const;
  double T_out_Work, T_out_Const;
  double h_Work_out;
  double h_Const_out;
  //
  CHeat_1_flow_W();
  };

struct CHeat_1_flow_Params
  {
  struct CFlow ** pIn_Work;
  struct CFlow ** pOut_Work;
  struct CFlow ** pIn_Const;
  struct CFlow ** pOut_Const;
  CSEps	* S_Work_In;
  CSEps	* S_Const_In;
  CSEps	* S_Work_Out;
  CSEps	* S_Const_Out;
  double * pKS;
  };

class IN_DLL CHeat_1_flow : public CHeat_1_flow_W, public IBaseModel
  {
  public:
    //
    CSEps	S_Work_In;
    CSEps	S_Const_In;
    CSEps	S_Work_Out;
    CSEps	S_Const_Out;
    //
    double kGas_Work;
    double kGas_Const;
    double Q;
    double Cp_Corp_Work;
    double Cp_Corp_Const;
    double kCp_Work;
    double kCp_Const;
    double Ksi_Work;
    double Ksi_Const;
    double dT_Corp_Work;
    double dT_Corp_Const;
    double Q_cal, Q_dg;
    CComp * pComp_Work;
    CComp * pComp_Const;
    bool Static;
    double h_Const_out;
    bool CalcdTdFc;
    //
    CHeat_1_flow( char * Name, IBaseModel * pMain );
    struct CFlow * pIn_Work;
    struct CFlow * pOut_Work;
    struct CFlow * pIn_Const;
    struct CFlow * pOut_Const;
    CHeat_1_flow_Params Params;
//
    void Calc( double dt );
    bool Newton( double dt );
    bool Section( double dt );
    bool BiSection( double dt );
    virtual bool Teta ( double h_Work_out, double & Teta );
    virtual bool Finc ( double h_Work_out, double & F );
    // Интерфейс IBaseModel
    int GetParams( char * );
    int SaveState ( );
    int RestoreState ( char * StrName );
    int SetData( int TypeData, void * pData );
  };

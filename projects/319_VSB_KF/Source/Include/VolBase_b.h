#pragma once

#include "BaseInt.h"
#include "DataTypes.h"
#include "SysDataTypes.h"
#include "Data.h"
#include "Chem.h"
#include "EpsVol.h"
#include "Defects.h"
#include "NewComp.h"
#include "ACS_Def.h"
//??#include "Actions.h"
#include "Extensions.h"


#ifdef LINUX
#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif
#else
#undef IN_DLL
#ifdef DLL_VolBase
#undef IN_DLL
#define IN_DLL __declspec(dllexport)
#else
#define IN_DLL __declspec(dllimport)
#endif
#endif


#define MAX_NODES 16 
struct IN_DLL CVolHeat
	{
	virtual void GetHeat( double dt, double & k_Heat, double & T_Heat, double & Eps1 ) = 0;
	virtual void SetHeat( double dt, double k_Heat, double T_Heat, double Eps1, double T, double Q_Heat ) = 0;
	};

struct IN_DLL CVolBase_W
	{
	double Cmol_Gas[K_GAS];
	double Cmol_Liq[K_GAS];
  double Cmol_1F[K_GAS];
  double Cmol_2F[K_GAS];
  double Gamma_mix[K_GAS];
	double h_Gas[K_GAS];
	double h_Liq[K_GAS];
	bool IsComp[K_GAS];
//
	double P, T, M, M_Gas, M_Liq, Lev_liq;
  double M_Liq_1, Lev_liq_1;
  double M_Liq_2, Lev_liq_2;
	double H, h, h_liq, h_gas;
	double Mw_liq, Mw_gas;
	double dMdt_mol;
	CComp CompVol;
  double M3_liq, M3_liq_1, M3_liq_2;
  double F_Summ;
  bool Is_2F;
  double Eps_1F;
  double Lev_liq_2in1;
  double Lev_liq_1in2;
  double M_Liq_1_kg, M_Liq_2_kg;
  double V_Liq_1, V_Liq_2;
  double Static_Vout, Static_Lout;
  double Static_Fix_P_Fout;
  double Static_Fix_L_Flow_1;
  double Static_Fix_L_Flow_2;
  double Vcond;
	CVolBase_W();
	};

struct IN_DLL CVolBase_P
	{
	double Max_dM;
	double D_trub;
	double M_korp_t;
	double k_dt;
	double Ro_liq;
	double Vol;
	double LevZ;
  double LevZ_2;
  double K_F1;
  double K_F2;
  double LevZabs;
  int Тип;
//  double Lev_Opt;
  double Max_Flow_Add;
	double k_M_Lev;
	double LevMin;
//
	double _P;
	double _M_liq_kg;
	double _Sliv_kg;
	double _Lev_liq;
	double kToAir;
  double k_Cond;
  bool Trace;
  double Lev_0, Lev_100;
  bool FullInitN2;
  int Залить;
  bool Объема_2;
  double Add_1F, Add_2F, Add_Comp, Add_Comp_T;
  char Add_Comp_Name[32];
  double Slow_Mgas;
//
  bool Static_Calc_Mix;
  bool Static_Fix_P;
  bool Static_Iter_P;
  double Static_P_fiz;
  int Static_Out_gas;
  bool Static_Fix_L;
  int Static_Out_Liq_1;
  int Static_Out_Liq_2;
  bool Fast_T;
  bool NoFunnel;
  double V_Cond[K_GAS];
  double Gas_Vol;
  double Liq_Vol;
  /*
  bool Use_Cmass_Gas;
  bool Use_Cmass_Liq;
  double Cmass_Gas[K_GAS];
  double Cmass_Liq[K_GAS];
  double Cproc_Gas[K_GAS];
  double Cproc_Liq[K_GAS];
  bool Use_Cmass_1F;
  bool Use_Cmass_2F;
  double Cmass_1F[K_GAS];
  double Cmass_2F[K_GAS];
  double Cproc_1F[K_GAS];
  double Cproc_2F[K_GAS];
*/
//
	CVolBase_P();
	};

  struct SummXXX
  {
    double H_summ_in_gas_A[K_GAS];
    double H_summ_in_liq_A[K_GAS];
    double H_summ_in_gas_B[K_GAS];
    double H_summ_in_liq_B[K_GAS];
    double Summ_FS_Gas[K_GAS];
    double Summ_FS_Liq[K_GAS];
    double Q;
    double Q_Cool;
    double dH_in;
    double dQ_mix;
    double dF_Gas_in;
    double dF_Liq_in;
    double dF_Gas_out;
    double dF_Liq_out_1;
    double dF_Liq_out_2;
    double F_Summ;
    double Flow_in_liq_kg;
    void Clear()
    {
      memset ( this, 0, sizeof ( *this ));
    }
  };

class IN_DLL CVolBase : public IBaseModel, public CVolBase_W, public CVolBase_P
	{
	public:
	int k_Node;
	CFlow ** pFlow;
	bool * IsFlow;
	CSEps ** State_in;
  double Eps[3];
	double * Flow_kg;
	double * Flow_Gas_mol;
	int * ID_Out;
	CComp ** pComp_Out;
	CSVol StateVol;
	double Flow_in,	Flow_out, Flow_in_liq_kg;
//////////////////////////////////////////////
	bool EmptyVol;
	bool FullVol;
	bool Destroy;
	int Defect;
	int kStep;
  double F_lost;
	double dHr;
	double Q_ext;

	CVolHeat * pVolHeat;
	int kStepTz;
	bool ЕстьЗадания;
	bool IsDelete[K_GAS];
	double F_Del[K_GAS];
	bool Delete;
	double k_Delete;
  bool Restore;
  bool PseudoWater;
  double Flow_Water_In;
  bool OverFlow;
  bool Calc_2F;
  double Eps_W_in_O;
  double Eps_O_in_W;
  int Type2F;
  double G1_G2[K_GAS];
  double G1_G2_Q[K_GAS];
  double * H_node;
  int * Vol_Conn;
  double Mw_1F, Mw_2F;
  double P_sat[K_GAS];
  double P_Pz;
  double P_Start_Iter_P;
  double Summ_Flow_kg;
  bool bp;
/////////////////////////////////////////////////
  IReactionsParam ReactionsParam;
  IReactions * pReactions;
//
#define PARM_ALL 1
#define PARM_Режим 0
#include "IO_Create.h"
#include "volbasepoints.h"
//
	CNewComp * pAddComp;
	CFlow AddFlow;
  SummXXX S;
//
  int SaveState ( );
  int RestoreState ( char * StrName );
	int GetParams( char * );
  int UpdateParam( struct CParams & Param );
//
	CVolBase( char * Name, IBaseModel * pMain );
	virtual ~CVolBase();
//
	void VolStep( double dt );
  void Dynamic( double dt );
  void Static( double dt );
  void InitData( int kIO );
	void VolInit ( CComp * pComp0, double P0, double T0 );
  void New_M_liq ( int Faza, double M_liq );
  void Compress (  );
	void AddDefects ( DWORD Root );
  double CalcLev ( double Vol_liq );
  double CalcVol ( double Lev_liq );
  void CalcHliq(  );
  void SetPress( double Press );
  double CalcFlowIn ( int nComp );
  void SetComp( CComp * pComp, double T, double Lev );
  void CalcShowComp( );
  void Calc2F( );
  void Calc2F_Comp( );
  void SdS( double Eps, double &S, double &dSdE );
  double Summa( double Eps );
  double CalcEps( double Eps0 );
  void CalcEps_0( int Conn, double H_io );
  void CalcEps_1( int Conn, double H_io );
  void CalcEps_1F( int Conn, double H_io );
  void SetVH(  );
  void Level_2Vol( double dt );
  void Level_1Vol( double dt );
  void Static_Set_Lev( double Lev );
  double Calc_dH( double T,  double H_summ_in_gas_A[], double H_summ_in_liq_A[], double H_summ_in_gas_B[], double H_summ_in_liq_B[], double R  );
  void Fast_Calc_T( double & T, double R );
  void Calc_dXY( double Fo, double Lo, double Summ_FS_Gas[K_GAS], double Summ_FS_Liq[K_GAS], double * F, double * dFdFo );
  void Calc_Mix( double & Fo, double & Lo, double Summ_FS_Gas[K_GAS], double Summ_FS_Liq[K_GAS] );
  void SummFlow( double dt );
  void DefectFlow( );
  bool Dynamic_Func_VC( double dt, double Vcond, double & Vc, double & dVcdVc, double Cmol_Gas[], double Cmol_liq[]);
  void Dynamic_Iter_VC( double dt );
  void DynamicMix_Gas( double dt );
  void DynamicMix_Liq( double dt );
  void DeleteComp( double dt );
  void DynamicCond( double dt );
  void CalcBalance( double dt );
  void CalcRoH( );
  void CalcT( double dt );
  void Set_P_Z( double dt );
  void Add( );
  int Step1();
	};

#define SET_BP_SUB \
  {\
  static C_BP T;\
  if ( pMain->ClassID == 0 || T.ID == 0 )\
  T.SetID( pMain->ClassID );\
  if ( T.Name == NULL )\
  {\
  T.Name = __FUNCTION__;\
  T.File = __FILE__;\
  T.Line = __LINE__;\
    }\
    if ( pMain->BreakPoint && T.Break ) BREAK;\
    }

double To_m3 ( double T, bool * IsComp, double * Cmol_Liq	);

#pragma once

#include "BaseInt.h"
#include "Data.h"
#include "EpsVol.h"


struct CThermo_Parm
{
    double T_in, H_in, T_out_s0, T_out;
    double Eps_in, Eps_out_s0, Eps_out;
    double P_in, P_out;
    double KPD;
    double dH;
    double H_out;
    double Gamma;
};

class CThermo_CC : public CThermo_Parm, public IBaseModel
{
    void S ( double P, double T, double &S, double &dSdT );
    void H ( double T, double &H, double &dHdT );
  public:
    bool * IsComp;
    double * Cmol;
    CComp * pComp;
    double kCool;
    double h_Cool;
    //
    CSEps In;
    CSEps Adiabat;
    CSEps Loose;
    double T_out_max;
    double Cp;
    double T_Out_Z;
    bool WorkReg_PT;
    //
    CThermo_CC( char * Name, IBaseModel * pMain );
    void Calc( double dt );
    void InitState();
    void Calc_Steam();
    int SaveState( );
    int RestoreState( char * StrName );
};

struct CStage_W
{
    double k_dP;
    double Pow;
    //
    double k_dP_Ro;
    double P_0;
    double _T_in, _T_out;
    double Mw_gas;
    double K_P2_P1;
};

struct CStage_P
{
    double Flow_Surge;
    //	static double Flow_0;
    double Flow_0;
    double k_dP_0;
    double k_V;
    double Gamma;
    double KPD;
    double KPD_M;
    double T_out_max;
    //
    double kReg_PZ;
    double P_Out_Z, dP_Z;
    //	double Pow_Ro;
    double Omega;
    //	double Pow_Omega;
    //
    double Oborot;
    double Fix_k_dP;
    bool Fix_k;
    double k_Total;
    /////////////////////////////////////
    double kReg_TZ;
    double T_Out_Z;
    double F_In_Z;
    double Flow_mol;
    double Flow_nm3;
    bool Use_P_Out_Z;
    double Min_kdP;
    double Max_kdP;
    double Reg_kdP;
    double Tin_0;
    double Mu_0;
};


class CStage_СС : public CStage_W, public CStage_P, public IBaseModel
{
  public:
    CFlow * pFlow_In;
    CFlow * pFlow_Out;
    CThermo_CC Thermo;
    double _Pow_MWt, _P_in, _P_out;
    double _Flow_kg, _Flow_m3;
    double _Zapas, _dP_lose;
    bool Trace;
    bool GetCurrData;
    //
    bool Razgerm;
    double OmegaRazgerm;
    double def_k_dP;
    double reg_k_dP;
    bool WorkReg_PT;
    //
    CStage_СС( char * Name, IBaseModel * pMain );
    void Calc ( double dt );
    void Calc_dP ( );
    int SaveState( );
    int RestoreState( char * StrName );
    void InitState();
    int GetParams( char * );
    int UpdateParam( struct CParams & Param );
    void Calc_K_P2_P1( );
    double P2_P1_0( );
};


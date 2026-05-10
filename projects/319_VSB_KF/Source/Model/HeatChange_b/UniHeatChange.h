#pragma once

#include "Data.h"
#include "EpsVol.h"

struct CUniHeatChange_W
  {
  // параметры
  ///////////////////////////////////////////////////
  // задавать на входе
  double KS;
  // work
  double T_in_Trub, T_in_Vol;
  double T_out_Trub, T_out_Vol;
  double T_Corp;
  double QQQ, dQ;
  //
  double T_in_Vol_Air;
  double h_out_Trub, h_out_Vol;
  bool First_Step;
	double Cp_Trub, Cp_Vol;
  double Q_prev;
  CUniHeatChange_W();
  };

struct CCalcParams
  {
  double dt;
  CFlow * pHot_in;
  CFlow * pCool_in;
  CFlow * pHot_out;
  CFlow * pCool_out;
  CSEps	* S_Hot_In;
  CSEps * S_Cool_In;
  CSEps	* S_Hot_Out;
  CSEps	* S_Cool_Out;
  };

class CUniHeatChange : public CUniHeatChange_W, public IBaseModel
  {
  public:
    //
    CSEps	S_Trub_In;
    CSEps	S_Vol_In;
    CSEps	S_Trub_Out;
    CSEps	S_Vol_Out;
    CSEps	S_Trub_Lim;
    CSEps	S_Vol_Lim;
    //
    double Q;
    double M_Trub;
    double M_Vol;
    double Q_MWt;
    CComp * pComp_Trub;
    CComp * pComp_Vol;
    bool Static;
    double kAntiFlow, Teta, K_Korp;
    double h_out_Trub_, h_out_Vol_;  // на предыдущем шагу
    double M_trub_mol, M_vol_mol;
    double Tau_Q;
    double _dH_Trub, _dH_Vol;
    double Eps_Q;
    double T_Trub_Z;
    double T_Vol_Z;
    double Slow_1, Slow_2;
    bool Err_Tz;
    CCalcParams Params;
    bool Расчет_через_Cp;
    bool Работа_через_Cp;
    bool Прямоток;
    bool WasLimit;
    double EpsQ;
    double EpsQfin;
    double dT_Trub;
    double dT_Vol;
    double KS_KS;
    double Q_Q;
    double dTdop;
    double kQ;
    double _dH;
    int kIterQ;
    bool KS_Large;
    //
    CUniHeatChange( char * Name, IBaseModel * pMain );
    struct CFlow * pIn_Trub;
    struct CFlow * pOut_Trub;
    struct CFlow * pIn_Vol;
    struct CFlow * pOut_Vol;
    void Calc( double dt );
    void CalcHeat( double dt );
    int GetParams( char * );
    int SaveState ( );
    int RestoreState ( char * StrName );
    int SetData( int TypeData, void * pData );
    void CalcTemp( double dt );
    void CalcTrubZ( double dt, double T_Trub_Z, CFlow * pVol_in, CFlow * pTrub_in, CFlow * pVol_out, CFlow * pTrub_out,  
                    CSEps	* S_Vol_In,  CSEps *	S_Trub_In,  CSEps	* S_Vol_Out, CSEps	* S_Trub_Out );
    void CalcVolZ( double dt, double T_Trub_Z, CFlow * pVol_in, CFlow * pTrub_in, CFlow * pVol_out, CFlow * pTrub_out,  
                    CSEps	* S_Vol_In,  CSEps *	S_Trub_In,  CSEps	* S_Vol_Out, CSEps	* S_Trub_Out );
//    void CalcTemp_Cp( double dt );
    void Calc_Q_Trub_Z( );
    void Calc_Q_Vol_Z( );
    void CalcTemp_Q( double dt );
    double Calc_Q( );
    bool Calc_QQ( double Q, double & Q_Cp );
    void CalcTrubZ_Cp( double dt );
    void CalcVolZ_Cp( double dt );
    };

bool TestTemp ( double T1_in, double T2_in, double T1_out, double T2_out );

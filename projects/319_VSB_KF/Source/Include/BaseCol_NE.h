
#pragma once
#include "BaseInt.h"
#include "Chem.h"
#include "EpsVol.h"
#include "Param.h"
#include "Data.h"
#include "math.h"
//#include "Gamma.h"
#include "UniHeat.h"
#include "Connect.h"
#include "Extensions.h"

#define OLD_PARAMS 

#undef IN_DLL
#ifdef DLL_BaseCol
#undef IN_DLL
#define IN_DLL __declspec(dllexport)
#else
#define IN_DLL __declspec(dllimport)
#endif

struct  IN_DLL CDiff
{
    double OldValue;
    double NewValue;
    double DiffValue;
    int kTest;
    double SaveValue;
    CDiff()
    {
      SaveValue = NewValue = OldValue = 12345.;
      kTest = 0;
      DiffValue = 0.;
    }
    bool Diff( double Value, double Delta = 0.02 )
    {
      if ( OldValue == 12345. )
      {
        SaveValue = NewValue = OldValue = Value;
        return false;
      }
      if ( kTest++ < 5 )
      {
        SaveValue = NewValue = OldValue = Value;
        return false;
      }
      OldValue = SaveValue;
      NewValue = Value;
      bool Stop = false;
      DiffValue = NewValue / OldValue - 1.;
      if ( fabs ( DiffValue ) > Delta )
        Stop = true;
      SaveValue = NewValue;
      return Stop;
    }
};


struct IN_DLL CShowРежим : public IShowParam
{
    int StepT;
    class CBaseCol_NE * pCol;
    void Show ( struct CParams * Param );
    void Init ( class CBaseCol_NE * _pCol ) { pCol = _pCol; };
    double Show_Dbl ( struct CParams * Param ) { return *(double*) Param->Addr; };
    CShowРежим() { pCol = NULL; StepT = -1; };
};

struct IN_DLL CExtIO
{
    CSEps	Src;
    bool Src_IsComp[K_GAS];
    double Src_CmolLiq[K_GAS];
    double Src_CmolGas[K_GAS];
    double h_Src_Liq[K_GAS];
    double h_Src_Gas[K_GAS];
    CFlow * pFlow;
    double * pPress;
    struct CKarman * pKarman;
    double dP_Hydro_Static;
    double dP_Hydro_Level;
    double Max_Gas_Flow;
    double kdH;
    int nStage;
    int nSrc;
    ///////////////////////////////
    bool Gas;
    bool SetFlowOut;
    bool StaticFlow;
    bool UseForCtrlLev;
    CComp * pComp;
    int ID_Comp;
    double Omega;
    double Eps;
    NEW
    //
    CExtIO ( char * Name, IBaseModel * pMain ) : Src ( Name, pMain )
    {
      CLEAR(Src_IsComp)
      CLEAR(Src_CmolLiq)
      CLEAR(Src_CmolGas)
      CLEAR(h_Src_Liq)
      CLEAR(h_Src_Gas)
      pFlow = NULL;
      pPress = NULL;
      dP_Hydro_Static = 0.;
      dP_Hydro_Level = 0.1;
      pComp = NULL;
      Eps = 0.;
      nStage = 0;
      //
      ID_Comp = 0;
      Gas = false;
      SetFlowOut = true;
      StaticFlow = true;
      UseForCtrlLev = false;
      pPress = NULL;
      kdH = 0.;
      pKarman = NULL;
      Max_Gas_Flow = 1000.;
    };
    void SetComp( char * Fmt, char * NameFlow )
    {
      NewComp( &ID_Comp, &pComp, Fmt, NameFlow );
    }
    void Set( CFlow * _pFlow )
    {
      pFlow = _pFlow;
      double F = pFlow->Flow_mol;
      if ( !SetFlowOut )
        return;
      if ( F < 0. )
      {
        pFlow->ID_COMP = ID_Comp;
        pFlow->pComp = pComp;
      }
      else
      {
        Src.PH( pFlow );
        Eps = Src.E;
        Src.SetCompData ( Src_IsComp, Src_CmolLiq, Src_CmolGas, h_Src_Liq, h_Src_Gas );
        pFlow->T = Src.T;
      }
    }
};

#define MAX_HEAT 3
struct IN_DLL CExtHeat_W
{
    double Cp;
};

struct IN_DLL CExtHeat : public IBaseModel, public CExtHeat_W
{
    CExtHeat * pPrevHeat;
    CExtHeat * pNextHeat;
    int nStage;
    int nGroup;
    CComp * pComp;
    CSEps	Out;
    //
    double h_in;
    double KS;
    double P;
    double Flow;
    CFlow * pFlow;
    //
    NEW
    CExtHeat ( char * Name, IBaseModel * pMain );
    bool SetIn ( CFlow * pFlow );
    bool SetOut ( CFlow * pFlow );
    bool SetCoef( double &A, double &B );
    bool CalcHeat( double T_ext );
    int SaveState( );
    int RestoreState ( char * StrName );
};
#define MAX_IN 8

typedef void (*tCalcChem) ( double dt, class CStage_NE * pStage, void * pParam );
typedef void (*tCalcChemEx) ( double dt, double T, double P, bool * IsComp, double * Cmol, void * pParam, double & k_Flow, double & dh );

struct IN_DLL CKarman_W
{
    double Level;
    double M_liq;
    double F_Karman_In;
    double F_Karman_Out;
    double T_liq;
    double To_m3;
    double To_kg;
    double P_out;
    double O1;
    double O2;
};


struct IN_DLL CKarman : public CKarman_W
{
    int nStage;
    int nNode;
    CStr Name;
    CExtIO * pSrc;
    double * pP;
    //
    double Vol;
    double Alfa_Karman;
    double HydroPress;
    double Eps_Press;
    double F_Karman_In_kg;
    double F_Karman_Out_kg;
    double dF_kg;
    double LevZ;
    //
    CKarman();
    void SetPress( CExtIO * pSrc );
    void OutLiq( CExtIO * pSrc, bool * IsComp, double * X, double * Y, double & FlowGasOut, double ** ppMixOut );
    void Sliv( CExtIO * pSrc, double dt );
    double Static_Level ( double Lout );
    double Flow_In ( double Flow_Summ, double dt );
};

struct IN_DLL CStage_Ptr
{
    class CBaseCol_NE * pCol;
    class CStage_NE * pUp;
    class CStage_NE * pDown;
    //===================================
    CExtIO	* Src[MAX_IN];
    int kSrc;
    //===================================
    bool * IsComp;
    double * Henry;
    double * Alfa_Comp;
    CHeat * pStageHeat;
    void * pCalcChemParam;
    //  tCalcChem pCalcChem;
    //  tCalcChemEx pCalcChemEx;
    //  IGamma * pGammaCommon;
    //  IGamma * pGammaAbs;
    IGamma * pColumnGamma;
    IReactions * pReactions;
    bool Razgerm_Gas;
    bool Razgerm_Liq;
    double Omega_Razgerm;
    int Numb;
    int StopSrc;
    int StopLiq;
    int StopGas;
    int StopSloj;
    int StopHeat;
    int SpecStop1;
    int SpecStop2;
    CStage_Ptr();
};

struct IN_DLL CStage_W
{
    double P, T_gas, T_liq;
    double Pz;
    double M_gas, M_liq;
    ////
    double Vin, Vout, Lin, oooLout, To_m3;
    double X[K_GAS];
    double Y[K_GAS];
    double P_z[K_GAS];
    double Vc_Comp[K_GAS];
    double V_Cond;
    //////////////////////////////////////////////////////
    double Eps;
    //////////////////////////////////////////////////////
    double T_In;
    //////////////////////////////////////////////////////
    double h_gas[K_GAS];
    double h_gas_mix;
    //////////////////////////////////////////////////////
    double Gamma[K_GAS];
    //////////////////////////////////////////////////////
    double dH_liq;
    //////////////////////////////////////////////////////
    double dH_cond;
    //////////////////////////////////////////////////////
    double Cp_gas, Cp_liq;
    //////////////////////////////////////////////////////
    double dP_Stage;
    //////////////////////////////////////////////////////
    double L_Summ_out;
    double L_Karman;
    double L_Down;
    double Flow_Liq_Stage;
    //////////////////////////////////////////////////////
    double Alfa_Level;
    CStage_W();
};

struct IN_DLL CStage_Static
{
    double P, T_gas, T_liq;
    double M_gas, M_liq;
    double F_source_gas, F_source_liq;
    double Vin, Vout, Lin, Lout;
    double Pz;
    double X[K_GAS];
    double Y[K_GAS];
    double Cond[K_GAS];
    double h_liq[K_GAS];
    double h_gas[K_GAS];
    double P_sat[K_GAS];
    double Alfa;
    /*
  double P, T_gas, T_liq;
  double Pz;
  double M_gas, M_liq;
  ////
  double Vin, Vout, Lin, Lout, To_m3; 
  double Vc_Comp[K_GAS];
  double V_Cond;
  //////////////////////////////////////////////////////
  double Eps;
  //////////////////////////////////////////////////////
  double T_In;
  //////////////////////////////////////////////////////
  double h_gas[K_GAS];
  double h_gas_mix;
  //////////////////////////////////////////////////////
  double Gamma[K_GAS];
  //////////////////////////////////////////////////////
  double dH_liq;
  //////////////////////////////////////////////////////
  double dH_cond;
  //////////////////////////////////////////////////////
  double Cp_gas, Cp_liq;
*/
    CStage_Static();
};


const int Колпачковая = 0;
const int Насадка = 1;
const int Ситчатая = 2;
const int Глухая = 3;
const int Куб = 4;

struct IN_DLL CStage_P
{
    double Alfa_Stage;
    double HenryStage;
    //	double Gamma_1_in_2;
    //	double Gamma_2_in_1;
    double Vol_Stage;
    double S_Stage;
    double Real_Vol_Stage;
    double Real_S_Stage;
    double Real_H_Stage;
    double Omega_IO;
    double h_st;
    double k_L;
    double k_LV;
    double k_GL;
    double Cp_korp;
    double Eps_Murphree;
    double Hvap_Stage;
    double k_Cool;
    double dH_liq_Z;
    double dH_gas_Z;
    double T_liq_Z;
    bool SetdH;
    double Lev_min;
    double Lev_max;
    double Doliv;
    double Sliv;
    //	double h_In;	//Высота входа в тарелку (от 0 до 1) относительно тарелки
    //	double k_Hydro_statatic; //Гидростат от уровня тарелки
    double M_min;
    bool * pCalcTom3;
    double ExtHeat;
    double Ksi_Vol;
    bool ЭтоКуб;
    double kVoutIO;
    double dP_nom;
    double V_nom;
    double k_h_cond[K_GAS];
    int StageType; // Колпачковая=0;Насадка=1;Ситчатая=2;Глухая=3;Куб=4;
    // static double SlowCond;
    //  double GammaH2S;
    //static bool ShowX;
    //static bool ShowY;
    //static bool ShowPz;
    //static bool ShowP_Y;
    //static bool ShowPzX;
    //static bool ShowVc_Comp;
    //static bool ShowX_In;
    //static bool ShowY_In;
    //
    CStage_P();
};

struct IN_DLL CStage_In
{
    double H_Summ_In;
    double Flow_Summ_In;
    double dH_liq_Summ;
    double Y_In[K_GAS];
    double X_In[K_GAS];
    //	double V_In, L_In;
    double h_Liq_In[K_GAS];
    double H_Summ_Gas_In_A[K_GAS];
    double H_Summ_Liq_In_A[K_GAS];
    double H_Summ_Gas_In_B[K_GAS];
    double H_Summ_Liq_In_B[K_GAS];
    static double Summ_In[K_GAS];
    double Summ_In_Gas[K_GAS];
    double Summ_In_Liq[K_GAS];
    double Flow_Gas_Out_Src;
    double Flow_Liq_Out_Src;
    //  Vc_Comp = Vc_Comp_C[K_GAS] - Vc_Comp_Pz[K_GAS] * Pz(T)
    double Vc_Comp_Pz[K_GAS];
    double Vc_Comp_C[K_GAS];
};

struct CNode
{
    int Numb;
    char Type;
    int nStage;
    char * Name;
    struct CObjectPoint * pOP;
    bool OutLiqCub;
    bool SkipTestType;
};

struct IN_DLL CGamma
{
    double k_PNT_0, k_PNT_1;
    double h_PNT_0, h_PNT_1;
    double T_PNT_0, T_PNT_1;
    int G_GAS, G_ABS;
    double A_Gamma, B_Gamma;
    double A_Henry, B_Henry;
    bool New;
    CGamma();
    void CalcCoef( );
    bool CalcGamma( double TC, bool IsComp[], double X[], double Gamma[]  );
};

struct IN_DLL CCond_Sloj
{
    // In
    double P, T_liq;
    // Out
    double Lout, Vout;
    double V_cond, dH_cond;
    // Params
    bool * IsComp;
    double * Alfa_Comp;
    double * pAlfa_Stage;
    CStage_NE * pDown;
    double * pEps;
    double * Henry;
    double * Gamma;
    double * Flow_L;// = X_In;
    double * Flow_V;// = Y_In;
    //
    double P_z[K_GAS];
    double X[K_GAS];
    double Y[K_GAS];
    double Ysloj[K_GAS];
    double V_Cond[K_GAS];
    double * h_cond;
    double * k_h_cond;
    int StopSloj;
    int * SpecStop1;
    int * SpecStop2;
    //
    CCond_Sloj()
    {
      memset ( this, 0, sizeof ( *this ));
    }
    void CalcCond_Sloj( );
    void Cond_XY_Sloj( double Flow_Sloj_io, double Eps, double Summ_V_in, double Flow_L[], double Flow_D[], double Flow_V[] );
    bool Cond_Press_F_dF( double S, double Eps, double Flow_L[], double Flow_V[], double & FM, double & dFdE );
};


class IN_DLL CStage_NE : public IBaseModel, 
                         public CStage_Ptr,
                         public CStage_W,
                         public CStage_P,
                         public CStage_In
{
  public:
    char Name[32];
    ////
    double Mw_Gas, Mw_Liq;
    //	Конденсация
    bool IsCond;
    double Fin, Fout;
    double Ydown[K_GAS];
    double k_hliq;
    //
    double Xnew[K_GAS];
    double M_liq_new;
    double dh_hydro;
    //
    //--	double V_Cond;
    double Eps;
    double Static_Eps;
    double P_Pz;
    double Мерфри;
    double FlowStopSlivm3;
    double kExtLout;
    //
    double K_Cond;
    double R_Cond;
    double * Yin;
    double Level;
    double M_liq_0;
    double dLdM;
    double Flow_Mix;
    double FlowGasOut;
    double V_Cond_Gas;
    //
    double h_gas[K_GAS];
    double h_liq[K_GAS];
    double h_gas_T_liq[K_GAS];
    double h_cond[K_GAS];
    double h_gas_mix;
    double h_liq_mix;
    //
    double Flow_gas_in;
    double Flow_liq_in;
    double F_gas[K_GAS];
    double F_liq[K_GAS];
    //  double ShowDistrib[K_GAS];
    //
    double Fin_Bal;
    double Fout_Bal;
    double dFdt_Bal;
    double Summ_Bal_mol;
    double M_Bal_0;
    double M_Bal_1;
    double P_Y[K_GAS];
    double P_PzX[K_GAS];
    static int HowShowParams;
    bool Trace;
    double Lev_Z;
    double dLev_dt_max;
    double Lev_Opt;
    double Max_Flow_Add;
    double StageBalance;
    double SummStageBalance;
    bool FullStage;
    bool NoCond;
    double _P;
    double GasOutFromStage;
    //CStage_Static Static;
    //
    CCond_Sloj Sloj0;
    //
    CStage_NE( char * Name, IBaseModel * pMain );
    NEW
      //
      void Init();
    int GetParams( char * );
    int UpdateParam( struct CParams & Param );
    void VisualParams( char * );
    int ShowParams( DWORD DataType, struct CShowData * pSD );
    int SaveState( );
    int RestoreState ( char * StrName );
    void Add ( CExtIO * Src );
    //////////////////////////////////////////////////////
    // Температуры тарелок
    void CalcHeat( double dt );
    void CalcH( );
    double F_T_gas( double Tm, double dT );
    double F_T_liq( double Tm, double dT, bool AddR = true );
    void Calc_T( double & Tm );
    double Calc_F( double Tm, double dT );
    // Вывод
    void CalcLiq( double dt );
    void CalcGas( double dt );
    //
    // Общие
    void Model_Cond( double dt );
    void CalcLevel( double dt );
    void Calc_dP( double dt );
    void Summ_Flow_In( double dt );
    // Конденсация в объеме
    void CalcCond_Static_Vol( );
    void CalcCond_Static_Vol_Func( double V_out, double L_out, double * Flow_V, double * Flow_L, double * pSumm, double * pdSumm_dV );
    void CalcCond_Static_Vol_XY( double V_out, double L_out, double * Flow_V, double * Flow_L );
    void CalcCond_Dynamic_Vol( double dt );
    void CalcCond_Dynamic_Vol_Func( struct Dynamic_Vol_Func_Params * P );
    void CalcCond_Dynamic_Vol_XY( struct Dynamic_Vol_Func_Params * P );

    void CalcCond_Static( double dt );
    void SetLevel_Static( double dt );
    void Cond_XY_Static( double M, double Eps, double _L[], double _V[] );
    bool Cond_Static_F_dF( double M, double Eps, double _L[], double _V[], double & F, double & dFdE );
    // Конденсация в слое
    double CalcStaticHeat( double T );
    void CalcCond_Sloj( );
    void Cond_XY_Sloj( double Flow_Sloj_io, double Eps, double Summ_V_in, double Flow_L[], double Flow_D[], double Flow_V[] );
    //
    //--  void CalcCond_Alfa( double dt );
    //--  bool Cond_F_dF_Alfa( double dt, double M, double L, double V, double _L[], double _V[], double & FM, double & dFdL );
    //--  void Cond_XY_Alfa( double dt, double M, double L, double V, double _L[], double _V[] );
    //
    void CompressXY( bool Used[K_GAS] );
    void CorrData( );
    void Static_Press( );
    void CalcCond_Press( );
    //  void CalcCond_Sloj( double F_in, double Y_Gas[], double T,
    //    double & F_out, double & Rcond_summ, double & Vcond_summ, double Y_out[], double V_cond[] );
    double CalcCond_Y( double F, double * A, double * B, double * Y, double * Y_in );
    bool Cond_Press_F_dF( double M, double Eps, double _L[], double _V[], double & F, double & dFdE );
    void Cond_XY_Press( double M, double Eps, double _L[], double _V[] );
    void IterStaticHeat( );
    double SummFlow();
};


struct CBaseCol_W
{
    double P_top;
    bool IsComp[K_GAS];
    double Omega_Top;
    CBaseCol_W();
};

#define MAX_HENRY 10
//enum eAdsType { AdsNone, AdsH2S_MEA, AdsCO2_MEA, AdsCO2_K2CO3 };
struct CBaseCol_P
{
    //  double T_min;
    //  double T_max;
    bool CalcStageBalance;
    int n_Comp_Balance;
    int n_Comp_Balance_1;
    int n_Comp_Delete;
    double h_cub;
    bool UseHydrostat;
    double Henry[K_GAS];
    double Alfa_Comp[K_GAS];
    double ClearComp[K_GAS];
    bool MustClear;
    //  eAdsType AdsType;
    //  bool UseCO2Appr;
    //  bool CalcAdsDes;
    //  double Omega_Def;
    //  double P_min;
    //  double P_max;
    //  double kInertHeat;
    double P_Top_Z;
    bool Fast_T;
    double Slow_Fast_T;
    bool WriteBalance;
    bool Static_Fix_L;
    bool Static_Iter_T;
    int ModelCond;
    bool Cond_Level;
    CBaseCol_P();
};

#define PROB_LOW_LEV 0x0001
#define PROB_HI_LEV  0x0002
#define PROB_FIXED_P 0x0004
#define PROB_MIN_P   0x0008
#define PROB_MAX_P   0x0010
#define PROB_HI_DM   0x0020
#define PROB_FIX_T   0x0040
#define PROB_FIX_H   0x0080



class IN_DLL CBaseCol_NE	: public IBaseModel, public CBaseCol_W, public CBaseCol_P
{
  public:
    int Model_Gas_Liq;
    int nCub;
    CStage_NE ** pStage;
    bool * pCommonStage;
    bool Static_Calc_Mix;
    double Comp_In[K_GAS];
    double Comp_Out[K_GAS];
    double Comp_Balance[K_GAS];
    double Flow_In_mol;
    double Flow_Out_mol;
    double Flow_Balance_mol;
    double * Flow_Stage_In_mol;
    double * Flow_Stage_Out_mol;
    double * Flow_Stage_Balance_mol;
    double * Flow_Stage_Balance_kg;
    double * Heat_Stage_In_mol;
    double * Heat_Stage_Out_mol;
    double * Heat_Stage_Balance_mol;
    double Flow_In_kg;
    double Flow_Out_kg;
    double Flow_Balance_kg;
    double H_In;
    double H_Out;
    double H_Balance;
    double * Режим;
    bool Разгерметизация_Верха;
    int Проблемы;
    int ЕстьПроблемы;
#define PROF
#define LONG
#define EPROF
#define ELONG
#define NO_DEFECT
#include "IO_Create.h"
#include "BaseCol_NE_Pnt.h"
#undef NO_DEFECT
    int kStep;
    bool Work;
    bool Razgerm;
    double OmegaRazgerm;
    CExtHeat * pHeat_In[MAX_HEAT];
    CExtHeat * pHeat_Out[MAX_HEAT];
    int Режим_по_тарелкам;
    CShowРежим ShowРежим;
    //  CStage_NE * pParamsStage;
    int ShowVariantParams;
    double VariantParams[K_GAS];
    int n_Comp_Balance;
    double dT_abs_med;
    double dT_abs_max;
    IGamma * pGammaCommon;
    IGamma * pGammaAbs;
    IGamma * pColumnGamma;
    IReactions * pReactions;
    int NumbShow;
    enum eACS_Flag { eACS_No, eACS_Pabs, eACS_P, eACS_D, eACS_T, eACS_L };
    int MesPress;
    int OldShowVariantParams;
    double * Calc_C;
    double k_h_air;
    double k_l_air;
    //??  I_ColAdditionList * pColAdditions;
    /////////////////////////////////////////////////
    NEW
    CBaseCol_NE( char * Name, IBaseModel * pMain );
    int GetParams( char * );
    int SaveState ( );
    int RestoreState ( char * StrName );
    CExtern_Pnt * GetPoint( const char * PntName );
    void SetPoints( );
    void InitCol( int kStage );
    void InitState( );
    void Utils_1( );
    //	void CalcH2SCoef( );
    void Step( double dt );
    void CreateHeat ( int nFirstSt, int nLastSt, bool UpToDown );
    void SetHeatFlow ( int nFirstSt, int nLastSt, bool UpToDown, CFlow * pIn, CFlow * pOut );
    void SetHeatQ ( int nFirstSt, int nLastSt, bool Use_Q_HeatZ, double Q_Z, double & k_Q_HeatZ, double KS, double & dQ );
    int ShowParams( DWORD DataType, struct CShowData * pSD );
    int UpdateParam( CParams & Param );
    int SetData( int TypeData, void * pData );
    void DrawState ( CDrawObjData * pDraw );
    void DrawFixed ( CDrawObjData * pDraw );
    void SetРежим ( bool Show );
    void SetVariantParams();
    double Show_Press ( double Pabs );
};


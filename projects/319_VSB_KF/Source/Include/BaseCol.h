#pragma once
#include "BaseInt.h"
#include "Chem.h"
#include "EpsVol.h"
#include "Param.h"
#include "Data.h"
#include "math.h"
//#include "Gamma.h"
#include "UniHeat.h"


#define OLD_PARAMS 

#undef IN_DLL
#ifdef DLL_BaseCol
#undef IN_DLL
#define IN_DLL __declspec(dllexport)
#else
#define IN_DLL __declspec(dllimport)
#endif


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
    double Lev_Feed; // Уровень вывода относительно тарелки
    double kdH;
    ///////////////////////////////
    bool Gas;
    bool SetFlowOut;
    CComp * pComp;
    int ID_Comp;
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
        Lev_Feed = 2.;
        pComp = NULL;
        //
        ID_Comp = 0;
        Gas = false;
        SetFlowOut = true;
        pPress = NULL;
        kdH = 0.;
    };
    //void Set ( double P_src, double P )
    //	{
    //   *pPress = P;
    //	CComp * pComp = pFlow -> Get_Comp();
    //	if ( P_src > 0. )
    //		Src.PH ( P_src, pFlow -> h_Flow, pComp );
    //	else
    //		{
    //		if ( P_src < 0. )
    //			Src.PH ( P, pFlow -> h_Flow, pComp );
    //		else
    //			Src.PH ( pFlow );
    //		}
    //	FINITE(Src.E)
    //	Src.SetCompData ( Src_IsComp, Src_CmolLiq, Src_CmolGas,
    //									  h_Src_Liq, h_Src_Gas );
    //	}
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

/////////////////////////////////////////////////////////////
struct CEpsEps_P
{
    double T_old;
    double Step_T;
    double HenryPrim;
    double E, h_gas, h_liq, h;
    double P;
    bool WaterCol;
    bool NH3Col;
    bool Azeotrope;
    double GammaPz;
    //	Тепло
    double A_T_ext_heat;  // Q_ext =  A_T_ext_heat * T - B_T_ext_heat;
    double B_T_ext_heat;
    double T_ext_heat;
    /////////////////////////////////////////////////////////////
    static double Q_W[K_GAS];
    static double K_W[K_GAS];
    static double X_W[K_GAS];
    static double Y_W[K_GAS];
    static double Z_W[K_GAS];
    static double h_Liq[K_GAS];
    static double h_Gas[K_GAS];
    static double P_sat[K_GAS];
    /////////////////////////////////////////////////////////////
    bool * IsComp;
    double * Henry;
    double * Gamma;
    double * HenryStage;
    //	double * pT_min;
    //	double * pT_max;
    double * T;
    double * Pz_H2S;
    double * dH_liq;
    //double T_min;
    //double T_max;
    /////////////////////////////////////////////////////////////
    CEpsEps_P();
};

class IN_DLL CEpsEps	: public CEpsEps_P
{
    double CalcEps_PT( double P, double T, double Eps0 );
    void Set_XY( );
    void Calc_H( );
    double Summ( double Eps );
    void SdS( double Eps, double  & S, double &dSdE );
    void H( );
    void Pz( double T );
    void H_liq ( );
    void H_gas ( );
    void SetComp ( double Cmol[] );
    void SetGamma();
public:
    I_Addition * pColAdditions;
    class CBaseCol * pCol;
    class CStage * pStage;
    //
public:
    NEW
    CEpsEps( );
    void PH( double P, double H, double Cmol[], double T0 = 0. );
    int GetParams( IBaseModel * pMain );
};
////////////////////////////////////////////////////////////////////////////////////////

#define MAX_IN 8

typedef void (*tCalcChem) ( double dt, class CStage * pStage, void * pParam );
typedef void (*tCalcChemEx) ( double dt, double T, double P, bool * IsComp, double * Cmol, void * pParam, double & k_Flow, double & dh );

struct IN_DLL CStage_Ptr
{
    class CStage * pUp;
    class CStage * pDown;
    //===================================
    CExtIO	* Src[MAX_IN];
    int kSrc;
    //===================================
    bool * IsComp;
    double * Henry;
    double * Alfa_Z;
    class CBaseCol * pCol;
    double * Alfa_Comp;
    //  double * A_H2S;
    //  double * B_H2S;
    //  CLevel * pRegLev;
    CHeat * pStageHeat;
    void * pCalcChemParam;
    tCalcChem pCalcChem;
    tCalcChemEx pCalcChemEx;
    bool Razgerm_Gas;
    bool Razgerm_Liq;
    double Omega_Razgerm;
    int Numb;
    int StopSrc;
    int StopLiq;
    int StopGas;
    int StopFlow;
    int StopHeat;
    CStage_Ptr();
};

struct IN_DLL CStage_W
{
    double P, T_gas, T_liq;
    double Pz;
    double M_gas, M_liq;
    ////
    double Vin, Vout, Lin, Lout, To_m3;
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
    CStage_W();
};

const int Колпачковая = 0;
const int Насадка = 1;
const int Ситчатая = 2;
const int Глухая = 3;
const int Куб = 4;

struct IN_DLL CStage_P
{
    double P_src;
    double Alfa_Stage;
    double HenryStage;
    double Gamma_1_in_2;
    double Gamma_2_in_1;
    double Vol_Stage;
    double S_Stage;
    double Real_Vol_Stage;
    double Real_S_Stage;
    double Real_H_Stage;
    double Omega;
    double dP_nom;
    double V_nom;
    double Omega_IO;
    double h_st;
    double k_L;
    double k_LV;
    double k_GL;
    double k_Eps;
    double Cp_gas, Cp_liq, Cp_korp;
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
    bool UseGydroIndP;
    double dP_Hydro_Static;
    double dP_Hydro_Level;
    double h_In;	//Высота входа в тарелку (от 0 до 1) относительно тарелки
    double k_Hydro_statatic; //Гидростат от уровня тарелки
    double M_min;
    bool CalcTom3;
    double Tau_SS;
    double H_SS;
    double Eff;
    double ExtHeat;
    double kExtLout;
    double FlowStopSlivm3;
    double Ksi_R_Rel;
    double Ksi_Vol;
    int nHenry;
    bool ЭтоКуб;
    double kVoutIO;
    int StageType; // Колпачковая;Насадка;Ситчатая;
    //  double GammaH2S;
    static bool ShowX;
    static bool ShowY;
    static bool ShowPz;
    static bool ShowP_Y;
    static bool ShowPzX;
    static bool ShowVc_Comp;
    static bool ShowX_In;
    static bool ShowY_In;
    //
    CStage_P();
};

struct IN_DLL CStage_In
{
    double H_Summ_In;
    double Flow_Summ_In;
    double dH_liq_Summ;
    static double Summ_In[K_GAS];
    double Y_In[K_GAS];
    double X_In[K_GAS];
    double V_In, L_In;
    double h_Liq_In[K_GAS];
    CEpsEps Eps_In;
};

struct CNode
{
    int Numb;
    char Type;
    int nStage;
    char * Name;
    struct CObjectPoint * pOP;
    bool OutLiqCub;
    bool OutGasTop;
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

class IN_DLL CStage : public IBaseModel, 
                      public CStage_Ptr,
                      public CStage_W,
                      public CStage_P,
                      public CStage_In
{
public:
    char Name[32];
    ////
    ////
    double dT_liq;
    double dT_gas;
    double dT_liq_dH;
    double dT_gas_dH;
    double Mw_Gas, Mw_Liq;
    double dh_hydro;
    // Конденсация в слое
    double P_comp[K_GAS];
    //	Конденсация
    bool IsCond;
    double Fin, Fout;
    double Ydown[K_GAS];
    //--	double Vc_Comp[K_GAS];
    double k_hliq;
    //
    double Xnew[K_GAS];
    double M_liq_new;
    //
    //--	double V_Cond;
    double Eps;
    double P_Pz;
    double Pz_H2S;
    //
    double K_Cond;
    double * Yin;
    double Level;
    double M_liq_0;
    double dLdM;
    double Flow_Mix;
    double FlowGasOut;
    double V_Cond_Gas;
    //
    //	double h_gas[K_GAS];
    double h_liq[K_GAS];
    double h_gas_T_liq[K_GAS];
    //	double h_gas_mix;
    double h_liq_mix;
    //
    double Flow_gas_in;
    double Flow_liq_in;
    double F_gas[K_GAS];
    double F_liq[K_GAS];
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
    double Lev_old;
    double Max_Flow_Add;
    int nStageBalance;
    double StageBalance;
    double ConstStageBalance;
    char * NameStageBalance;
    bool CalcStageBalance;
    double SummStageBalance;
    bool FullStage;
    //
    CStage( char * Name, IBaseModel * pMain );
    NEW
        void CalcLiq( double dt );
    void CalcHeat_Eps( double dt );
    void CalcH_Eps( );
    void CalcGas( double dt );
    int GetParams( char * );
    void VisualParams( char * );
    int ShowParams( DWORD DataType, struct CShowData * pSD );
    int SaveState( );
    int RestoreState ( char * StrName );
    void Add ( CExtIO * Src );
    void FeedFlash( double dt );
    void CompressXY( bool Used[K_GAS] );
    void CorrData( );
};

struct IN_DLL CVol_P
{
    double Accel_Vol;
    double AlfaCond;
    double AlfaPz;
    CVol_P();
};

struct IN_DLL CVol_W
{
    bool Work;
    bool Overflow;
    int nStartVol;
    double Napor;
    double _O1;
    double _O2;
    double Ro;
    double h_sloj;
    double Lev_Cub;
    //
    CVol_W();
};


struct IN_DLL CVol : public CStage, public CVol_P, public CVol_W
{
    CSEps StateSrc;
    CComp * pComp;
    int ID_Comp;
    double dMdt_p, dMdt_m;
    double Disbalance;
    double _Flow_Summ_In;
    ///
    CVol( char * Name, IBaseModel * pMain );
    void InitVol ( );
    void InitCopy ( int nStart );
    int GetParams( char * );
    int SaveState( );
    int RestoreState ( char * StrName );
    void FeedFlash( double dt );
    void CalcLiq( double dt );
    void CalcGas( double dt );
    bool FullCub( );
    NEW
};

struct CBaseCol_W
{
    double P_top;
    double Flow_out;
    double Flow_1;
    double Henry[K_GAS];
    bool IsComp[K_GAS];
    double Alfa_Comp[K_GAS];
    double Omega_Top;
    double oA_H2S;
    double oB_H2S;
    CBaseCol_W();
};

#define MAX_HENRY 10
enum eAdsType { AdsNone, AdsH2S_MEA, AdsCO2_MEA, AdsCO2_K2CO3 };
struct CBaseCol_P
{
    //  double T_min;
    //  double T_max;
    bool Use_min_max;
    bool Calc_Flow_T;
    int n_Comp_Balance;
    int n_Comp_Delete;
    double h_cub;
    double k_P_stage;
    double P_Top_Z;
    bool UseHydrostat;
    bool CanOverflow;
    bool Fast_Q_SS;
#ifndef OLD_PARAMS 
    double Henry[K_GAS];
    double Alfa_Comp[K_GAS];
#endif 
    bool ClearComp[K_GAS];
    bool MustClear;
    int TypeCol;
    double dt_Vol;
    //double T_H2S_0;
    //double k_H2S_0;
    //double T_H2S_1;
    //double k_H2S_1;
    eAdsType AdsType;
    bool UseCO2Appr;
    //  double HenryH2S;
    bool CalcAdsDes;
    double Omega_Def;
    double P_min;
    double P_max;
    double Ksi_R;
    double Eps_Gas;
    double kInertHeat;
    double kInertVout;
    double QuickGas;
    double Flow_src_min;
    double Tau_cool, Tau_sliv;
    double Tau_gas;
    bool CalckVout;
    bool QuickX;
    bool ShowModelTAB;
    int SetPsrc;
    double kSrcPlus;
    double kSrcMin;
    bool WriteBalance;
    char Comp_Balance_Name[64];
    //  double Henry_Stage[MAX_HENRY][K_GAS];
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


//#define TAB_USER(Name) TAB(Name,LEV_USER) 
//#define TAB(Name) TAB(Name,LEV_INSTR) 
//#define TAB_SUPER(Name) TAB(Name,LEV_SUPER) 
//#define TAB_HIDE(Name) TAB(Name,LEV_HIDE) 



class IN_DLL CBaseCol	: public IBaseModel, public CBaseCol_W, public CBaseCol_P
{
public:
    int nCub;
    CStage ** pStage;
    double Comp_In[K_GAS];
    double Comp_Out[K_GAS];
    double Comp_Balance[K_GAS];
    int CtrlComp[K_GAS];
    double Flow_In_mol;
    double Flow_Out_mol;
    double Flow_Balance_mol;
    double Flow_In_kg;
    double Flow_Out_kg;
    double Flow_Balance_kg;
    double H_In;
    double H_Out;
    double H_Balance;
    bool Разгерметизация_Верха;
    int Проблемы;
    int ЕстьПроблемы;
#define NO_DEFECT
#define PROF
#define LONG
#define EPROF
#define ELONG
#include "IO_Create.h"
#include "BaseCol_Pnt.h"
#undef NO_DEFECT
#undef PROF
#undef LONG
#undef EPROF
#undef ELONG
    I_AdditionList * pColAdditions;
    //	CGamma Gamma;
    int kStep;
    bool Work;
    bool OutPureGas;
    bool OutPureLiq;
    //??//  CArrCount Timer;
    //  CTraceTime TT;
    CFlow * pOut_Gas;
    bool GasColumn;
    CVol Vol;
    bool Razgerm;
    double OmegaRazgerm;
    CExtHeat * pHeat_In[MAX_HEAT];
    CExtHeat * pHeat_Out[MAX_HEAT];
    CGamma * pAdsGas_1;
    double H_ads[K_GAS];
    //  CGamma * pAdsGas_2;
    bool LongList;
    bool ShowProf;
    /////////////////////////////////////////////////
    NEW
    CBaseCol( char * Name, IBaseModel * pMain );
    int GetParams( char * );
    int SaveState ( );
    int RestoreState ( char * StrName );
    CExtern_Pnt * GetPoint( const char * PntName );
    void SetPoints( );
    void InitCol( int kStage, bool WaterCol = false, bool NH3Col = false, bool Azeotrope = false );
    void InitWaterCol();
    void InitState( );
    //	void CalcH2SCoef( );
    void Step( double dt );
    void CalcPress( double dt );
    void CalcVolPress( double dt );
    void CreateHeat ( int nFirstSt, int nLastSt, bool UpToDown );
    void SetHeatFlow ( int nFirstSt, int nLastSt, bool UpToDown, CFlow * pIn, CFlow * pOut );
    void SetHeatQ ( int nFirstSt, int nLastSt, bool Use_Q_HeatZ, double Q_Z, double & k_Q_HeatZ, double KS, double & dQ );
    int ShowParams( DWORD DataType, struct CShowData * pSD );
    int UpdateParam( CParams & Param );
    int SetData( int TypeData, void * pData );
    void DrawState ( CDrawObjData * pDraw );
    void DrawFixed ( CDrawObjData * pDraw );
};




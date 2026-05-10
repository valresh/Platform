#pragma once

#include "BaseInt.h"
#include "DataTypes.h"
#include "SysDataTypes.h"
//#include "SR.h"
#include "EpsVol.h"
#include "Data.h"
#include "Chem.h"
#include "Defects.h"
#include "ACS_Def.h"
//#include "heatoil.h"
#include "UniHeat.h"

struct CThermo_Parm
{
    double Gamma;
    double T_in, H_in, T_out_s0, T_out;
    double Eps_in, Eps_out_s0, Eps_out;
    double P_in, P_out;
    double KPD;
    double dH;
    double H_out;
    double T_out_max;
    double T_out_min;
    //  double aGamma[K_GAS];
};

class CThermo_PC : public CThermo_Parm, public IBaseModel
{
    void H ( double T, double &H, double &dHdT );
  public:
    bool * IsComp;
    double * Cmol;
    CComp * pComp;
    double kCool;
    double h_Cool;
    //
    CSEps In;
    //
    //	  bool bGamma;
    CThermo_PC( char * Name, IBaseModel * pMain );
    void Calc();
    void Calc_Steam();
    int SaveState( );
    int RestoreState ( char * StrName );
    void S ( double P, double T, double &S, double &dSdT );
};

struct CStage_W
{
    double Oborot;
    double Flow_Gas;
    double Pow;
};

struct CStage_P
{
    double Flow_Nom_m3;
    //  double kFlow_Nom;
    double P_Nom;
    double Eps;
    double KPD, KPD_mech;
    //
    double dP_in;
    double dP_out;
    double PowZ;
    //
    double K_heat;
    double S_heat;
    double k_Flow;// "Сглаживание потока" );
};


class CStage_PC : public CStage_W, public CStage_P, public IBaseModel
{
  public:
    CFlow * pFlow_In;
    CFlow * pFlow_Out;
    class CPC_1 * pCompr;
    CThermo_PC Thermo;
    double _T_in, _T_out;
    double Pow, _Pow_MWt, _P_in, _P_out;
    double _Flow_kg, _Flow_m3;
    double _Flow_lose;
    bool Trace;
    //
    bool Razgerm;
    double OmegaRazgerm;
    CTubeVolHeat Рубашка;
    CFlow FlowM;
    //    double k_Cool;
    //
    CStage_PC( char * Name, IBaseModel * pMain );
    void Calc ( double dt );
    int SaveState( );
    int RestoreState ( char * StrName );
};

struct _W
{
    double Oborot;
    bool On;
    double P_Air;
    int State;
    _W();
};

class CPC_1 : public IBaseModel, public IUniModel, public _W
{
    //
  public:
    CPC_1( char * ObjName );
    virtual ~CPC_1();
    // Интерфейс IBaseModel
    int Init( int Regim );
    int Step0( );
    int Step1( );
    int StepT(double dt);
    int GetParams( char * );
    int ShowParams( DWORD DataType, struct CShowData * pSD );
    int UpdateParam( struct CParams & Param );
    int SaveState ( );
    int RestoreState ( char * StrName );
    int SetData( int TypeData, void * pData );
    int OnDefect( CDef * pDefect );
    ///////////////////////////////////////////////////
    //  int Work( double dt );
    //  int TestCtrlPoints( int kNodes, struct CObjectPoint ** _ppNodes );
    //  int PutAddr( char * Point, void * pData, char * Name );
    int DstMM( char * DataName, struct IMM * pMM );// Вызов получателя сигнала
    int SrcMM( char * DataName, struct IMM * pMM );
    ///////////////////////////////////////////////////
    virtual int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
    virtual int SetMatr( void * pExternals, int PntGroup,  double dt,
                        double A[/* kExkV */], double B[ /*kE*/ ] );   // A * X = B
    virtual int SetVar( void * pExternals, int PntGroup, double dt,
                       // Переменные на входах
                       double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
    void DrawObj ( struct CDrawObjData * pDraw );
    NEW
    /////////////////////////////////////////////////
    //#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "PC_1_Defects.h"
#include "ACS_Create.h"
#include "ACS.h"
      /////////////////////////////////////////////////
      CStage_PC ST_1;
    //  CPumpData * pPump;
    bool RazGerm;
    double OmegaRazGerm;
    double FlowQ;
    CMotorData* pMotor;
    bool CanPuskStop;
    bool DefectStop;
    bool DefectPow;
    double RelDefectPow;
    bool UseCtrl;
    //
    void Calc ( double dt );
    void Logica ( double dt );
    typedef std::vector<CDef*> TDefVector;
    TDefVector defVector;
    CMotorData MotorData;
};

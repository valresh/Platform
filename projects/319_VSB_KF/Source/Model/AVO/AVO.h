#pragma once

#include "BaseModel.h"
#include "DataTypes.h"
//#include "SysDataTypes.h"
//#include "SR.h"
//#include "GydroData.h"
//#include "Data.h"
#include "CommProc.h"
#include "Defects.h"
#include "ACS_Def.h"
#include "UniHeat.h"
#include "IntReg.h"
#include "Chem.h"

#define MAX_MOTOR 8

struct _W
  {
  bool On;
  double T_in, T_out;
  double T_in_Air, T_out_Air;
  double Cp, Cp_Air;
  double kOmega;
  double Jalousie_Eff;
  double Jalousie_1;
  double Jalousie_2;
  double P_cond;
  double Eps_cond;
  double Jalousie_3;
  double Jalousie_4;
  _W();
  };

class CAVO : public IBaseModel, public IUniModel, public _W
	{
//
public:
	CAVO( char * ObjName );
	virtual ~CAVO();
// Интерфейс IUniModel
	int Init( int );
	int Step0( );
	int Step1( );
	int StepT(double dt);
	int GetParams(  char * StrName );
	int ShowParams( DWORD DataType, struct CShowData * pSD );
  int UpdateParam( struct CParams & Param );
  int SaveState ( );
  int RestoreState ( char * StrName );
  int SetData( int TypeData, void * pData );
  int OnDefect( CDef * pDefect );
  void DrawObj ( struct CDrawObjData * pDraw );
  virtual void AddResources (CGDIResourceMgr *pResMgr);
  virtual void DrawObject ( struct CDrawObjData * pDraw, CGDIResourceMgr *pResMgr);
  int DstMM( char * DataName, struct IMM * pMM );// Вызов получателя сигнала
///////////////////////////////////////////////////
  int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );
  int SetVar( void * pExternals, int PntGroup, double dt, 
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
  virtual bool GetParamsList( struct CParams ** pParams, int * kParams );
  static CParams * pParams;
  static int kParams;
  NEW
/////////////////////////////////////////////////
/////////////////////////////////////////////////
  CFlow * pIn_Water;
  CFlow * pOut_Water;
  CFlow * pIn_Water_2;
  CFlow * pIn_Steam;
  CFlow * pOut_Steam;
/////////////////////////////////////////////////
  CTubeVolHeat Heat;
  CTubeVolHeat HeatAir;
  CHeat_1_flow Heat1flow;
  CSEps Cond;
/////////////////////////////////////////////////
  CFlow In_Air;
  CFlow Heat_Air;
  CFlow Out_Air;
/////////////////////////////////////////////////
//  CStarter Пускатель;
  bool Heat_Defect;
  bool Angle_Defect;
  bool Trub_Defect;
  double k_Heat;
  double Def_Omega_Trub;
  double Def_Angle;
  double FlowQ;
  bool Static;
  int Ctrl_Jal_1;
  int Ctrl_Jal_2;
  int Ctrl_Jal_3;
  int Ctrl_Jal_4;
  double * pJal[4];
  CMotorData* pMotors[MAX_MOTOR];
  int kMotor;
 /////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "AVO_Defects.h"
#include "ACS_Create.h"
#include "ACS.h"
  char pSrc[MAX_MOTOR][64];
//
  void Calc ( double dt );
  void CalcCond( );
  void DrawFixed ( CDrawObjData * pDraw );
  double CalcCondPath( );
  void Set_Jalousie( double dt );
  bool Set_One_Jalousie( double dt, int & Ctrl_Jal,
                         double & Jalousie,
                         CAnalog & Жалюзи, CDiscrete & Жалюзи_открыты, CDiscrete & Жалюзи_закрыты,
                         CDiscrete & Закрыть_жалюзи, CDiscrete & Открыть_жалюзи  );
	};

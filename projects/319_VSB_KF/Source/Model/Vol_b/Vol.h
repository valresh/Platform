#pragma once

#include "BaseInt.h"
#include "Data.h"
#include "VolBase_b.h"
#include "ACS_Def.h"
#include "UniHeat.h"
#include "IntReg.h"
#include "Control.h"
#include "Extensions.h"
#include "HydroStruct.h"

#ifdef LINUX
#define CParams QParams
#endif

struct ICreateVolExtensions : IExtension
  {
  bool ReadData( IBaseModel * pModel );
  };


struct CShowVolComp : public IShowParam
  {
  class CVol * pVol;
  void Init ( class CVol * pVol );
  void Show ( struct CParams * Param );
  double Show_Dbl ( struct CParams * Param ) { return *(double*) Param->Addr; };
  CShowVolComp();
  };

struct CShowVolFlow : public IShowParam
  {
  class CVol * pVol;
  void Init ( class CVol * pVol );
  void Show ( struct CParams * Param );
  double Show_Dbl ( struct CParams * Param ) { return *(double*) Param->Addr; };
  CShowVolFlow();
  };

struct CShow2FComp : public IShowParam
  {
  class CVol * pVol;
  void Init ( class CVol * pVol );
  void Show ( struct CParams * Param );
  double Show_Dbl ( struct CParams * Param ) { return *(double*) Param->Addr; };
  CShow2FComp();
  };

#define MAX_N 64 
struct _W
  {
  double P_Napor;
  bool WorkHeat; 
  double Уровень_псевдоводы;
  double Flow_summ_out;// Поток для nFlow_summ
  bool OverFlow;
  double Ctrl_Lev_KLap;
  double Flow_summ_total;
  double Flow_Summ_Ctrl;
  double Flow_Ctrl_out;
  double dt_w;
  int State;
  _W();
  };

struct CNode
  {
  int Numb;
  int nIO;
  char Type;
  char Spec;
  bool UseForCtrlLev;
  double Slow;
  struct CObjectPoint * pOP;
  };

class CVol : public IBaseModel, public IUniModel, public _W
	{
//
public:
	CVol( char * ObjName, bool Calc_2F, bool FullMatr );
	virtual ~CVol();
// Интерфейс IBaseModel
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
  // Рисуется до вызова PlayEnhMetaFile, не перекрывая название объекта
  // 
  virtual BOOL NeedDrawBefore() { return TRUE; }
///////////////////////////////////////////////////
  int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
//  int GydroInit( IBaseModel * pGydro, int PntGroup, struct CNodeData * pInfo );
  int SetMatr( void * pExternals, int PntGroup,  double dt, 
	  double A[/* kExkV */], double B[ /*kE*/ ] );
  int SetVar( void * pExternals, int PntGroup, double dt, 
	  double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
  int DstMM( char * DataName, struct IMM * pMM );// Вызов получателя сигнала
  NEW
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#define B_2F
#define E_2F
#include "IO_Create.h"
#include "Points.h"
#include "Vol_Defects.h"
#include "ACS_Create.h"
#include "ACS.h"
  CAnalogD * pУровень;
  CAnalogD * pУровень_продукта;
  CAnalogD * pУровень_воды;
/////////////////////////////////////////
  bool Calc_2F;
  CNode * pNodes;
  int kNodes;
  double * Omega;
  CFlow ** pFlow;
  bool * UseTopIn;
  int * Направление;
  int * NumbNode;
  double KsiTop;
  double * dPGydro_Static;
  double * dP_node;
  double * P_Slow;
  double * Mw_Flow;
  double * Flow_No_This;
  double * dPHydro;
  double * dP_Hydro;
  double * kOmega;
  int kIO;
  bool UseSumm;
  int CompLiqGas;
  int OldCompLiqGas;
  int CompMolMass;
/////
  CComp CompGas;
  CComp CompLiq;
  bool CalcCompGas;
  bool CalcCompLiq;
  CVolBase VolBase;
  CSaveVarData SaveVar;
///
  CFlow * pFlow_heat_in;
  CFlow * pFlow_heat_out;
  CFlow * pFlow_heat_in_2;
  CFlow * pFlow_heat_out_2;
  CFlow * pFlow_heat_in_3;
  CFlow * pFlow_heat_out_3;
  bool HeatDefect;
  bool Разгерметизация;
  bool  Напорная;
  bool HydroOverflow;
///
  double Ps;
  double * pFlowOutZ;
  double FlowQ;
  bool DefectUplot;
  double * pExtHeat;
  int kCommIO;
  double P_IO;
///////////////////////////////////////////////
  double Lev_old;
  double M_old;
///////////////////////////////////////////////
  CHeat Heat;
  CHeat Heat_2;
  CHeat Heat_3;
  bool UseHeat[3];
  CLevelControl LevCtrl;
  CStableControl Stable;
  double Flow_Ctrl;
  bool PseudoWater;
  CFlow * pPseudoWater;
  bool UseHeater;
  bool ParamsUsed;
  bool Объема_2;
  CShowVolComp Show_Comp;
  CShow2FComp Show_2F;
  CShowVolFlow Show_Flow;
  CShowP ShowP;
//
  double Omega_Heat, Omega_Heat_2, Omega_Heat_3;
  double Q_Heat_1, Q_Heat_2, Q_Heat_3;
  double k_Heat, k_Heat_2, k_Heat_3;
  double S_Heat, S_Heat_2, S_Heat_3;
  bool Cond_Heat, Cond_Heat_2, Cond_Heat_3;
  double Q_Heat;
  double ShowComp[K_GAS];
  double ShowComp2F[K_GAS];
  double * pFlowProps;
  int Comp12;
  int Comp2FMolMass;
  int Type_Flow;
  char * NodeNames;
  int * NodeNumbs;
  int kNodeNames;
  int nCompBalance;
  double Calc_C[K_GAS];
//
  void CallElectroHeat ( double dt );
  void CalсHeat ( double dt );
  void Calc ( double dt );
  void CalcLev ( double dt );
  void DrawFixed ( void * pData );
  void SetShowComp();
  void Set2FComp();
  void SetFlowProps();
  bool SelectNodePipe( struct CParamClicked * pPC, int nSelectedNode, char * ParamName );
  int GetNumbMode ( int NumbOut );
  void LoadReactions( );
  double TestCalcPress ( double TC );
  double TestCalcTemp ( double Pabs );
	};

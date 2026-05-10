#pragma once

#include "BaseInt.h"
#include "DataTypes.h"
#include "SysDataTypes.h"
#include "SR.h"
#include "Data.h"
#include "Chem.h"
#include "Defects.h"
#include "ACS_Def.h"
#include "EpsVol.h"

#define K_COMP 7
// 0 - C1-C4
// 1 - C5-90
// 2 - 90-170
// 3 - 170-350
// 4 - 350-550
// 5 - 550+
// 6 - Кокс

struct _W_VBR
  {
  double k_Flow_IO;   // F_out = k_Flow * F_in
  double Tout;
  _W_VBR();
  };

class CVBR : public IBaseModel, public IUniModel, public _W_VBR
	{
//
public:
	CVBR( char * ObjName );
	virtual ~CVBR();
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
  int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  int SetMatr( void * pExternals, int PntGroup, double dt, double A[/* kExkV */], double B[ /*kE*/ ] );
  int SetVar( void * pExternals, int PntGroup, double dt, double Var[/*kV*/], struct CBaseStruct * pVar[/*kV*/] );
	NEW
/////////////////////////////////////////////////
#include "IO_Create.h"
#include "Points.h"
#include "ACS_Create.h"
#include "ACS.h"
  CFlow * pFlow_in;
  CFlow * pFlow_out;
  CSEps In, Out_Реактор_0, Out_Реактор; 
  double Flow_in;
// Псевдокомпоненты реакции
  CComp * pComp[6];
  double MwComp[6];
  int ID_Comp[6];
// Реакции ///
  double K[K_COMP][K_COMP];
  double k_Gas, k_P_1, k_P_2, k_Flow;
  double Cin[K_COMP], Cout[K_COMP], S[K_COMP]; //все массовые доли
// Не реагирующие
  bool No_React[K_GAS];
  double Cmol_No_Conv[K_GAS];
  bool IsComp_No_Conv[K_GAS];
  double Flow_No_Conv;
// Реагирующие на входе
  double Cmol_Conv[K_GAS];
  bool IsComp_Conv[K_GAS];
  double Flow_Conv;
  double Flow_Conv_kg; // 
  double Mw_in;
// После реакций
  double dQ_React;
//  double Flow_Conv_kg; == до реакций 
  double Flow_React_mol;
  double Cmol_React[K_GAS];
  bool IsComp_React[K_GAS];
  double Mw_react;
// Поправки к Cmol
  double kComp[K_GAS];
  double kCompMax;
  double CmolOpt[K_GAS];
  double OutToEtalon[K_GAS];
  double Mw_conv;
  double Flow_Conv_mol;
// Результат
  CComp * pCompSumm;
  int ID_CompSumm;
  bool IsCompSumm[K_GAS];
  double CmolSumm[K_GAS];
  double Flow_Summ_mol;
  double Mw_summ;
/////////////////////////////////////////////////
  bool Change_k;
  bool Change_C;
  int kStep;
  char Etalon[32];
  CComp CompEtalon;
  bool SetOptKoef;
//
  void Prepare( );
  void Calc ( double dt );
  void CalcK ( double TC );
  void React( double dt );
  void Convert( );
  void CalcT ( double dt );
	};

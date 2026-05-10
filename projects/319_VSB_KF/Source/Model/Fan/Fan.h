#pragma once

#include "BaseInt.h"
#include "DataTypes.h"
#include "EpsVol.h"
#include "heatoil.h"
#include "Defects.h"
#include "ACS_Def.h"
#include "Oborot.h"
#include "Chem.h"

struct _W
  {
  double dP_work;
  double H, a;
  bool On;
  double Pow;
  double Oborot;
  double Flow_mol;
  COborot ExtOborot;
  double K_P2_P1;
  double Mw_gas;
  double Tin;
  double To_m3_in;
  int pos_Х;
  _W();
  };

struct CPolyMes
  {
  CPolyMes( int * _Размерность )
    {
    Размерность = _Размерность;
    }
  int * Размерность;
  double P_Show;
  double P_atm;
  void Set ( double _P_atm )
    {
    P_atm = _P_atm;
    Show( );
//    P_mm_h2o = P_atm * 1e4;
//   P_Pa = P_atm * Pascal;
//    P_mm_hg = P_atm * 760.;
    }
  void Show ( )
    {
    switch ( *Размерность )
      {
      case 0:
        P_Show = P_atm * 1e4;
      break;
      case 1:
        P_Show = P_atm * Pascal;
      break;
      case 2:
        P_Show = P_atm * 760.;
      break;
      default:
      case 3:
        P_Show = P_atm;
      break;
      }
    }
  void Update ( void * Addr )
    {
    if ( Addr == NULL || Addr == &P_Show )
      {
      switch ( *Размерность )
        {
        default:
        case 0:
          P_atm = P_Show /  1e4;
          break;
        case 1:
          P_atm = P_Show / Pascal;
          break;
        case 2:
          P_atm = P_Show / 760.; 
          break;
        case 3:
          P_atm = P_Show;
          break;
        }
      return;
      }
    if ( Addr == Размерность )
      {
      Show( );
      return;
      }
    }
  };

class CFan : public IBaseModel, public IUniModel, public IElectroModel, public _W
	{
//
public:
	CFan( char * ObjName );
	virtual ~CFan();
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
  int GetProp( );
///////////////////////////////////////////////////
  int TestNodes( int kNodes, struct CObjectPoint ** ppNodes );
  int SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] );
  int SetVar( void * pExternals, int PntGroup, double dt, 
    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] );
  void ElectroOff();
  void ElectroOn();
  ///////////////////////////////////////////////////
//  int Work( double dt );
//  int TestCtrlPoints( int kNodes, struct CObjectPoint ** _ppNodes );
  int PutAddr( char * Point, void * pData, char * Name );
  int DstMM( char * DataName, struct IMM * pMM );// Вызов получателя сигнала
  int SrcMM( char * DataName, struct IMM * pMM );
	NEW
/////////////////////////////////////////////////
//#include "IO_Conn.h"
#include "IO_Create.h"
#include "Points.h"
#include "Fan_Defects.h"
#include "ACS_Create.h"
#include "ACS.h"
/////////////////////////////////////////////////
DEFECT(Останов_вентилятора,"Останов двигателя")
DEFECT(Выход_двигателя_из_строя,"Выход двигателя из строя")
/////////////////////////////////////////////////
  //CPumpData * pPump;
  CFlow * pFlow_in;
  CFlow * pFlow_out;
  //
  CSEps	StateIn;
  CHeatOil* Смазка;
  CHeatAir Охлаждение;
  int Defect;
  double Defect_Pow;
  double _def_Вибраций;
//
  CAnalog ** pVibro;
  double * rndVibrSens;
  int kStep;
  IBaseModel * pAnalogPipe;
  CMotorData * pMotor;
  CMotorData MotorData;
  double Обороты[10];
  double Напор[10];
  int kОН;
  double Поток[10];
  double Характеристика[10];
  int kПХ;
  CPolyMes Hmax;
  CPolyMes H_real;
  CPolyMes H_fact;
  CPolyMes H_zad;
  double aF, cF; 
  double P_kdg;
//
  bool ExtOboror( double dt );
  void IntOboror( double dt );
  void Calc ( double dt );
  void CalcParams( );
  double Hss( double Ro, double Q, double Omega );
  void DrawObj ( struct CDrawObjData * pDraw );
  void Fire( double dt );
  double RelH ( double O );
  double P2_P1_0( );
  void Calc_K_P2_P1( );
  void Спад ( double Flow_mol, double & A, double & C );
  void СпадРаб ( );
  void SetРаб ( );
  bool IsElectro;
};

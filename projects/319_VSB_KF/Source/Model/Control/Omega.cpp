#include "stdafx.h"
#define DLL_Control
#include "Control.h"
#include "Err.h"
#include "SetData.h"
#include "Connect.h"
#include "ACS_Def.h"

COmegaLevel::COmegaLevel( char * _ControlName, struct IBaseModel * _pObject, double * _pLev ):
IBaseControl( _ControlName, _pObject )
  {
  pLev = _pLev;
  pControl = NULL;
  CLEAR(CtrlRef)
  LevZ = 50.;
  LevMin = 15.;
  Lev = 0.;
  P = 20.;
  Tau = 200.;
  OmegaMax = 10000.;
  OmegaMin = 1.;
  RegMax = 100.;
  dFlow = 1000.;
  FlowOut = 0.;
  FlowIn = 0.;
  MV = 0.;
  En1 = En = 0.;
  }


void COmegaLevel::UpdateParams()
  {
  if ( CtrlRef[0] && CtrlRef[0] != '?' )
    {
    pControl = FindRef();
    if ( pControl == NULL )
      CtrlRef[0] = '?';
    }
  else
    pControl = NULL;
  }

double * COmegaLevel::FindRef()
  {
  if ( CtrlRef[0] == 0 )
    return NULL;
  char Point[256];
  lstrcpy ( Point, CtrlRef );
  char * P = strchr ( Point, '.' );
  if ( P == NULL )
    return NULL;
  *P = 0;
  char * Obj = Point;
  IBaseModel * pModel = IBaseModel::Find ( Obj );
  if ( pModel == NULL )
    return NULL;
  char * Param = P + 1;
  if ( Param[0] == '@' )
    {
    // ACS  М_Н_1	M	Remote
    Param++;
    //##
    CExtern_Pnt * pPnt = NULL;//(CExtern_Pnt*)::Find( &pModel->Points, Param, false );
    if ( pPnt == NULL )
      return NULL;
    if ( pPnt->Type[0] != 'A' )
      return NULL;
    CAnalog * pA = (CAnalog*)pPnt;
    double * pAddr = (double*)&(pA->Value);
    return pAddr;
    }
  CLockParams Lock;
  pModel->GetParams( NULL );
  for ( int n = 0; n < CParams::kParams; n++ )
    {
    CParams & Parm = CParams::Params[n];
    if ( lstrcmp ( Parm.ParamName, Param ) == 0 && Parm.Type == 'D' )
      {
      double * pAddr = (double*)Parm.Addr;
      return pAddr;
      }
    }
  return NULL;
  }

void COmegaLevel::GetParams( IBaseModel * pMain )
{
#define this pMain
#include "IO_Parms.h"
	TAB("Регулировка уровня",3)
		PARM(Lev,"#Текущий уровень")
		PARM(LevZ,"Заданный уровень,%")
		PARM(LevMin,"Минимальный уровень")
		PARM_L( TypeReg, "Способ регулировки", 
		"НЕ реулировать;Фиксировать;" // 0,1
		"Выходной поток;" // 2
		"Проходимость;Внешний регулятор;" // 3, 4
		)
		//
		TAB("Выходной поток",3)
			PARM(FlowOut,"#Текущий поток")
			PARM(FlowIn,"#Поступающий поток")
			PARM(dFlow,"Поток дисбаланса")
		ETAB
		TAB("Проходимость",3)
			PARM(Omega,"#Текущая проходимость")
			PARM(OmegaMin,"Мин. проходимость")
			PARM(OmegaMax,"Макс. проходимость")
		ETAB
		TAB("Внешний регулятор",3)
			PARM(MV,"#MV регулятора")
			PARM(CtrlRef,"Имя регулятора(@-ACS)")
			PARM(P,"P как у Yoko")
			PARM(Tau,"T как у Yoko")
			PARM(RegMax,"Регулятор при MV = 1")
		ETAB
	ETAB
#undef this
}

void COmegaLevel::GetParams2( IBaseModel * pMain )
{
#define this pMain
#include "IO_Parms.h"
	TAB("Режим",3)
		PARM(Lev,"#Текущий уровень")
		PARM(LevZ,"Заданный уровень,%")
		PARM(LevMin,"Минимальный уровень")
		PARM_L( TypeReg, "Способ регулировки", 
		"НЕ реулировать;Фиксировать;" // 0,1
		"Выходной поток;" // 2
		"Проходимость;Внешний регулятор;" // 3, 4
		)
		//
		TAB("Параметры регулировки выходного потока",3)
			PARM(FlowOut,"#Текущий поток")
			PARM(FlowIn,"#Поступающий поток")
			PARM(dFlow,"Поток дисбаланса")
		ETAB
		TAB("Параметры регулировки проходимости",3)
			PARM(Omega,"#Текущая проходимость")
			PARM(OmegaMax,"Макс. проходимость")
			PARM(P,"P как у Yoko")
			PARM(Tau,"T как у Yoko")
		ETAB
		TAB("Параметры внешней регулировки",3)
			PARM(CtrlRef,"Имя регулятора(@-ACS)")
			PARM(MV,"#MV регулятора")
			PARM(P,"P как у Yoko")
			PARM(Tau,"T как у Yoko")
			PARM(RegMax,"Регулятор при MV = 1")
		ETAB
	ETAB
#undef this
}

void COmegaLevel::Control( double dt )
  {
  if ( pLev == NULL )
    return;
  Lev = *pLev;
  LevelZ = LevZ;
  if ( TypeReg <= 1 )
    {
//НЕ реулировать, Фиксировать
    return;
    }
  if ( TypeReg == 2 )
    {
//Выходной поток
    double dL = ( Lev - LevZ ) * 0.01;
    FlowOut = FlowIn + dL * dFlow;
    if ( FlowOut < 0. )
      FlowOut = 0.;
    return;
    }
/////////////////
  if ( CtrlRef[0] && CtrlRef[0] != '?' && pControl == NULL )
    pControl = FindRef();
  if ( pControl == NULL )
    {
    if ( CtrlRef[0] && CtrlRef[0] != '?' )
      CtrlRef[0] = '?';
    }
  if ( En == 0. )
    {
    En1 = En = ( Lev - LevZ ) * 0.01;
    }
  else
    {
    En1 = En;
    En = ( Lev - LevZ ) * 0.01;
    }
  dt *= 3600.;
  double Kp = 1. / P;
  double dEn = En - En1;
  double dMV = Kp * ( dEn + dt / Tau * En );
//
  MV += dMV;
  if ( MV < 0. )
    MV = 0.;
  if ( MV > 1. )
    MV = 1.;
  if ( Lev < LevMin )
    MV = 0.;
  if ( TypeReg == 3 )
    {
  // "Проходимость
    const double MaxMin = 5.; // 10^5
    Omega = OmegaMax * pow ( 10., 5 * ( MV * 0.01 - 1. ));
    return;
    }
  if ( pControl == NULL )
    return;
// "Внешний регулятор;" // 3, 4
  *pControl = MV * RegMax;
  }

void COmegaLevel::SaveState( )
  {
  S_CLASS( ControlName, COmegaLevel_W)
  }

int COmegaLevel::RestoreState ( char * StrName )
  {
  R_CLASS( ControlName, COmegaLevel_W)
  return 1;
  }


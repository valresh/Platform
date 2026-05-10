#include "stdafx.h"
#define DLL_BaseCol
#include "BaseCol.h"
#include "math.h"
#include "Err.h"

CTopSep::CTopSep( char * Name, IBaseModel * pMain ) : IBaseModel ( Name, pMain )
, Src ( "Вход", this )
	{
	CLEAR(Src_IsComp)
	CLEAR(Src_CmolLiq)
	CLEAR(Src_CmolGas)
	CLEAR(h_Src_Liq)
	CLEAR(h_Src_Gas)
  pFlowOut = NULL;
  pComp = NULL;
  ID_Comp = 0;
  E = 1.;
  P = 1.;
  F_Gas = 0.;
  F_Liq = 0.;
  UseSep = true;
	};

void CTopSep::Init()
  {
  NewComp( &ID_Comp, &pComp, "Газ из %s", ObjName );
  }

void CTopSep::Set( double _P, double T, 
                   double Flow_Out, bool IsComp[], double CmolGas[] )
  {
  P = _P;
  Src.PT ( P, T, IsComp, CmolGas );
  E = 0.95 * E + 0.05 * Src.E;
  F_Gas = Flow_Out * E;
  F_Liq = Flow_Out - F_Gas;
	Src.SetCompData ( Src_IsComp, Src_CmolLiq, Src_CmolGas,
									  h_Src_Liq, h_Src_Gas );
	pComp->Set ( Src_IsComp, Src_CmolGas );
	pFlowOut-> T = T;
	pFlowOut-> h_Flow = Src.h_gas;
	pFlowOut-> To_kg = ::Mw ( pComp );
	pFlowOut-> To_m3 = M3;
	pFlowOut->pComp = pComp;
	pFlowOut->ID_COMP = ID_Comp;
  }

int CTopSep::GetParams( int & N, struct CParams Params[] )
  {
	#include "IO_Parms.h"
  PARM(UseSep,"Использовать")
  PARM(P,"#P(а)")
  PARM(Src.T,"#T")
  PARM(E,"#E")
  PARM(Src.h_gas,"#h_gas")
  PARM(Src.h_liq,"#h_liq")
  return 0;
  }
int CTopSep::UpdateParam( struct CParams & Param )
  {
  return 0;
  }
int CTopSep::SaveState( )
  {
	S_CLASS("W",CTopSep_W)
  return 0;
  }
int CTopSep::RestoreState ( char * StrName )
  {
	R_CLASS("W",CTopSep_W)
  return 1;
  }

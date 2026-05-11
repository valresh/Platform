#include "stdafx.h"
#include "Valve_b.h"
#include "Err.h"
//#include "CommProc.h"
#include "Work.h"
#include "SetData.h"
#ifdef LINUX
#include "WinDraw.h"
#endif

const double CValve_b::MAX_K_OMEGA = 1e5;
bool CValve_b::ShowConflict = false;
double CValve_b::MaxConflict = 10.;

CValve_b::CValve_b( char * _ObjName, char* _Type ) :	IBaseModel ( _ObjName ) 
, Eps_In("Вход", this )
#include "ACS_Constr.h"
#include "Valve_b_ACS.h"
{
	UNIMODEL
	ObjName = _ObjName;
	pFlow_i = NULL;
	pFlow_o = NULL;
#include "IO_Clear.h"
#include "Valve_b_Pnt.h"
	Performance = Линейная_Характеристика;
	Atmosphere = false;
	Omega_Depressurization = 0.0;
	Omega_max = F_Nom_Liq / dP_Nom_Liq * K_Omega;
	M = Mw_Nom_Liq;
	Ro = Ro_Nom_Liq;
	pAtmComp = NULL;
	eHydroTask = HYDRO_TASK_NO;
	kStep = 0;
	K_def = 1.0;
	dReg = 0.1;
	nDefect = 0;
	lstrcpy(Type, _Type);
	K_F = M /Ro;
	calcConflict = CALC_NO;
    Eps = 1.0;
}

CValve_b::~CValve_b()
{

}

int CValve_b::Init( int Regim )
{
	int Res = 0;
	//#include "IO_Init.h"
	//#include "Points.h"
	return 0;
}

int CValve_b::Step0( )
{
	if(Atmosphere)
	{
		NewComp( &ID_COMP_ATM, &pAtmComp, "Состав из %s", ObjName.Str );
		pAtmComp->Set(pAir);
	}
	Pin = 1.;
	Pout = 1.;
	return 0;
}

int CValve_b::Step1( )
{
	if(Gas)
		Omega_max = F_Nom_Gas / dP_Nom_Gas; 
	else 
		Omega_max = F_Nom_Liq / dP_Nom_Liq;
	return 0;	
}

int CValve_b::StepT ( double dt )
{
	Calc ( dt );
	return 0;
}

int CValve_b::SetData( int TypeData, void * pData )
{
	if ( TypeData == sd_ShowObjInfo )
	{
		Char<256>* pInfo = (Char<256>*)pData;
		pInfo->Prt( "%s: V=%4.1lf%%, dP=%4.2lf, F=%4.2lfкг", ObjName.Str, Position, _Differential, _Flow_Work_kg );
		return 0;
	}
	if ( TypeData == sd_SetInitState )
	{
		Position = 0;
		return 0;
	}
	return 1;
}

int CValve_b::SaveState( )
{
	S_CLASS("_W_Valve_b",_W_Valve_b);
	return 0;
}

int CValve_b::RestoreState( char * StrName )
{
	COPY_FIRST;
	R_CLASS("_W_Valve_b",_W_Valve_b);
	return 1;
}

void CValve_b::DrawObj ( struct CDrawObjData * pDraw )
{
	TransparentStd( pDraw->hDC, pDraw->rc, ColZadv(Position));
	if(eHydroTask != HYDRO_TASK_NO)
	{
		
		static HBRUSH hFon = (HBRUSH)GetStockObject( NULL_BRUSH );
		HBRUSH hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFon );
		HPEN hOldPen;
		switch(eHydroTask)
		{
		case HYDRO_TASK_NORM:
			{
				static HPEN hPen = CreatePen( PS_SOLID, 1, RGB ( 0, 255, 255 ));
				hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPen );		
			}
			break;
		case HYDRO_TASK_CONFLICT:
			{
				static HPEN hPen = CreatePen( PS_SOLID, 1, RGB ( 255, 0, 0 ));
				hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPen );		
			}			
		}
#ifdef LINUX
        int x0 = pDraw->rc.left-2;
        int y0 = pDraw->rc.top-2;
        int x1 = pDraw->rc.right+2;
        int y1 = pDraw->rc.bottom+2;
        MoveToEx( pDraw->hDC, x0, y0, NULL );
        LineTo( pDraw->hDC, x1, y0 );
        LineTo( pDraw->hDC, x1, y1 );
        LineTo( pDraw->hDC, x0, y1 );
        LineTo( pDraw->hDC, x0, y0 );
        x0 = pDraw->rc.left-4;
        y0 = pDraw->rc.top-4;
        x1 = pDraw->rc.right+4;
        y1 = pDraw->rc.bottom+4;
        MoveToEx( pDraw->hDC, x0, y0, NULL );
        LineTo( pDraw->hDC, x1, y0 );
        LineTo( pDraw->hDC, x1, y1 );
        LineTo( pDraw->hDC, x0, y1 );
        LineTo( pDraw->hDC, x0, y0 );
#else
		Rectangle ( pDraw->hDC, pDraw->rc.left-2, pDraw->rc.top-2, pDraw->rc.right+2, pDraw->rc.bottom+2 );
		Rectangle ( pDraw->hDC, pDraw->rc.left-4, pDraw->rc.top-4, pDraw->rc.right+4, pDraw->rc.bottom+4 );
#endif
		SelectObject ( pDraw->hDC, hOldPen );
		SelectObject ( pDraw->hDC, hOldFon );
	}
}












#include "stdafx.h"
#include "Vol.h"
#include "Err.h"
#include "CommProc.h"
#include "SetData.h"

#define ROUNDTOINT(number)	(int)floor(number + 0.5)
#define NEW_DRAW_LOGIC

enum BRUSH
{
	BRUSH_FILL,
  BRUSH_LEVEL,
};

enum PEN
{
	PEN_RECT_FIXED,
	PEN_RECT
};

_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
  WorkHeat = false;
	}

#undef B_2F
#define B_2F \
  if ( Show2F ) {

#undef E_2F
#define E_2F \
    }

static char CompNames[1044];
int NumbCompNames[K_GAS];
int kNumbCompNames = 1;

int PressMes = 0;
int CVol::GetParams(  char * StrName )
	{
	#include "IO_Parms.h"   
  char Name[256];
  bool Show2F = Calc_2F;
  if ( CParams::SaveAllParams )
    Show2F = true;
  ShowAllParams = true;
  if ( ShowAllParams || CParams::SaveAllParams )
    {
    ShowP.pMes = &PressMes;
#define SHOW
	#include "Points.h"
#undef SHOW
//////////////////////////////////////////////////
//................................................
    Defect_Params(this);
	ACS_Params( "АСУ", this );
//  ShowAllParams = true;
    TAB("Модули",5)
    TAB("Термодинамика",5)
    VolBase.GetParams( NULL );
    ETAB
    if ( UseHeat[0]  )
      {
      TAB("Змеевик 1",5)
      Heat.GetParams( NULL );
      ETAB
      }
    if ( UseHeat[1]  )
      {
      TAB("Змеевик 2",5)
      Heat_2.GetParams( NULL );
      ETAB
      }
    if ( UseHeat[2]  )
      {
      TAB("Змеевик 3",5)
      Heat_3.GetParams( NULL );
      ETAB
      }
	//if ( UseHeater )
	//{
		TAB("Электрообогрев", 5 )
		PARM( WorkHeat, "#Обогрев включен" )
		PARM( Q_Heat, "Мощность нагревателя, кВт" )
		ETAB
	//}
    TAB("Контроль уровня",5)
    LevCtrl.GetParams( this );
    ETAB
    TAB("Стабильность",5)
    Stable.GetParams( this );
    ETAB
    ETAB
    }
	return 0;
	}

int CVol::UpdateParam( CParams & Param )
  {
  if ( Param.Addr == &InitVol )
    {
    InitVol = false;
    VolBase.VolInit ( pN2, 1., T_air );
    OverFlow = false;
    for ( int i = 0; i < kIO; i++ )
      {
      VolBase.pComp_Out[i] ->Set ( pN2 );
      }
    return 0;
    }
  // if ( Param.Addr == &VolBase.Залить )
  //   {
  //   CSrcCompData * pData = (CSrcCompData*)VolBase.Залить;
  //   int Res = VolBase.UpdateParam( Param );
  //   Reg_L_1.Set( -pData->Lev );
  //   Reg_T.Set ( pData->T );
  //   return Res;
  //   }
  if ( Param.Addr == &Norm_Calc_C )
    {
    Norm_Calc_C = false;
    double S = 0.;
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( VolBase.IsComp[n] )
        S += Calc_C[n];
      }
    if ( S > 1e-3 )
      {
      double N = 100. / S;
      for ( int n = 0; n < LAST_GAS; n++ )
        {
        if ( VolBase.IsComp[n] )
          Calc_C[n] *= N;
        }
      }
    Calc_P = TestCalcPress( Calc_T ) - 1.;
    return 0;
    }
  if ( Param.Addr == &Calc_T )
    {
    Calc_P = TestCalcPress( Calc_T ) - 1.;
    return 0;
    }
  if ( Param.Addr == &Calc_P )
    {
    Calc_T = TestCalcTemp( Calc_P + 1. );
    return 0;
    }
	if ( KlapReg[0] )
    {
    IBaseModel * pKlap = IBaseModel::Find( KlapReg );
    if ( pKlap )
      pKlap->SetData ( sd_GetFlowZ, (void*)&pFlowOutZ );
    }
  if ( VolBase.Vol2F > 0.1 )
    Объема_2 = true;
  else
    Объема_2 = false;
  return 0;
  }

//#include "ShowParams.h"

int CVol::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
	switch ( DataType )
		{
    case sp_Actions:
//      pSD->NewAction = pSD->ОкноСоставов;
      pSD->NewAction = pSD->РасчетДавлений;
      pSD->pModel = this;
		return 1;
		}
  return 0;
  }

void CVol::DrawFixed ( void * pData )
  {
  if ( !VolBase.ЕстьЗадания )
    return ;
  //
  static HPEN hPen = CreatePen( PS_SOLID, 1, RGB ( 0, 255, 255 )); 
  static HBRUSH hFon = (HBRUSH)GetStockObject( NULL_BRUSH ); 
  CDrawObjData * pDraw = (CDrawObjData *)pData;
  HPEN hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPen );
  HBRUSH hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFon );
  Rectangle ( pDraw->hDC, pDraw->rc.left-2, pDraw->rc.top-2, pDraw->rc.right+2, pDraw->rc.bottom+2 );
  Rectangle ( pDraw->hDC, pDraw->rc.left-4, pDraw->rc.top-4, pDraw->rc.right+4, pDraw->rc.bottom+4 );
  SelectObject ( pDraw->hDC, hOldPen );
  SelectObject ( pDraw->hDC, hOldFon );
  }

HBRUSH hFon = CreateSolidBrush( RGB ( 142, 142, 142 )); 
HBRUSH hLev_1 = CreateSolidBrush( RGB ( 192, 0, 0 )); 
HBRUSH hLev_2 = CreateSolidBrush( RGB ( 0, 0, 192 )); 
HPEN hPen = CreatePen( PS_SOLID, 1, RGB ( 142, 142, 142 )); 
HPEN hPen_1 = CreatePen( PS_SOLID, 1, RGB ( 192, 0, 0 )); 
HPEN hPen_2 = CreatePen( PS_SOLID, 1, RGB ( 0, 0, 192 )); 
void CVol::DrawObj ( struct CDrawObjData * pDraw )
  {
	  if(g_bInstructor)
		  return;
	DrawFixed ( pDraw );
	int W = pDraw->rc.right - pDraw->rc.left;
	int H = pDraw->rc.bottom - pDraw->rc.top;
//
  HPEN hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPen );
  HBRUSH hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFon );
  if ( Calc_2F )
    {
    if ( Объема_2 )
      {
      int w_2F = (int)( W * W_2F + 0.5 );
      int h_2F = (int)( H * H_2F + 0.5 ); 
      int x0_2F = (int)( pDraw->rc.left + W * X_2F + 0.5 );
      int y0_2F = (int)( pDraw->rc.top + H * Y_2F + 0.5 );
      double dL_1 = VolBase.Lev_liq_1in2;
      if ( dL_1 < 0. )
        dL_1 = 0.;
      if ( dL_1 > 1. )
        dL_1 = 1.;
      double dL_2 = VolBase.Lev_liq_2;
      if ( dL_2 < 0. )
        dL_2 = 0.;
      if ( dL_2 > 1. )
        dL_2 = 1.;
      int dh_1 = (int)( dL_1 * h_2F + 0.5 ); 
      int dh_2 = (int)( dL_2 * h_2F + 0.5 );
      int dh = h_2F - dh_1 - dh_2; 
      SelectObject ( pDraw->hDC, hPen );
      SelectObject ( pDraw->hDC, hFon );
      int yt = y0_2F;
      int yb = yt + dh;
      if ( dh > 0 )
        Rectangle ( pDraw->hDC, x0_2F, yt, x0_2F + w_2F, yb ); 
      //
      yt = yb;
      yb += dh_1;
      if ( dh_1 > 0 )
        {
        SelectObject ( pDraw->hDC, hLev_1 );
        SelectObject ( pDraw->hDC, hPen_1 );
        Rectangle ( pDraw->hDC, x0_2F, yt, x0_2F + w_2F, yb ); 
        }
      //
      yt = yb;
      yb += dh_2;
      if ( dh_2 > 0 )
        {
        SelectObject ( pDraw->hDC, hLev_2 );
        SelectObject ( pDraw->hDC, hPen_2 );
        Rectangle ( pDraw->hDC, x0_2F, yt, x0_2F + w_2F, yb ); 
        }
  ////////////////////////////////////////////////////////////////
      int w = (int)( W * W_lev + 0.5 );
      int h = (int)( H * H_lev + 0.5 ); 
      int x0 = (int)( pDraw->rc.left + W * X_lev + 0.5 );
      int y0 = (int)( pDraw->rc.top + H * Y_lev + 0.5 );
      dL_1 = VolBase.Lev_liq_1;
      if ( dL_1 < 0. )
        dL_1 = 0.;
      if ( dL_1 > 1. )
        dL_1 = 1.;
      dL_2 = VolBase.Lev_liq_2in1;
      if ( dL_2 < 0. )
        dL_2 = 0.;
      if ( dL_2 > 1. )
        dL_2 = 1.;
      dh_1 = (int)( dL_1 * h + 0.5 ); 
      dh_2 = (int)( dL_2 * h + 0.5 );
      dh = h - dh_1 - dh_2; 
      SelectObject ( pDraw->hDC, hPen );
      SelectObject ( pDraw->hDC, hFon );
      yt = y0;
      yb = yt + dh;
      if ( dh > 0 )
        Rectangle ( pDraw->hDC, x0, yt, x0 + w, yb ); 
      //
      yt = yb;
      yb += dh_1;
      if ( dh_1 > 0 )
        {
        SelectObject ( pDraw->hDC, hLev_1 );
        SelectObject ( pDraw->hDC, hPen_1 );
        Rectangle ( pDraw->hDC, x0, yt, x0 + w, yb ); 
        }
      //
      yt = yb;
      yb += dh_2;
      if ( dh_2 > 0 )
        {
        SelectObject ( pDraw->hDC, hLev_2 );
        SelectObject ( pDraw->hDC, hPen_2 );
        Rectangle ( pDraw->hDC, x0, yt, x0 + w, yb ); 
        }
      }
    else
      {
      int w = (int)( W * W_lev + 0.5 );
      int h = (int)( H * H_lev + 0.5 ); 
      int x0 = (int)( pDraw->rc.left + W * X_lev + 0.5 );
      int y0 = (int)( pDraw->rc.top + H * Y_lev + 0.5 );
      double dL_1 = VolBase.Lev_liq_1;
      if ( dL_1 < 0. )
        dL_1 = 0.;
      if ( dL_1 > 1. )
        dL_1 = 1.;
      double dL_2 = VolBase.Lev_liq_2;
      if ( dL_2 < 0. )
        dL_2 = 0.;
      if ( dL_2 > 1. )
        dL_2 = 1.;
      int dh_1 = (int)( dL_1 * h + 0.5 ); 
      int dh_2 = (int)( dL_2 * h + 0.5 );
      int dh = h - dh_1 - dh_2; 
      SelectObject ( pDraw->hDC, hPen );
      SelectObject ( pDraw->hDC, hFon );
      int yt = y0;
      int yb = yt + dh;
      if ( dh > 0 )
        Rectangle ( pDraw->hDC, x0, yt, x0 + w, yb ); 
      //
      yt = yb;
      yb += dh_1;
      if ( dh_1 > 0 )
        {
        SelectObject ( pDraw->hDC, hLev_1 );
        SelectObject ( pDraw->hDC, hPen_1 );
        Rectangle ( pDraw->hDC, x0, yt, x0 + w, yb ); 
        }
      //
      yt = yb;
      yb += dh_2;
      if ( dh_2 > 0 )
        {
        SelectObject ( pDraw->hDC, hLev_2 );
        SelectObject ( pDraw->hDC, hPen_2 );
        Rectangle ( pDraw->hDC, x0, yt, x0 + w, yb ); 
        }
      }
    }
  else
    {
    int w = (int)( W * W_lev + 0.5 );
    int h = (int)( H * H_lev + 0.5 ); 
    int x0 = (int)( pDraw->rc.left + W * X_lev + 0.5 );
    int y0 = (int)( pDraw->rc.top + H * Y_lev + 0.5 );
    double dL_1 = VolBase.Lev_liq;
	  if ( dL_1 < 0. )
	    dL_1 = 0.;
    if ( dL_1 > 1. )
      dL_1 = 1.;
	  int dh_1 = (int)( dL_1 * h + 0.5 ); 
    int dh = h - dh_1; 
  //
    hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPen );
    hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFon );
    int yt = y0;
    int yb = yt + dh;
    if ( dh > 0 )
	    Rectangle ( pDraw->hDC, x0, yt, x0 + w, yb ); 
  //
    yt = yb;
    yb += dh_1;
    if ( dh_1 > 0 )
      {
      SelectObject ( pDraw->hDC, hLev_1 );
      SelectObject ( pDraw->hDC, hPen_1 );
      Rectangle ( pDraw->hDC, x0, yt, x0 + w, yb ); 
      }
    }
//
  SelectObject ( pDraw->hDC, hOldFon );
	SelectObject ( pDraw->hDC, hOldPen );
  }

void CVol::AddResources (CGDIResourceMgr *pResMgr)
  {
	//AddBrush (pResMgr, BRUSH_FILL, RGB (112, 146, 190));
	//AddBrush (pResMgr, BRUSH_LEVEL, RGB (192, 0, 0));
	//AddPen (pResMgr, PEN_RECT_FIXED, RGB (0, 255, 255), 1);
	//AddPen (pResMgr, PEN_RECT, RGB (71, 109, 160), 1);
  }

void CVol::DrawObject (struct CDrawObjData *pDraw, CGDIResourceMgr *pResMgr)
  {
  if(g_bInstructor)
	  return;
  DrawObj ( pDraw );
  return;
	HBRUSH hBrFill = GetBrush (pResMgr, pDraw->nModelID, BRUSH_FILL);
	HBRUSH hBrRed = GetBrush (pResMgr, pDraw->nModelID, BRUSH_LEVEL);
	HPEN hPenF = GetPen (pResMgr, pDraw->nModelID, PEN_RECT_FIXED);
	HPEN hPen = GetPen (pResMgr, pDraw->nModelID, PEN_RECT);

	if ( VolBase.ЕстьЗадания )
	{
		HBRUSH hFon = (HBRUSH)GetStockObject (NULL_BRUSH);
		HPEN hOldPen = (HPEN)SelectObject (pDraw->hDC, hPenF);
		HBRUSH hOldFon = (HBRUSH)SelectObject (pDraw->hDC, hFon);
		Rectangle (pDraw->hDC, pDraw->rc.left-2, pDraw->rc.top-2, pDraw->rc.right+2, pDraw->rc.bottom+2);
		Rectangle (pDraw->hDC, pDraw->rc.left-4, pDraw->rc.top-4, pDraw->rc.right+4, pDraw->rc.bottom+4);
		SelectObject (pDraw->hDC, hOldPen);
		SelectObject (pDraw->hDC, hOldFon);
	}
	HBRUSH hOldFon = (HBRUSH)SelectObject (pDraw->hDC, hBrFill);
	HPEN hOldPen = (HPEN)SelectObject (pDraw->hDC, hPen);
	int W = pDraw->rc.right - pDraw->rc.left;
	int H = pDraw->rc.bottom - pDraw->rc.top;
	bool bHorz = W > H;
	double dMargin = bHorz ? W / 8.0 : H / 8.0;
	int nMargin = (int)ROUNDTOINT(dMargin);
	double dw = bHorz ? (W - 2 * (nMargin - 2)) : W;
	int w = (int)ROUNDTOINT(dw);
	int x0 = pDraw->rc.right - w - 1;
	if ( bHorz )
		x0 -= nMargin;
	int y0 = bHorz ? pDraw->rc.top : pDraw->rc.top + nMargin - 1;
	int h = bHorz ? H - 1 : H - nMargin * 2;
	if ( w >= 5 && h >= 5 )
	{
		double dL = 1. - VolBase.Lev_liq;
		if ( dL < 0. )
			dL = 0.;
		int dhl_1 = (int)(dL * h); 
		Rectangle ( pDraw->hDC, x0, y0, x0 + w, y0 + h ); 
		SelectObject (pDraw->hDC, hBrRed);
		Rectangle (pDraw->hDC, x0, y0 + dhl_1, x0 + w, y0 + h ); 
	}
	SelectObject (pDraw->hDC, hOldFon);
	SelectObject (pDraw->hDC, hOldPen);
	BOOL bRes = ::DeleteObject (hBrFill);
	bRes &= ::DeleteObject (hBrRed);
	bRes &= ::DeleteObject (hPenF);
	bRes &= ::DeleteObject (hPen);
	if ( !bRes )
		KKK();
}

void CVol::SetShowComp()
  {
  double * Cmol;
  if ( CompLiqGas == 2 )
    {
    if ( OldCompLiqGas == 0 )
      Cmol = VolBase.Cmol_Liq;
    else
      Cmol = VolBase.Cmol_Gas;
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( VolBase.IsComp[n] )
        {
        Calc_C[n] = Cmol[n] * 100.;
        }
      }
    CompLiqGas = OldCompLiqGas;
    return;
    }
  OldCompLiqGas = CompLiqGas;
  if ( CompLiqGas == 0 )
    Cmol = VolBase.Cmol_Liq;
  else
    Cmol = VolBase.Cmol_Gas;
  if ( CompMolMass == 0 )
    {
// Моль
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( VolBase.IsComp[n] )
        {
        ShowComp[n] = Cmol[n] * 100.;
        }
      }
    return;
    }
// Масс
  double M = Mw( VolBase.IsComp, Cmol );
  double N = 100. / M;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( VolBase.IsComp[n] )
      {
      ShowComp[n] = Cmol[n] * pGas[n]->Mw * N;
      }
    }
  }

void CVol::Set2FComp()
  {
  double * Cmol;
  if ( VolBase.Is_2F )
    {
    if ( Comp12 == 0 )
      Cmol = VolBase.Cmol_1F;
    else
      Cmol = VolBase.Cmol_2F;
    }
  else
    {
    Comp12 = 0;
    Cmol = VolBase.Cmol_Liq;
    }
  
  if ( Comp2FMolMass == 0 )
    {
    // Моль
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( VolBase.IsComp[n] )
        {
        ShowComp2F[n] = Cmol[n] * 100.;
        }
      }
    return;
    }
  // Масс
  double M = Mw( VolBase.IsComp, Cmol );
  double N = 100. / M;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( VolBase.IsComp[n] )
      {
      ShowComp2F[n] = Cmol[n] * pGas[n]->Mw * N;
      }
    }
  }
#define Flow_min 1e-5

void CVol::SetFlowProps()
  {
//  PARM_L( Type_Flow, "Параметр потока", "Поток, кг;Поток, кмоль;Доля газа;Перепад;" )
  Char<128>Name;
  if ( Type_Flow == 0 )
    {
    for ( int n = 0; n < kIO; n++ )
      {
 //     sprintf_s ( Name, sizeof(Name), "#%s-Поток, кг", (char*)pNodes[n].pOP->PntName );
      if ( VolBase.Flow_kg )
        {
        double F = VolBase.Flow_kg[n];
        if ( fabs ( F ) < Flow_min )
          F = 0.;
        pFlowProps[n] = F;
        }
      }
    return;
    }
  if ( Type_Flow == 1 )
    {
    for ( int n = 0; n < kIO; n++ )
      {
//      sprintf_s ( Name, sizeof(Name), "#%s-Поток, кмоль", (char*)pNodes[n].pOP->PntName );
      if ( VolBase.pFlow[n] )
        {
        double F = VolBase.pFlow[n]->Flow_mol;
        if ( fabs ( F ) < Flow_min )
          F = 0.;
        pFlowProps[n] = F;
        }
      }
    return;
    }
  if ( Type_Flow == 2 )
    {
    for ( int n = 0; n < kIO; n++ )
      {
      //      Температура
      if ( VolBase.State_in[n] )
        pFlowProps[n] = VolBase.State_in[n]->T;
      }
    return;
    }
  if ( Type_Flow == 3 )
    {
    for ( int n = 0; n < kIO; n++ )
      {
//      sprintf_s ( Name, sizeof(Name), "#%s-Доля газа", (char*)pNodes[n].pOP->PntName );
      if ( VolBase.State_in[n] )
        pFlowProps[n] = VolBase.State_in[n]->E;
      }
    return;
    }
  if ( Type_Flow == 4 )
    {
    for ( int n = 0; n < kIO; n++ )
      {
//      sprintf_s ( Name, sizeof(Name), "#%s-Перепад", (char*)pNodes[n].pOP->PntName );
      pFlowProps[n] = dP_node[n];
      }
    return;
    }
  }

double CVol::TestCalcPress( double TC )
  {
  if ( Gas_Calc_C )
    {
    double S = 0.;
    double Pz = 0.;
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( VolBase.IsComp[n] )
        {
        double X = Calc_C[n] / pGas[n]->Pz( TC );
        S += X;
        Pz += Calc_C[n] * VolBase.StateVol.Henry[n];
        }
      }
    if ( S > 1e-3 )
      {
      double N = 1. / S;
      return Pz * N; 
      }
    return 0.;
    }
  double S = 0.;
  double Pz = 0.;
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( VolBase.IsComp[n] )
      {
      S += Calc_C[n];
      Pz += Calc_C[n] * pGas[n]->Pz( TC ) * VolBase.StateVol.Henry[n];
      }
    }
  if ( S > 1e-3 )
    {
    double N = 1. / S;
    return Pz * N; 
    }
  return 0.;
  }

double CVol::TestCalcTemp( double Pabs )
  {
  double Tb = T_min;
  double Te = T_max;
  double T = 0.5 * ( Tb + Te );
  while ( Te - Tb > 1e-5 )
    {
    T = 0.5 * ( Tb + Te );
    double P = TestCalcPress( T );
    if ( P < Pabs )
      Tb = T;
    else
      Te = T;
    }
  return T;
  }

#include "stdafx.h"
#include "Gag.h"
#include "Err.h"
#include "CommProc.h"
#include "SetData.h"
#include "Param.h"
#ifdef LINUX
#include "WinDraw.h"
#endif


// Жидкость
static double def_Ro_std_liq = 1000.;// кг/м3
static double def_Mu_std_liq = 18.;
static double def_Dy_std_liq = 250.;// мм
static double def_Q_std_liq = 500.; // м3/ч
static double def_dP_std_liq = 0.001;
// Газ
static double def_T_std_gas = 298.15; // К
static double def_Mu_std_gas = 16.;
static double def_Dy_std_gas = 200.;// мм
static double def_Q_std_gas = 50000; // нм3/ч
static double def_Pin_std_gas = 30.;
static double def_dP_std_gas = 0.001;
// Жидкость
double CGag::Ro_std_liq = def_Ro_std_liq;// кг/м3
double CGag::Mu_std_liq = def_Mu_std_liq;
double CGag::Dy_std_liq = def_Dy_std_liq;// мм
double CGag::Q_std_liq = def_Q_std_liq; // м3/ч
double CGag::dP_std_liq = def_dP_std_liq;
// Газ
double CGag::Dy_std_gas = def_Dy_std_gas;// мм
double CGag::Q_std_gas = def_Q_std_gas; // нм3/ч
double CGag::dP_std_gas = def_dP_std_gas;
//
double CGag::T_std_gas = def_T_std_gas; // К
double CGag::Mu_std_gas = def_Mu_std_gas;
double CGag::Pin_std_gas = def_Pin_std_gas;

void CGag::CalcK_Gag( )
  {
  if ( GasGag )
    {
    // Omega = K * V                           Ksi < 0.5   Q = Omega * dP
    // Q = K * 0.5 * Pin * V / sqrt ( Mu )     Ksi >= 0.5
    double Q_mol = Q_std_gas * iM3;
    K_Gag = К_Reg * Q_mol * Dy * Dy * sqrt ( Mu_std_gas ) / 
      ( sqrt ( Mu ) * Dy_std_gas * Dy_std_gas * dP_std_gas );
    return;
    }
  double Q_mol = Q_std_liq * Ro_std_liq / Mu_std_liq;
  K_Gag = К_Reg * Q_mol * Dy * Dy * sqrt ( Ro_liq ) * Mu_std_liq / 
    ( sqrt ( Ro_std_liq ) * Mu * Dy_std_liq * Dy_std_liq * dP_std_liq );
  }

_W::_W()
  {
  memset ( this, 0, sizeof ( _W ));
  kSS = 1.;
  }


int CGag::GetParams(  char * StrName )
	{
  #include "IO_Parms.h"
  #include "Points.h"
  Defect_Params(this);
	return 0;
	}

int CGag::UpdateParam( CParams & Param )
  {
  NewParams = true;
  return 0;
  }

//#include "ShowParams.h"

int CGag::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
	switch ( DataType )
		{
		case sp_Actions:
      if ( Заглушка == false )
        {
//        pGag->mOn = true;
        Заглушка = true;
        }
      else
        {
//        pGag->mOn = false;
        Заглушка = false;
        }
		return 0;
		}
  return 0;
  }

double CGag::CalcOmega( )
  {
  if ( NewParams )
    {
    CalcK_Gag( );
    NewParams = false;
    }
  IsOpen = Заглушка;
  if ( IsOpen )
    return K_Gag;
  return K_Gag * VentMin;
  }

void CGag::DrawNoUsed ( struct CDrawObjData * pDraw )
  {
  static HPEN hPen = CreatePen( PS_SOLID, 1, RGB ( 0, 255, 255 )); 
  static HBRUSH hFon = (HBRUSH)GetStockObject( NULL_BRUSH ); 
  HPEN hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPen );
  HBRUSH hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFon );
  Ellipse ( pDraw->hDC, pDraw->rc.left-2, pDraw->rc.top-2, pDraw->rc.right+2, pDraw->rc.bottom+2 );
  SelectObject ( pDraw->hDC, hOldPen );
  }

//HBRUSH hFonOff = CreateSolidBrush( RGB ( 255, 0, 0 ));
//HBRUSH hFonOn = CreateSolidBrush( RGB ( 0, 255, 0 ));
//HFONT hFont = NULL;
void CGag::DrawObj ( struct CDrawObjData * pDraw )
  {
  if(g_bInstructor)
    return;
  if ( NoUsed )
    DrawNoUsed ( pDraw );
  // if ( hFont == NULL )
  //   {
  //   hFont = CreateFont (
  //     14,
  //     0,                // average character width
  //     0,           // angle of escapement
  //     0,          // base-line orientation angle
  //     FW_BOLD,              // font weight
  //     0,           // italic attribute option
  //     0,        // underline attribute option
  //     0,        // strikeout attribute option
  //     ANSI_CHARSET,          // character set identifier
  //     OUT_TT_ONLY_PRECIS,  // output precision
  //     CLIP_DEFAULT_PRECIS,    // clipping precision
  //     ANTIALIASED_QUALITY,          // output quality
  //     VARIABLE_PITCH,   // pitch and family
  //     "Tahoma"           // typeface name
  //     );
  //   }
  //if ( Break )
  //  {
  //  static HPEN hPen = CreatePen( PS_SOLID, 1, RGB ( 0, 255, 255 )); 
  //  static HBRUSH hFon = (HBRUSH)GetStockObject( NULL_BRUSH ); 
  //  CDrawObjData * pDraw = (CDrawObjData *)pData;
  //  HPEN hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPen );
  //  HBRUSH hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFon );
  //  Rectangle ( pDraw->hDC, pDraw->rc.left-2, pDraw->rc.top-2, pDraw->rc.right+2, pDraw->rc.bottom+2 );
  //  Rectangle ( pDraw->hDC, pDraw->rc.left-4, pDraw->rc.top-4, pDraw->rc.right+4, pDraw->rc.bottom+4 );
  //  SelectObject ( pDraw->hDC, hOldPen );
  //  SelectObject ( pDraw->hDC, hOldFon );
  //  }
  //else
  //  {
  //  if ( OnlyOut || OnlyIn )
  //    {
  //    int Align = SetTextAlign ( pDraw->hDC, TA_CENTER | TA_BOTTOM );
  //    COLORREF Col = SetTextColor	( pDraw->hDC, RGB ( 255, 255, 255 ));
  //    HFONT hFontOld = (HFONT)SelectObject ( pDraw->hDC, hFont );
  //    int Mode = SetBkMode ( pDraw->hDC, TRANSPARENT ); 
  //    TextOut ( pDraw->hDC, pDraw->rc.right, pDraw->rc.top,	"##", 2 );
  //    SetTextColor	( pDraw->hDC, Col );
  //    SelectObject ( pDraw->hDC, hFontOld );
  //    SetTextAlign ( pDraw->hDC, Align ); 
  //    SetBkMode ( pDraw->hDC, Mode ); 
  //    }
  //  }
  //if ( Type == 0 )
  //  {
  //  if ( Open )
  //    FillRect ( pDraw->hDC, &pDraw->rc, hFonOn );
  //  else
  //    FillRect ( pDraw->hDC, &pDraw->rc, hFonOff );
  //  return ;
  //  }
  // if ( Заглушка != 0 )
  //   FillRect ( pDraw->hDC, &pDraw->rc, hFonOn );
  // else
  //   FillRect ( pDraw->hDC, &pDraw->rc, hFonOff );
  if ( Заглушка != 0 )
    TransparentStd( pDraw->hDC, pDraw->rc, RGB(0,255,0 ) );
   else
    TransparentStd( pDraw->hDC, pDraw->rc, RGB(255,255,0 ) );
  }


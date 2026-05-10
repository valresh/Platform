#include "stdafx.h"
#include "IO.h"
#include "Err.h"
#include "CommProc.h"
#ifdef LINUX
#include "WinDraw.h"
#endif

#define ROUNDTOINT(number)	(int)floor(number + 0.5)
#define NEW_DRAW_LOGIC

enum BRUSH
{
	BRUSH_FILL_SCR,
	BRUSH_FILL_DST,
	BRUSH_FILL_CONN
};

enum PEN
{
	PEN_SRC,
	PEN_DST,
	PEN_CONN,
	PEN_ERR
};

_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
	}


int CIO::GetParams(  char * StrName )
	{
//  if ( strstr ( ObjName, "25-P-2056-ITW к V-101" ))
//    KKK();
	#include "IO_Parms.h"
	#include "Points.h"
//  Script.GetParams();
//  Script.GetDefects();
  if ( pComp && pComp->pSpecial )
    {
    pComp->pSpecial->GetParams( this );
    }
  TAB("Задания", 5 )
  CTRL_Params( "", this );
  PARM_B( SetFlow, "Задать сами потоки", true )
  PARM_D( Omega_min_Z, "Минимальная проходимость", 0. / mOmega )
  PARM_D( Omega_max_Z, "Максимальная проходимость", 1e6 / mOmega )
  PARM_D( k_Omega_Z, "Скорость изменения проходимости", 1. )
  ETAB
  ACS_Params( "АСУ", this );
//	TAB("Модули",5)
	return 0;
	}

int CIO::UpdateParam( CParams & Param )
  {
  if ( Param.Addr == Composition )
    {
    SetComp( );
    return 0;
    }
  if ( Param.Addr == &WorkAsSrc && WorkAsSrc )
    {
    SetComp( );
    return 0;
    }
  if ( Param.Addr == &CalcPT || Param.Addr == &Pizb || Param.Addr == &T )
    {
    if ( CalcPT == 1 && Param.Addr != &T )
      {
//Т насыщенного пара
      double Te = T;
      double Tb = T;
      State.PT( Pizb + 1., Te, pComp );
      if ( State.E > 0. )
        {
        while ( State.E > 0. )
          {
          Te = Tb;
          if ( Tb < -80 )
            return 0;
          Tb -= 20.;
          State.PT( Pizb + 1., Tb, pComp );
          }
        }
      else
        {
        while ( State.E <= 0. )
          {
          Tb = Te;
          if ( Te > 400. )
            return 0;
          Te += 20.;
          State.PT( Pizb + 1., Te, pComp );
          }
        }
      double Tm = 0.5 * ( Tb + Te );
      while ( Te - Tb > 1e-6 )
        {
        Tm = 0.5 * ( Tb + Te );
        State.PT( Pizb + 1., Tm, pComp );
        if ( State.E > 0. )
          Te = Tm;
        else
          Tb = Tm;
        }
      T = Tm;
      return 0;
      }
    if ( CalcPT == 2 && Param.Addr != &Pizb )
      {
//P насыщенного пара
      double Pe = Pizb;
      double Pb = Pizb;
      State.PT( Pe + 1., T, pComp );
      if ( State.E > 0. )
        {
        while ( State.E > 0. )
          {
          Pb = Pe;
          if ( Pe > 100. )
            return 0;
          Pe += 2.;
          State.PT( Pe + 1., T, pComp );
          }
        }
      else
        {
        while ( State.E <= 0. )
          {
          Pe = Pb;
          if ( Pb < 0. )
            return 0;
          Pb -= 0.2;
          State.PT( Pb + 1., T, pComp );
          }
        }
      double Pm = 0.5 * ( Pb + Pe );
      while ( Pe - Pb > 1e-6 )
        {
        Pm = 0.5 * ( Pb + Pe );
        State.PT( Pm + 1., T, pComp );
        if ( State.E > 0. )
          Pb = Pm;
        else
          Pe = Pm;
        }
      Pizb = Pm;
      return 0;
      }
    }
  Set_PTE( );
  //if ( SaveSt && Type != Out )
  //  {
  //  SaveSt = false;
  //  SavePTC( );
  //  }
  //if ( RestSt )
  //  {
  //  RestSt = false;
  //  RestPTC( );
  //  }
  return 0;
  }

//#include "ShowParams.h"

int CIO::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
	switch ( DataType )
		{
		case sp_Actions:
      pSD->NewAction = pSD->ОкноСоставов;
		return 1;
		}
  return 0;
  }

//
HBRUSH hFon_Src = CreateSolidBrush( RGB ( 0, 0, 255 )); 
HPEN hPen_Src = CreatePen( PS_SOLID, 1, RGB ( 255, 255, 255 )); 
//
HBRUSH hFon_Dst = CreateSolidBrush( RGB ( 0, 0, 0 )); 
HPEN hPen_Dst = CreatePen( PS_SOLID, 1, RGB ( 255, 255, 255 ));
//
HBRUSH hFon_Conn = CreateSolidBrush( RGB ( 0, 255, 0 )); 
HPEN hPen_Conn = CreatePen( PS_SOLID, 1, RGB ( 255, 255, 255 )); 
HPEN hPen_Conn3 = CreatePen( PS_SOLID, 3, RGB ( 255, 255, 255 )); 
//
HBRUSH hFon_NoConn = CreateSolidBrush( RGB ( 255, 0, 0 )); 
HPEN hPen_NoConn = CreatePen( PS_SOLID, 1, RGB ( 255, 255, 255 ));
//
HBRUSH hFon_Err = CreateSolidBrush( RGB ( 255, 255, 255 )); 
HPEN hPen_Err = CreatePen( PS_SOLID, 1, RGB ( 255, 0, 0 )); 
//
HBRUSH hFon_WorkAsSrc = CreateSolidBrush( RGB ( 255, 0, 255 )); 
HPEN hPen_WorkAsSrc = CreatePen( PS_SOLID, 3, RGB ( 255, 0, 255 )); 
//

HFONT hFont = NULL;

void CIO::DrawObj ( struct CDrawObjData * pDraw )
  {
	  if(g_bInstructor)
		  return;
  if ( hFont == NULL )
    {
    hFont = CreateFont (
      18,
      0,                // average character width
      0,           // angle of escapement
      0,          // base-line orientation angle
      FW_BOLD,              // font weight
      0,           // italic attribute option
      0,        // underline attribute option
      0,        // strikeout attribute option
      ANSI_CHARSET,          // character set identifier
      OUT_TT_ONLY_PRECIS,  // output precision
      CLIP_DEFAULT_PRECIS,    // clipping precision
      ANTIALIASED_QUALITY,          // output quality
      VARIABLE_PITCH,   // pitch and family
      "Tahoma"           // typeface name
      );
    }
  bool ErrParm = false;
  if ( Type == Out )
    {
    if ( T == 100. )
      ErrParm = true;
    if ( !ErrParm && Pizb == 0. )
      ErrParm = true;
    }
  int W = pDraw->rc.right - pDraw->rc.left;
  int H = pDraw->rc.bottom - pDraw->rc.top;
  int x0 = ( pDraw->rc.right + pDraw->rc.left ) / 2;
  int y0 = ( pDraw->rc.bottom + pDraw->rc.top ) / 2;
  HBRUSH hOldFon;
  HPEN hOldPen;
  hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFon_Dst );
  hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPen_Dst );
  switch ( Type )
    {
    case In:
      hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFon_Dst );
      hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPen_Dst );
    break;
    case Atm:
    case Out:
      hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFon_Src );
      hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPen_Src );
    break;
    case IO:
      if ( WorkAsSrc )
        {
        hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFon_WorkAsSrc );
        hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPen_WorkAsSrc );
        }
      else
        {
        hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFon_Conn );
        hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPen_Conn );
        }
    break;
    }
  //
  int w = 8;
  int h = 8;
  Rectangle ( pDraw->hDC, x0 - w, y0 - h, x0 + h, y0 + h ); 
  w = 16;
  h = 16;
  if ( ErrParm )
    {
    SelectObject ( pDraw->hDC, hPen_Err );
    SelectObject ( pDraw->hDC, GetStockObject ( NULL_BRUSH ) );
    Ellipse ( pDraw->hDC, x0 - w, y0 - h, x0 + h, y0 + h );
    }
  SelectObject ( pDraw->hDC, hOldFon );
  SelectObject ( pDraw->hDC, hOldPen );
  //
  //
  bool IsOut = Out_P || Out_T || Out_F_m3 || Out_F_kg;// || Out_F_Sol_kg;
  if ( IsOut )
    {
    char Txt[1024];
    int L = 0;
    double Val_P = 0.;
    double Val_T = 0.;
    double Val_F = 0.;
    if ( Out_T )
      {
      switch ( Type )
        {
        case In:
        case IO:
          Val_T = T_in;
        break;
        case Out:
          Val_T = T;
        break;
        }
      L += sprintf_s ( Txt+L, 1024 - L, "%5.1lf ",  Val_T );
      }
    if ( Out_P )
      {
      switch ( Type )
        {
        case In:
        case IO:
          Val_P = P_in - 1;
          break;
        case Out:
          Val_P = Pizb;
          break;
        }
      L += sprintf_s ( Txt+L, 1024 - L, "%5.2lf ",  Val_P );
      }
    if ( Out_F_m3 )
      {
      Val_F = Flow_m3_in_out;
      L += sprintf_s ( Txt+L, 1024 - L, "%5.1lf",  Val_F );
      Out_F_kg = false;
//    Out_F_Sol_kg = false;
      }
    if ( Out_F_kg )
      {
      Val_F = Flow_kg_in_out;
      L += sprintf_s ( Txt+L, 1024 - L, "%5.1lf",  Val_F );
//      Out_F_Sol_kg = false;
      }
    //if ( Out_F_Sol_kg && pSolid )
    //  {
    //  Val_F = Flow_mol_in_out * pSolid->C * pSolid->Mw;
    //  L += sprintf_s ( Txt, 255 - L, "%5.1lf",  Val_F );
    //  }
    COLORREF Col = SetTextColor	( pDraw->hDC, RGB ( 255, 0, 0 ));
    HFONT hFontOld = (HFONT)SelectObject ( pDraw->hDC, hFont );
    int Align = SetTextAlign ( pDraw->hDC, TA_LEFT | TA_BOTTOM ); 
    SIZE Size;
    GetTextExtentPoint32( pDraw->hDC, Txt, L, &Size );
    int x, y = y0;//pDraw->rc.bottom;
    if ( x0 < 600 )
      x = pDraw->rc.left - Size.cx - 10;
    else
      x = pDraw->rc.right + 5;
    //
    if ( Out_T )
      {
      L = sprintf_s ( Txt, 255, "%5.1lf ", Val_T );
      TextOut ( pDraw->hDC, x, y,	Txt, L );
      GetTextExtentPoint32( pDraw->hDC, Txt, L, &Size );
      x += Size.cx;
      }
    //
    if ( Out_P )
      {
      SetTextColor	( pDraw->hDC, RGB ( 255, 255, 0 ));
      L = sprintf_s ( Txt, sizeof(Txt), "%5.2lf ", Val_P );
      TextOut ( pDraw->hDC, x, y,	Txt, L );
      GetTextExtentPoint32( pDraw->hDC, Txt, L, &Size );
      x += Size.cx;
      }
    //
    if ( Out_F_m3 )
      {
      SetTextColor	( pDraw->hDC, RGB ( 0, 255, 0 ));
      L = sprintf_s ( Txt, sizeof(Txt), "%5.1lf", Val_F );
      TextOut ( pDraw->hDC, x, y,	Txt, L );
      }
    //
    if ( Out_F_kg )
      {
      SetTextColor	( pDraw->hDC, RGB ( 192, 192, 192 ));
      L = sprintf_s ( Txt, sizeof(Txt), "%5.1lf", Val_F );
      TextOut ( pDraw->hDC, x, y,	Txt, L );
      }
    //if ( Out_F_Sol_kg )
    //  {
    //  SetTextColor	( pDraw->hDC, RGB ( 192, 192, 0 ));
    //  L = sprintf_s ( Txt, sizeof(Txt), "%5.1lf", Val_F );
    //  TextOut ( pDraw->hDC, x, y,	Txt, L );
    //  }
    //
    SetTextColor	( pDraw->hDC, Col );
    SelectObject ( pDraw->hDC, hFontOld );
    SetTextAlign ( pDraw->hDC, Align ); 
    return ;
    }
  }

void CIO::AddResources (CGDIResourceMgr *pResMgr)
{
	AddBrush (pResMgr, BRUSH_FILL_SCR, RGB (0, 0, 255));
	AddBrush (pResMgr, BRUSH_FILL_DST, RGB (0, 0, 0));
	AddBrush (pResMgr, BRUSH_FILL_CONN, RGB (0, 255, 0));
	AddPen (pResMgr, PEN_SRC, RGB (255, 255, 255), 1);
	AddPen (pResMgr, PEN_DST, RGB (255, 255, 255), 1);
	AddPen (pResMgr, PEN_CONN, RGB (255, 255, 255), 1);
	AddPen (pResMgr, PEN_ERR, RGB (255, 0, 0), 1);
	AddFont (pResMgr, 0, RGB (255, 0, 0), 18, TRUE, _T("Tahoma"));
}

void CIO::DrawObject (struct CDrawObjData * pDraw, CGDIResourceMgr *pResMgr)
{
	if(g_bInstructor)
		return;
	HBRUSH hBrSrc = GetBrush (pResMgr, pDraw->nModelID, BRUSH_FILL_SCR);
	HBRUSH hBrDst = GetBrush (pResMgr, pDraw->nModelID, BRUSH_FILL_DST);
	HBRUSH hBrCon = GetBrush (pResMgr, pDraw->nModelID, BRUSH_FILL_CONN);
	HPEN hPenSrc = GetPen (pResMgr, pDraw->nModelID, PEN_SRC);
	HPEN hPenDst = GetPen (pResMgr, pDraw->nModelID, PEN_DST);
	HPEN hPenCon = GetPen (pResMgr, pDraw->nModelID, PEN_CONN);
	HPEN hPenErr = GetPen (pResMgr, pDraw->nModelID, PEN_ERR);
	HFONT hCurFont = GetFont (pResMgr, pDraw->nModelID, 0);

    SetMapMode (pDraw->hDC, MM_TEXT);
	bool bErrParm = false;
	if ( Type == Out )
	{
		if ( T == 100. )
			bErrParm = true;
		if ( !bErrParm && Pizb == 0. )
			bErrParm = true;
	}
	int W = pDraw->rc.right - pDraw->rc.left;
	int H = pDraw->rc.bottom - pDraw->rc.top;
	int x0 = (int)ROUNDTOINT((pDraw->rc.right + pDraw->rc.left) / 2);
	int y0 = (int)ROUNDTOINT((pDraw->rc.bottom + pDraw->rc.top) / 2);
	HBRUSH hOldFon;
	HPEN hOldPen;
	hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hBrDst );
	hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPenDst );
	switch ( Type )
	{
		case In:
			hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hBrDst );
			hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPenDst );
			break;
		case Atm:
		case Out:
			hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hBrSrc );
			hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPenSrc );
			break;
		case IO:
			if ( WorkAsSrc )
			{
				hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFon_WorkAsSrc );
				hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPenSrc );
			}
			else
			{
				hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hBrCon );
				hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPenCon );
			}
			break;
	}
#ifdef NEW_DRAW_LOGIC
	bool bVert = H > W;
	double dMargin = bVert ? W / 5.0 : H / 5.0;
	int nMargin = (int)ROUNDTOINT(dMargin);
	int nSize = bVert ? (int)ROUNDTOINT((W - 2 * nMargin) / 2) : (int)ROUNDTOINT((H - 2 * nMargin) / 2);
	if ( bVert )
		x0 --;
	else
		y0 --;
	Rectangle ( pDraw->hDC, x0 - nSize, y0 - nSize, x0 + nSize, y0 + nSize ); 
	if ( bErrParm )
	{
		nSize *= 2;
		SelectObject ( pDraw->hDC, hPenErr );
		SelectObject ( pDraw->hDC, GetStockObject ( NULL_BRUSH ) );
		Ellipse ( pDraw->hDC, x0 - nSize, y0 - nSize, x0 + nSize, y0 + nSize );
	}
#else
	int w = 8;
	int h = 8;
	Rectangle ( pDraw->hDC, x0 - w, y0 - h, x0 + h, y0 + h ); 
	w = 16;
	h = 16;
	if ( bErrParm )
	{
		SelectObject ( pDraw->hDC, hPenErr );
		SelectObject ( pDraw->hDC, GetStockObject ( NULL_BRUSH ) );
		Ellipse ( pDraw->hDC, x0 - w, y0 - h, x0 + h, y0 + h );
	}
#endif // NEW_DRAW_LOGIC
	SelectObject ( pDraw->hDC, hOldFon );
	SelectObject ( pDraw->hDC, hOldPen );
	bool IsOut = Out_P || Out_T || Out_F_m3 || Out_F_kg;
	if ( IsOut )
	{
		char Txt[256];
		int L = 0;
		double Val_P = 0.;
		double Val_T = 0.;
		double Val_F = 0.;
		if ( Out_T )
		{
			switch ( Type )
			{
				case In:
				case IO:
					Val_T = T_in;
					break;
				case Out:
					Val_T = T;
					break;
			}
			L += sprintf_s ( Txt, 255 - L, "%5.1lf ",  Val_T );
		}
		if ( Out_P )
		  {
			switch ( Type )
			  {
				case In:
				case IO:
					Val_P = P_in - 1;
					break;
				case Out:
					Val_P = Pizb;
					break;
			  }
			L += sprintf_s ( Txt, 255 - L, "%5.2lf ",  Val_P );
      if ( Val_P > 1e10 )
        Val_P = 1e10;
      if ( Val_P < -1e10 )
        Val_P = -1e10;
		  }
		if ( Out_F_m3 )
		{
			Val_F = Flow_m3_in_out;
      if ( Val_F > 1e10 )
        Val_F = 1e10;
      if ( Val_F < -1e10 )
        Val_F = -1e10;
			L += sprintf_s ( Txt, 255 - L, "%5.1lf",  Val_F );
			Out_F_kg = false;
		}
		if ( Out_F_kg )
		{
			Val_F = Flow_kg_in_out;
      if ( Val_F > 1e10 )
        Val_F = 1e10;
      if ( Val_F < -1e10 )
        Val_F = -1e10;
			L += sprintf_s ( Txt, 255 - L, "%5.1lf",  Val_F );
		}
		if ( hCurFont )
		{
			COLORREF Col = SetTextColor	( pDraw->hDC, RGB ( 255, 0, 0 ));
			HFONT hFontOld = (HFONT)SelectObject ( pDraw->hDC, hCurFont );
			int Align = SetTextAlign ( pDraw->hDC, TA_LEFT | TA_BOTTOM ); 
			SIZE Size;
			GetTextExtentPoint32( pDraw->hDC, Txt, L, &Size );

			TEXTMETRIC tm;
			::GetTextMetrics (pDraw->hDC, &tm);

			int x, y = pDraw->rc.bottom;
			if ( x0 < 600 )
				x = pDraw->rc.left - Size.cx - 10;
			else
				x = pDraw->rc.right + 5;
			if ( Out_T )
			{
				L = sprintf_s ( Txt, 255, "%5.1lf ", Val_T );
				TextOut ( pDraw->hDC, x, y,	Txt, L );
				GetTextExtentPoint32( pDraw->hDC, Txt, L, &Size );
				x += Size.cx;
			}
			if ( Out_P )
			{
				SetTextColor	( pDraw->hDC, RGB ( 255, 255, 0 ));
				L = sprintf_s ( Txt, sizeof(Txt), "%5.2lf ", Val_P );
				TextOut ( pDraw->hDC, x, y,	Txt, L );
				GetTextExtentPoint32( pDraw->hDC, Txt, L, &Size );
				x += Size.cx;
			}
			if ( Out_F_m3 )
			{
				SetTextColor	( pDraw->hDC, RGB ( 0, 255, 0 ));
				L = sprintf_s ( Txt, sizeof(Txt), "%5.1lf", Val_F );
				TextOut ( pDraw->hDC, x, y,	Txt, L );
			}
			if ( Out_F_kg )
			{
				SetTextColor	( pDraw->hDC, RGB ( 192, 192, 192 ));
				L = sprintf_s ( Txt, sizeof(Txt), "%5.1lf", Val_F );
				TextOut ( pDraw->hDC, x, y,	Txt, L );
			}
			SetTextColor ( pDraw->hDC, Col );
			SelectObject ( pDraw->hDC, hFontOld );
			SetTextAlign ( pDraw->hDC, Align ); 
		}
	}

	BOOL bRes = ::DeleteObject (hBrSrc);
	bRes &= ::DeleteObject (hBrDst);
	bRes &= ::DeleteObject (hBrCon);
	bRes &= ::DeleteObject (hPenSrc);
	bRes &= ::DeleteObject (hPenDst);
	bRes &= ::DeleteObject (hPenCon);
	bRes &= ::DeleteObject (hPenErr);
	if ( !bRes )
		KKK();
}

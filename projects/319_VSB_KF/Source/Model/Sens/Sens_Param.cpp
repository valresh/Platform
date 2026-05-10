#include "stdafx.h"
#include "Sens.h"
#include "Err.h"
#include "CommProc.h"


_W::_W()
	{
  memset ( this, 0, sizeof ( _W ));
	}

bool CSens::МассовыеДоли = false;
bool CSens::UseAnalysComp[K_GAS];

CParams * CSens::pParams = NULL;
int CSens::kParams = -1;

bool CSens::GetParamsList( struct CParams ** ppParams, int * pkParams )
  {
  if ( pAnalyz )
    return false;
  if ( kParams <= 0 )
    return false;
  *ppParams = pParams;
  *pkParams = kParams;
  return true;
  }

extern char * AllMes;
int CSens::GetParams(  char * StrName )
	{
	#include "IO_Parms.h"
	#include "Points.h"
  if ( pGamma )
    pGamma->GetParams( this, true );
  char Name[128];

  if ( pAnalyz )
    {
    TAB("Анализаторы", 1 )
	if(!КривыеКипения)
    TAB("Значения", 1 )	
		for ( int n = 0; n < kAnalyzData; n++ )
		{
			if ( pAnalyz->Name[n] == NULL )
				continue;
            Sprintf( Name, "#%s", pAnalyz->Name[n] );
			if(strchr(pAnalyz->Name[n], '$'))
				continue;
			PARM(pAnalyz->C[n],Name )
		}
	ETAB
	if(!КривыеКипения)
    TAB("Поправки", 4 )
    for ( int n = 0; n < kAnalyzData; n++ )
      {
      Sprintf( Name, "К-т %d", n + 1 );
	  if(strchr(pAnalyz->Name[n], '$'))
		  continue;
      PARM(k_Analyz[n],Name )
      }
	ETAB
    if ( CoefTBP )
      {
      TAB("Поправки к кривой кипения", 4 )
      PARM(AddCoefTBP,"Поправка аддитивная")
      for ( int n = 0; n < kCoefTBP; n++ )
        {
        Sprintf( Name, "#kT для V = %d", n * 5 );
        PARM( CoefTBP[n], Name )
        }
      Char<512>Path;
#ifdef LINUX
      Path.Prt( "%sDATA/Equipment/Анализаторы/К-ты_%s.csv", PROJECT_ROOT, (char*)ObjName );
#else
      Path.Prt( "%sDATA\\Equipment\\Анализаторы\\К-ты_%s.csv", PROJECT_ROOT, (char*)ObjName );
#endif

      void * F = Fopen ( Path, "wt" );
      if ( F )
        {
        if ( AddCoefTBP )
          Fprintf( F, "#++++;\n" );
        else
          Fprintf( F, "#****;\n" );
        for ( int n = 0; n < kCoefTBP; n++ )
          {
          Fprintf( F, "%7.5lf;\n", CoefTBP[n] );
          }
        Fclose ( F );
        }
	 ETAB
      }
	 ETAB
    }
  //if ( MwAppr )
  //  {
  //  TAB("Аппроксимация плотности",1)
  //  Char<64>Name;
  //  for ( int n = 0; n < K_APPR; n++ )
  //    {
  //    Name.Prt( "Mw %d", n + 1 );
  //    PARM ( MwAppr[n], Name );
  //    }
  //  for ( int n = 0; n < K_APPR; n++ )
  //    {
  //    Name.Prt( "Ro %d", n + 1 );
  //    PARM ( RoAppr[n], Name );
  //    }
  //  }
  ACS_Params( "АСУ", this );

//	TAB("Модули",5)
//  IBaseModel::GetParams( StrName );
	return 0;
	}

int CSens::UpdateParam( CParams & Param )
{
	if ( Param.Addr == &_Value )
	{
		if(Type == Нет_типа)
			Val_old = Value = _Value;
		return 0;
	}
	if ( Param.Addr == &Test_V && Test_V )
	{
		pControlVal = new CAnalogControl ( "Показания", this, &_Value, &V_test_nom, &V_test_delta );
		V_test_nom = _Value;
		V_test_delta = 0.05 * V_test_nom;
		return 0;
	}
	static bool МассовыеДоли;
	static bool КривыеКипения;
	//if ( MwAppr )
	//  {
	//  for ( int n = 0; n < K_APPR; n++ )
	//    {
	//    if ( MwAppr[n] > 0. )
	//      LogMwAppr[n] = log ( MwAppr[n] );
	//    }
	//  }
	return 0;
}

//#include "ShowParams.h"

int CSens::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
	switch ( DataType )
		{
		case sp_Actions:
		  if ( GetAsyncKeyState( VK_CONTROL ) & 0x8000 )
        {
        Tau = 0.01;
        }
		return 1;
		}
  return 0;
  }

HFONT hFont = NULL;
//
HBRUSH hFon_Sgn = CreateSolidBrush( RGB ( 255, 255, 0 )); 
HPEN hPen_Sgn = CreatePen( PS_SOLID, 1, RGB ( 255, 255, 0 )); 
//
HBRUSH hFon_Blk = CreateSolidBrush( RGB ( 255, 0, 0 )); 
HPEN hPen_Blk = CreatePen( PS_SOLID, 3, RGB ( 255, 0, 255 ));
//
HPEN PenMin = CreatePen( PS_SOLID, 6, RGB( 255, 255, 0 ));
HPEN PenMax = CreatePen( PS_SOLID, 6, RGB( 0, 255, 255 ));


void CSens::DrawObj ( struct CDrawObjData * pDraw )
  {
  //HPEN hPenOld = (HPEN)SelectObject( pDraw->hDC, hPen );
  //MoveToEx(pDraw->hDC, pDraw->rc.left, pDraw->rc.top, NULL );
  //LineTo(pDraw->hDC, pDraw->rc.right, pDraw->rc.top );
  //LineTo(pDraw->hDC, pDraw->rc.right, pDraw->rc.bottom );
  //LineTo(pDraw->hDC, pDraw->rc.left, pDraw->rc.bottom );
  //LineTo(pDraw->hDC, pDraw->rc.left, pDraw->rc.top );
  //SelectObject( pDraw->hDC, hPenOld );
  if(g_bInstructor)
	  return;
  if ( Анализатор )
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
      "Arial"           // typeface name
      );
    }
  int x0 = ( pDraw->rc.right + pDraw->rc.left ) / 2;
  int y0 = ( pDraw->rc.bottom + pDraw->rc.top ) / 2;
  double V = _Value;
  HFONT hFontOld = 	(HFONT)SelectObject ( pDraw->hDC, hFont );
  int Align = SetTextAlign ( pDraw->hDC, TA_TOP|TA_CENTER ); 
  char Txt[256];
  COLORREF Col;
  int Len;
  switch ( ObjName[0] )
    {
    case 'T':
      Col = SetTextColor	( pDraw->hDC, RGB ( 255, 0, 0 ));
      if ( V > 5000 )
        Len = sprintf_s ( Txt, 255, ">" );
      else
        if ( V < -273 )
          Len = sprintf_s ( Txt, 255, "<", V );
        else
          Len = sprintf_s ( Txt, 255, "%5.1lf", V );
      break;
    case 'F':
      if ( V > 1e8 )
        Len = sprintf_s ( Txt, 255, ">" );
      else
        if ( V < -1e8 )
          Len = sprintf_s ( Txt, 255, "<", V );
        else
          Len = sprintf_s ( Txt, 255, "%5.1lf", V );
      Col = SetTextColor	( pDraw->hDC, RGB ( 0, 255, 0 ));
    break;
    case 'P':
      Col = SetTextColor	( pDraw->hDC, RGB ( 255, 255, 0 ));
      if ( V > 500000 )
        Len = sprintf_s ( Txt, 255, ">" );
      else
        if ( V < -10000. )
          Len = sprintf_s ( Txt, 255, "<", V );
        else
          Len = sprintf_s ( Txt, 255, "%5.2lf", V );
      break;
    default:
      Col = SetTextColor	( pDraw->hDC, RGB ( 255, 255, 255 ));
      if ( V > 1e10 )
        Len = sprintf_s ( Txt, 255, ">" );
      else
        if ( V < -1e10 )
          Len = sprintf_s ( Txt, 255, "<", V );
        else
          Len = sprintf_s ( Txt, 255, "%5.2lf", V );
      break;
    }
  // if ( GetAsyncKeyState(VK_CONTROL) & 0x8000 )
  //   {
  //   if ( MaxV > MinV )
  //     {
  //     if ( !InvalidMin && !InvalidMax )
  //       {
  //       Len = sprintf_s ( Txt, 255, "%5.2lf...%5.2lf", MinV, MaxV );
  //       }
  //     else
  //       {
  //       if ( InvalidMin )
  //         Len = sprintf_s ( Txt, 255, "< %5.2lf", MinV );
  //       else
  //         Len = sprintf_s ( Txt, 255, "> %5.2lf", MaxV );
  //       }
  //     }
  //   }
  TextOut ( pDraw->hDC, x0, y0,	Txt, Len );
  SetTextColor	( pDraw->hDC, Col );
  SelectObject ( pDraw->hDC, hFontOld );
  SetTextAlign ( pDraw->hDC, Align );
  if ( UseMin )
    {
    if ( V < MinV )
      {
      HPEN Old = (HPEN)SelectObject ( pDraw->hDC, PenMax );
      MoveToEx( pDraw->hDC, pDraw->rc.left-4, pDraw->rc.bottom + 7, NULL );
      LineTo( pDraw->hDC, pDraw->rc.right, pDraw->rc.bottom + 7 );
      SelectObject ( pDraw->hDC, Old );
      }
    }
  if ( UseMax )
    {
    if ( V > MaxV )
      {
      HPEN Old = (HPEN)SelectObject ( pDraw->hDC, PenMax );
      MoveToEx( pDraw->hDC, pDraw->rc.left-4, pDraw->rc.top - 6, NULL );
      LineTo( pDraw->hDC, pDraw->rc.right, pDraw->rc.top - 6 );
      SelectObject ( pDraw->hDC, Old );
      }
    }
  }


void CSens::AddResources (CGDIResourceMgr *pResMgr)
{
	AddFont (pResMgr, 0, RGB (255, 0, 0), 18, TRUE, _T("Tahoma"));
}

void CSens::DrawObject ( struct CDrawObjData * pDraw, CGDIResourceMgr *pResMgr)
{
	if(g_bInstructor)
		return;
	if ( Анализатор )
		return;
	HFONT hCurFont = GetFont (pResMgr, pDraw->nModelID, 0);
	if ( !hCurFont )
		return;
	int x0 = ( pDraw->rc.right + pDraw->rc.left ) / 2;
	int y0 = ( pDraw->rc.bottom + pDraw->rc.top ) / 2;
	HFONT hFontOld = (HFONT)SelectObject ( pDraw->hDC, hCurFont);
	int Align = SetTextAlign ( pDraw->hDC, TA_TOP|TA_CENTER ); 
	char Txt[256];
	COLORREF Col;
	int Len;
	double V = _Value;
  bool Show = true;
  if ( GetAsyncKeyState(VK_CONTROL) & 0x8000 )
    {
    if ( Nominal == 0. )
      Show = false;
    else
      V -= Nominal;
    }
  if ( GetAsyncKeyState(VK_SHIFT) & 0x8000 )
    {
    if ( Nominal == 0. )
      Show = false;
    else
      V = Nominal;
    }
  if ( !Show )
    return;
	switch ( ObjName[0] )
	  {
		case 'T':
			Col = SetTextColor	( pDraw->hDC, RGB ( 255, 0, 0 ));
			Len = sprintf_s ( Txt, 255, "%5.1lf", V );
			break;
		case 'F':
			Len = sprintf_s ( Txt, 255, "%5.1lf", V );
			Col = SetTextColor	( pDraw->hDC, RGB ( 0, 255, 0 ));
			break;
		case 'P':
			Col = SetTextColor	( pDraw->hDC, RGB ( 255, 255, 0 ));
			Len = sprintf_s ( Txt, 255, "%5.2lf", V );
			break;
		default:
			Col = SetTextColor	( pDraw->hDC, RGB ( 255, 255, 255 ));
			Len = sprintf_s ( Txt, 255, "%5.2lf", V );
			break;
	  }
	TextOut ( pDraw->hDC, x0, y0,	Txt, Len );
	SetTextColor ( pDraw->hDC, Col );
	SelectObject ( pDraw->hDC, hFontOld );
	SetTextAlign ( pDraw->hDC, Align );
//////////////////////////////////////////////////////////////////////////
  }

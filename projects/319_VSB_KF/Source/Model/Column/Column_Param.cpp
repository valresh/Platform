#include "stdafx.h"
#define DLL_COLUMN
#include "Column.h"
#include "Err.h"
#include "CommProc.h"
#include "HydroStruct.h"

#define PipeName pOP->pPool->Name                                        


#define ROUNDTOINT(number)	(int)floor(number + 0.5)

enum BRUSH
{
	BRUSH_FILL,
	BRUSH_LEVEL,
};

enum PEN
{
	PEN_RECT
};


_W_Column::_W_Column()
	{
  memset ( this, 0, sizeof ( _W_Column ));
  kGasOut = 1.;
  for ( int n = 0; n < MAX_IO; n++ )
    Press[n] = 1.;
	}

#include "IO_Parms.h"

void CColumn::SaveCoef( )
  {
  //TAB("Save",7)
  //Char<128>Name;
  //for ( int n = 0; n < MAX_ACS; n++ )
  //  {
  //  Name.Prt ( "A_ACS %d", n + 1 );
  //  PARM( A_ACS[n], Name );
  //  }
  }

int CColumn::GetParams( char * )
	{
  bool LongList = !ShortParamsList; 
  bool ShowProf = false;
   Char<128>Name;
  if ( CParams::SaveAllParams )
    {
    LongList = true; 
    ShowProf = true;
    }

#define PROF if ( ShowProf ) {
#define LONG if ( LongList ) {
#define EPROF }
#define ELONG }

#define SHOW
	#include "Points.h"
#undef SHOW
//  if ( pLevCtrl )
//    pLevCtrl->GetParams( NULL );
  Column.LongList = LongList; 
  Column.ShowProf = ShowProf;
  Column.GetParams( "Main" );
  if ( Column.pColAdditions )
    Column.pColAdditions->GetParams( this );
  Defect_Params(this);
  ACS_Params( "АСУ", this );
//
	return 0;
	}

int CColumn::UpdateParam( CParams & Param )
  {
//  if ( pLevCtrl )
//    pLevCtrl->UpdateParam( Param );
  //if ( Param.Addr == &DialCalcPress )
  //  {
  //  DialCalcPress = false;
  //  return 0;
  //  }
  if ( Param.Addr == &InitState )
    {
    InitState = false;
    Column.InitState( );
    return 0;
    }
  if ( Param.Addr == &WaterCol )
    {
    if ( WaterCol )
      Column.InitWaterCol( );
    return 0;
    }
  if ( Column.pColAdditions )
    Column.pColAdditions->UpdateParams( );
  return 0;
  }

//#include "ShowParams.h"

int CColumn::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
  switch ( DataType )
    {
    case sp_Actions:
      pSD->NewAction = pSD->РасчетДавлений;
      pSD->pModel = this;
    return 1;
    }
  return 0;
  }

int Numb ( char * Node )
  {
  while ( 1 )
    {
    char s = *Node;
    if ( s == 0 )
      return 0;
    if ( s >= '0' && s <= '9' )
      return atoi ( Node );
    Node++;
    }
  }

/*
CExtern_Pnt * CColumn::GetPoint( const char * PntName )
  {
  for ( int n = 0; n < kACS; n++ )
    {
    if ( strcmp ( ACS_Name[n], PntName ) == 0 )
      return ACS[n];
    }
  int N = Numb ( (char*)PntName + 1 );
  if ( N > kStage )
    return NULL;
  strcpy ( ACS_Name[kACS], PntName );
  char Type = PntName[0];
  TypeACS[kACS] = Type;
  nACS[kACS] = N;
  CAnalogD * pPnt = new CAnalogD ( this, (char*)PntName, "AO" );
  ACS[kACS] = pPnt;
  kACS++;
  return pPnt;
  }
*/
//( . )
HBRUSH hFon = CreateSolidBrush( RGB ( 142, 142, 142 )); 
HBRUSH hLev = CreateSolidBrush( RGB ( 192, 0, 0 )); 
HPEN hPen = CreatePen( PS_SOLID, 1, RGB ( 0, 0, 255 )); 
void CColumn::DrawObj ( struct CDrawObjData * pDraw )
{
	if(g_bInstructor)
		return;
	//DrawFixed ( pDraw );
	HBRUSH hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFon );
	HPEN hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPen );
	int W = pDraw->rc.right - pDraw->rc.left;
	int H = (pDraw->rc.bottom - pDraw->rc.top) / 2;
	int w = W / 10;
	if ( w < 20 )
	w = 20;
	int h = H - H / 5; 
	int x0 = pDraw->rc.right - 5 - w;
	int y0 = pDraw->rc.top + H ;
	double dL = 1. - L_куб * 0.01;
	if ( dL < 0. )
	  dL = 0.;
	int dhl_1 = (int)( dL * h ); 
	Rectangle ( pDraw->hDC, x0, y0, x0 + w, y0 + h ); 
	SelectObject ( pDraw->hDC, hLev );
	Rectangle ( pDraw->hDC, x0, y0 + dhl_1, x0 + w, y0 + h ); 
	SelectObject ( pDraw->hDC, hOldFon );
	SelectObject ( pDraw->hDC, hOldPen );
}

void CColumn::AddResources (CGDIResourceMgr *pResMgr)
{
	AddBrush (pResMgr, BRUSH_FILL, RGB (112, 146, 190));
	AddBrush (pResMgr, BRUSH_LEVEL, RGB (192, 0, 0));
	AddPen (pResMgr, PEN_RECT, RGB (71, 109, 160), 1);
}

void CColumn::DrawObject (struct CDrawObjData *pDraw, CGDIResourceMgr *pResMgr)
{
	if(g_bInstructor)
		return;
	HBRUSH hBrFill = GetBrush (pResMgr, pDraw->nModelID, BRUSH_FILL);
	HBRUSH hBrRed = GetBrush (pResMgr, pDraw->nModelID, BRUSH_LEVEL);
	HPEN hPen = GetPen (pResMgr, pDraw->nModelID, PEN_RECT);
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
		double dL = 1. - L_куб * 0.01;
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
	bRes &= ::DeleteObject (hPen);
	if ( !bRes )
		KKK();
}
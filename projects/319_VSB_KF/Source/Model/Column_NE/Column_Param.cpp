#include "stdafx.h"
#define DLL_COLUMN
#include "Column_NE.h"
#include "Err.h"
#include "CommProc.h"
#include "HydroStruct.h"

#define ROUNDTOINT(number)	(int)floor(number + 0.5)
#define PipeName pOP->pPool->Name

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
//  kGasOut = 1.;
  for ( int n = 0; n < MAX_IO; n++ )
    Press[n] = 1.;
	}

#include "IO_Parms.h"


int CColumn_NE::GetParams( char * )
	{
  Char<128>Name;
  #define LIST_PARAMS
  #include "Points.h"
  if ( CParams::ReadProps )
    return 0;
  if ( Column.pGammaCommon )
    Column.pGammaCommon->GetParams( this, true );
  if ( Column.pGammaAbs )
    Column.pGammaAbs->GetParams( this, true );
  if ( Column.pColumnGamma )
    Column.pColumnGamma->GetParams( this, true );
  if ( Column.pReactions )
    {
    int Res = Column.pStage[1]->pReactions->GetParamList ( 1, 0, this );
    if ( Res < 0 )
      {
      Column.pReactions->GetParams( this );
      }
    else
      {
      int NumbCall = 2;
      int Res = 0;
      while ( Res >= 0 )
        {
        for ( int s = 1; s <= kStage; s++ )
          {
          Res = Column.pStage[s]->pReactions->GetParamList ( NumbCall, s, this );
          if ( Res == 1 || Res < 0 )
            break;
          }
        NumbCall++;
        }
      }
    }
  if ( pCtrlLev )
    pCtrlLev->GetParams( this );
  //if ( Column.pColAdditions )
  //  Column.pColAdditions->GetParams( this );
  ACS_Params( "АСУ", this );
	return 0;
	}

int CColumn_NE::UpdateParam( CParams & Param )
  {
  if ( Param.Addr == &InitState )
    {
    InitState = false;
    Column.InitState( );
    return 0;
    }
  if ( Param.Addr == Air_in )
    {
    nAir = -1;
    for ( int n = 0; n < kIO; n++ )
      {
      if ( strstr ( pNodes[n].Name, Air_in ))
        {
        nAir = n;
        break;
        }
      }
    }

  if ( Param.Addr == &Column.NumbShow )
    {
    if ( Column.NumbShow < 1 )
      Column.NumbShow = kStage;
    if ( Column.NumbShow > kStage )
      Column.NumbShow = 1;
//    if ( !CParams::SaveAllParams && Column.pStage )
//      Column.pStage[NumbShow]->GetParams( NULL );
//????    Column.pParamsStage = Column.pStage[NumbShow];
typedef void (*tReplaceParams)( __int64 Frame, IBaseModel * pModel );
    tReplaceParams pReplaceParams = (tReplaceParams)ReplaceParams;
    (*pReplaceParams) ( ParamFrame, Column.pStage[Column.NumbShow] );
    return 0;
    }
  if ( Param.Addr == &Norm_Calc_C )
    {
    Norm_Calc_C = false;
    double S = 0.;
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( Column.IsComp[n] )
        S += Calc_C[n];
      }
    if ( S > 1e-3 )
      {
      double N = 100. / S;
      for ( int n = 0; n < LAST_GAS; n++ )
        {
        if ( Column.IsComp[n] )
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
  //if ( Column.pColAdditions )
  //  Column.pColAdditions->UpdateParams( );
  if ( pLevCtrl )
    {
    nRegCtrl = -1;
    for ( int n = 0; n < kIO; n++ )
      {
      if ( Src[n]->UseForCtrlLev )
        {
        nRegCtrl = n;
        break;
        }
      }
    }
  //if ( Param.Addr == &ColCommonGamma )
  //  {
  //  Column.pGammaCommon = (IGamma*)ColCommonGamma.GetExt( );
  //  if ( Column.pGammaCommon )
  //    Column.pGammaCommon ->Init ( this, NULL );
  //  return 0;
  //  }
  //if ( Param.Addr == &ColAbsGamma )
  //  {
  //  Column.pGammaAbs = (IGamma*)ColAbsGamma.GetExt( );
  //  if ( Column.pGammaAbs )
  //    Column.pGammaAbs ->Init ( this, NULL );
  //  return 0;
  //  }
  //if ( Param.Addr == &ColumnGamma )
  //  {
  //  Column.pColumnGamma = (IGamma*)ColumnGamma.GetExt( );
  //  if ( Column.pColumnGamma )
  //    Column.pColumnGamma ->Init ( this, NULL );
  //  return 0;
  //  }
  //if ( Param.Addr == &ColReactions )
  //  {
  //  Column.pReactions = (IReactions*)ColReactions.GetExt( );
  //  if ( Column.pReactions )
  //    Column.pReactions ->Init ( this );
  //  return 0;
  //  }
  
  if ( Column.pGammaCommon )
    {
    Column.pGammaCommon->UpdateParams( Param );
//    InitCommonGamma();
    }
  if ( Column.pGammaAbs )
    {
    Column.pGammaAbs->UpdateParams( Param );
//    InitAbsGamma();
    }
  if ( Column.pColumnGamma )
    {
    Column.pColumnGamma->UpdateParams( Param );
    //    InitAbsGamma();
    }
  if ( Column.pReactions )
    {
    Column.pReactions->UpdateParams( Param );
//    InitReactions();
    }
  if ( pCtrlLev )
    pCtrlLev->UpdateParams( Param );
  return 0;
  }

double CColumn_NE::TestCalcPress( double TC )
  {
  if ( Gas_Calc_C )
    {
    double S = 0.;
    double Pz = 0.;
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      if ( Column.IsComp[n] )
        {
        double X = Calc_C[n] / pGas[n]->Pz( TC );
        S += X;
        Pz += Calc_C[n] * Column.Henry[n];
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
    if ( Column.IsComp[n] )
      {
      S += Calc_C[n];
      Pz += Calc_C[n] * pGas[n]->Pz( TC ) * Column.Henry[n];
      }
    }
  if ( S > 1e-3 )
    {
    double N = 1. / S;
    return Pz * N; 
    }
  return 0.;
  }

double CColumn_NE::TestCalcTemp( double Pabs )
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


//#include "ShowParams.h"

int CColumn_NE::ShowParams( DWORD DataType, struct CShowData * pSD )
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
CExtern_Pnt * CColumn_NE::GetPoint( const char * PntName )
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
void CColumn_NE::DrawObj ( struct CDrawObjData * pDraw )
{
	//DrawFixed ( pDraw );
	if(g_bInstructor)
		return;
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

void CColumn_NE::AddResources (CGDIResourceMgr *pResMgr)
{
	AddBrush (pResMgr, BRUSH_FILL, RGB (112, 146, 190));
	AddBrush (pResMgr, BRUSH_LEVEL, RGB (192, 0, 0));
	AddPen (pResMgr, PEN_RECT, RGB (71, 109, 160), 1);
}

void CColumn_NE::DrawObject (struct CDrawObjData *pDraw, CGDIResourceMgr *pResMgr)
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
	bRes = ::DeleteObject (hBrRed);
	bRes = ::DeleteObject (hPen);
  }

#if 0
void CColumn_NE::NewCommonGamma()
  {
  return;
  Column.pGammaCommon = (IGamma*)ColCommonGamma.GetExt( );
  if ( Column.pGammaCommon == NULL )
    return;
  Column.pGammaCommon->Init ( this, NULL );
  Column.pGammaCommon->Name = "Общие параметры";
  Char<128>Name;
  for ( int n = 0; n < kStage; n++ )
    {
    Column.pStage[n]->pGammaCommon = (IGamma*)ColCommonGamma.GetExt( );
    Name.Prt ( "Gamma %s", Column.pStage[n]->Name );
    Column.pStage[n]->pGammaCommon->Name = Name;
    Column.pStage[n]->pGammaCommon->Init( this, Column.pGammaCommon );
    }
  }

void CColumn_NE::NewAbsGamma()
  {
  return;
  Column.pGammaAbs = (IGamma*)ColAbsGamma.GetExt( );
  if ( Column.pGammaAbs == NULL )
    return;
  Column.pGammaAbs->Init ( this, NULL );
  Column.pGammaAbs->Name = "Общие параметры";
  Char<128>Name;
  for ( int n = 1; n <= kStage; n++ )
    {
    Column.pStage[n]->pGammaAbs = (IGamma*)ColAbsGamma.GetExt( );
    Name.Prt ( "Gamma %s", Column.pStage[n]->Name );
    Column.pStage[n]->pGammaAbs->Name = Name;
    Column.pStage[n]->pGammaAbs->Init( this, Column.pGammaAbs );
    }
  }
#endif

void CColumn_NE::NewColumnGamma()
  {
  //Column.pColumnGamma = (IGamma*)ColumnGamma.GetExt( );
  //if ( Column.pColumnGamma == NULL )
  //  return;
  //Column.pColumnGamma->Init ( this, NULL );
  //for ( int n = 1; n <= kStage; n++ )
  //  {
  //  Column.pStage[n]->pColumnGamma = Column.pColumnGamma;
  //  }
  }

void CColumn_NE::NewReactions( char * Dll, char * Point )
  {
  Column.pReactions = (IReactions*)Get_Ext( Dll, Point );
  if ( Column.pReactions == NULL )
    return;
  Column.pReactions->Init ( this );
  Column.pReactions->Name = "Общие параметры";
  Char<128>Name;
  for ( int n = 1; n <= kStage; n++ )
    {
    Column.pStage[n]->pReactions = (IReactions*)Get_Ext( Dll, Point );
    Name.Prt ( "Reactions %s", Column.pStage[n]->Name );
    Column.pStage[n]->pReactions->Name = Name;
    Column.pStage[n]->pReactions->Init( this );//, Column.pReactions );
    }
  }

void CColumn_NE::InitCommonGamma()
  {
#if 0
  if ( Column.pGammaCommon == NULL )
    return;
  Column.pGammaCommon->Init ( this, NULL );
  for ( int n = 1; n <= kStage; n++ )
    {
    if ( Column.pStage[n]->pGammaCommon == NULL )
      continue;
    Column.pStage[n]->pGammaCommon->Init( this, Column.pGammaCommon );
    }
#endif
  }

void CColumn_NE::InitAbsGamma()
  {
#if 0
  if ( Column.pGammaAbs == NULL )
    return;
  Column.pGammaAbs->Init ( this, NULL );
  for ( int n = 1; n <= kStage; n++ )
    {
    if ( Column.pStage[n]->pGammaAbs == NULL )
      continue;
    Column.pStage[n]->pGammaAbs->Init( this, Column.pGammaAbs );
    }
#endif
  }

void CColumn_NE::InitColumnGamma()
  {
  if ( Column.pColumnGamma == NULL )
    return;
  Column.pColumnGamma->Init ( this, NULL );
  }

void CColumn_NE::InitReactions()
  {
  if ( Column.pReactions == NULL )
    return;
  Column.pReactions->Init ( this );
  for ( int n = 1; n <= kStage; n++ )
    {
    if ( Column.pStage[n]->pReactions == NULL )
      continue;
    Column.pStage[n]->pReactions->Init( this );//, Column.pReactions );
    }
  }


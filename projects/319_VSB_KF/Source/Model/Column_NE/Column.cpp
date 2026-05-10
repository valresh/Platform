#include "stdafx.h"
#define DLL_COLUMN
#include "Column_NE.h"
#include "Err.h"
#include "Split.h"
//#include "Data.h"
#include "SetData.h"
#include "Connect.h"
#include "Resume.h"
#include "HydroStruct.h"
#include "CommProc.h"
#include "H2SCO2.h"


FILE * F = NULL;
CColumn_NE::CColumn_NE( char * _ObjName ) :	IBaseModel ( _ObjName ) 
, Column ( "Колонна", this )
//, ColCommonGamma ( "Взаимодействие" )
//, ColAbsGamma ( "Абсорбция" )
//, ColumnGamma ( "Активности" )
//, ColReactions ( "Реакции" )
//, Gamma ( "Тип колонны", IGamma::ID, this )
#include "ACS_Constr.h"
#include "ACS.h"
	{
  UNIMODEL
	ObjName = _ObjName;
	#include "IO_Clear.h"
	#include "Points.h"
  pNodes = 0;
  kStage = 0;
  n1St_out = -1;
  nCub_out = -1;
  pFlow_Heat_in = NULL;
  pFlow_Heat_out = NULL;
  FlowQ = 0.;
  OmegaHeatStage = NULL;
  kHeatStage = 0;
  pLevCtrl = NULL;
  nRegCtrl = -1;
  Use_LevCtrl = false;
  HaveProperties = true;
   //for ( int n = 0; n < MAX_ACS; n++ )
  //  {
  //  A_ACS[n] = 1.;
  //  }
  kStep = 0;
  pCtrlLev = NULL;
  ppKapmans = NULL;
  kKarman = 0;
  Cmol_N1t = Cmol_N2t = Cmol_N1b = Cmol_N2b = 0.;
  pCreateColumnExtensions = NULL;
  CLEAR(Calc_C)
  Column.Calc_C = Calc_C;
  nAir = -1;
  Column.k_h_air = 1.;
  Column.k_l_air = 1.;
  Absorber = false;
  Desorber = false;
	}

CColumn_NE::~CColumn_NE()
	{
	}

// Карманы
int CompKarmans ( const void * p1, const void * p2 )
  {
  CKarman * k1 = *(CKarman **)p1;
  CKarman * k2 = *(CKarman **)p2;
  if ( k1->nStage < k2->nStage )
    return -1;
  if ( k1->nStage > k2->nStage )
    return 1;
  return Lstrcmp( k1->Name, k2->Name );
  }

int CColumn_NE::Init( int Regim )
	{
	int Res = 0;
  InitCol( );
//  Hydro_dPmax = NewArr(double,kIO);
  nST = NewArr(int,kIO);
	Src = NewArr(CExtIO*,kIO);
  EpsSrc = NewArr(double,kIO);
  Flow_kg = NewArr(double,kIO);
  if ( Absorber )
    {
    ADParams * pP = new ADParams;
    Column.pReactions = new MDEA_Ads ( pP );
    Column.pReactions ->Init ( this );
    for ( int n = 1; n <= kStage; n++ )
      {
      Column.pStage[n]->pReactions = new MDEA_Ads ( pP );
      Column.pStage[n]->pReactions->Init( this );
      }
    }
  if ( Desorber )
    {
    ADParams * pP = new ADParams;
    Column.pReactions = new MDEA_Ads ( pP );
    Column.pReactions ->Init ( this );
    for ( int n = 1; n <= kStage; n++ )
      {
      Column.pStage[n]->pReactions = new MDEA_Ads ( pP );
      Column.pStage[n]->pReactions->Init( this );
      }
    }
//////////////////////////////////////
  char Name[256];
  int nGasIO = 0;
  kKarman = 0;
	for ( int n = 0; n < kIO; n++ )
	  {
//    Hydro_dP[n] = 0.;
//    Hydro_dPmax[n] = 0.;
	  int nStage = pNodes[n].nStage;
    nST[n] = nStage;
    if ( nStage <= 0 )
      continue;
    sprintf_s( Name, 256, "Вход %d на т.%d %s", n + 1, nStage, ObjName );
    CExtIO * pSrc = new CExtIO( Name, this );
	  Src[n] = pSrc;
    pSrc->pPress = &Press[n];
    pSrc->SetComp( "Состав %s", Name );
		pSrc->pComp->Set( pN2 );
    pSrc->StaticFlow = true;
    pSrc->Omega = 10000.;
    pSrc->nStage = nStage;
    pSrc->nSrc = n;
    if ( pNodes[n].Type == 'G' )
      {
      pSrc->Gas = true;
      }
    else
      {
      pSrc->Gas = false;
      if ( pNodes[n].Type == 'L' && pSrc->nStage != kStage )
        {
        CStage_NE * pSt = Column.pStage[pSrc->nStage];
        CKarman * pKarman = new CKarman();
        pSrc->pKarman = pKarman;
        kKarman++;
        pKarman->pP = &pSt->P;
        sprintf_s( Name, 256, "т.%d %s", nStage, pNodes[n].Name );
        pKarman->Name = Name;
        pKarman->nStage = pSrc->nStage;
        pKarman->nNode = n;
        pKarman->pSrc = pSrc;
        }
      }
    Column.pStage[nStage]->Add ( pSrc );
	  }
  if ( kKarman > 0 )
    {
    ppKapmans = NewArr(CKarman*,kKarman);
    int nKarman = 0;
    for ( int n = 0; n < kIO ; n++ )
      {
      if ( Src[n]->pKarman )
        ppKapmans[nKarman++] = Src[n]->pKarman;
      }
    ASS(nKarman == kKarman)
    qsort( ppKapmans, kKarman, sizeof( CKarman * ), CompKarmans );
    }
//  if ( Use_LevCtrl )
//    pLevCtrl = new COmegaLevel ( "Регулятор уровня", this, &L_уровнемер );
  if ( HaveExtensions )
    {
    pCreateColumnExtensions = (ICreateColumnExtensions*)ReadExtension ( this );
    }
  else
    pCreateColumnExtensions = NULL;
//  NewCommonGamma();
//  NewAbsGamma();
//  NewColumnGamma();
//  NewReactions();
//  InitCommonGamma();
//  InitAbsGamma();
//  InitColumnGamma();
//  InitReactions();
  pCtrlLev = GetPropInt<IIntegral_Ctrl*>( ControlLev );
  if ( pCtrlLev )
    pCtrlLev->Init( this );
	return 0;
	}

int CColumn_NE::Step0( )
	{
  Column.k_GL = 1.;
  nAir = -1;
  if ( Air_in[0] )
    {
    for ( int n = 0; n < kIO; n++ )
      {
      if ( strstr ( pNodes[n].Name, Air_in ))
        {
        nAir = n;
        break;
        }
      }
    }
	return 0;
	}


int CColumn_NE::Step1( )
	{
  if ( Column.pReactions )
    {
    Column.pReactions ->Init ( this );
    for ( int n = 1; n <= kStage; n++ )
      {
      Column.pStage[n]->pReactions->Init( this );
      }
    }
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
/* /
  CLockParams Lock;
  GetParams( NULL );
  void * P = Fopen ( "Column_NE.csv", "wt" );
  char Tabs[8][128];
  int Pos = 0;
  for ( int n = 0; n < CParams::kParams; n++ )
    {
    CParams & Parm = CParams::Params[n];
    if ( Parm.Type == 'T' )
      {
      lstrcpy ( Tabs[Pos++], Parm.ParamName );
      for ( int n = 0; n < Pos; n++ )
        {
        Fprintf ( P, "%s", Tabs[n] );
        if ( n < Pos - 1 )
          Fprintf ( P, "/" );
        }
      Fprintf ( P, ";\n" );
      continue;
      }
    if ( Parm.Type == 'E' )
      {
      Pos--;
      continue;
      }
    if ( Parm.ParamName[0] == '#')
      Fprintf ( P, ";%s;\n", Parm.ParamName + 1 );
    else
      {
      char Txt[256];
      Parm.Out ( Txt );
      Fprintf ( P, ";%s;%s;\n", Parm.ParamName, Txt );
      }
    }
  Fclose ( P );
/**/
  return 0;
	}

int CColumn_NE::StepT ( double dt )
	{
  for ( int n = 0; n < Repeats; n++ )
    Calc ( dt );
  kStep++;
	return 0;
	}

#define PipeName pOP->pPool->Name                                        

int CColumn_NE::SetData( int TypeData, void * pData )
	{
  if ( TypeData == sd_SetParmsData )
    {
    Column.SetVariantParams();
    Column.SetРежим ( true );
    return 1;
    }

  if ( TypeData == sd_ParamsWillShow )
    {
// Вывод распределений
    CShowParmsOmStage * pShow = (CShowParmsOmStage*)pData;
    if ( pShow == NULL )
      return sd_ParamsWillShow;
    Char<128> Name;
#include "IO_Parms.h"
    for ( int n = 1; n <= kStage; n++ )
      {
      Name.Prt ( pShow->Format, n );
      CStage_NE * pSt = Column.pStage[n];
      int ID = pShow->ID;
      switch ( pShow->What )
        {
        case CShowParmsOmStage::X:
          PARM( pSt->X[ID],Name );
        break;
        case CShowParmsOmStage::Y:
          PARM( pSt->Y[ID],Name );
        break;
        case CShowParmsOmStage::Gamma:
          PARM( pSt->Gamma[ID],Name );
        break;
        }
       }
    return 0;
    }
 

  if ( TypeData == sd_ParamClicked )
    {
    CParamClicked * pPC = (CParamClicked*)pData;
    Char<128>Name;
    for ( int n = 0; n < kIO ; n++ )
      {
      Name.Prt ( "%s ", pNodes[n].Name );
      if ( strstr ( pPC->ParamName, Name ))
        {
        pPC->Command = CParamClicked::ВыделитьТрубу;
        lstrcpy ( pPC->Object, pNodes[n].PipeName );
        return 0;
        }
      }
    return 1;
    }
  if ( TypeData == sd_SetInitState )
    {
    for ( int n = 0; n < MAX_IO; n++ )
      Press[n] = 1.;
    Column.InitState();
    return 0;
    }
  if ( TypeData == sd_GetObjData )
    {
    CGetObjData * pG = (CGetObjData*)pData;
    pG->Addr = NULL;
    if ( strncmp ( pG->Name, "L_", 2 ) == 0 )
      {
      int n = atoi ( pG->Name + 2 );
      if ( n >= 1 && n <= kStage )
        {
        pG->Addr = &Column.pStage[n]->Level;
        return sd_GetObjData;
        }
      }
    return 0;
    }
  if ( TypeData == sd_GetOverGasSource )
    {
    *(double**)pData = &FlowQ; 
    return sd_GetOverGasSource;
    }
  if ( TypeData == sd_AddToTrends )
    {
#include "IO_Trends.h"
    PARM_D ( P_верха, "Давление верха(и)", 0. )
    PARM_D ( T_верха, "Температура верха", 0. )
    PARM_D ( P_низа, "Давление куба(и)", 0. )
    PARM_D ( T_низа, "Температура куба", 0. )
    PARM_D ( L_куб, "Уровень в кубе (истинный)", 0. )
    CTrends * pT = (CTrends*)pData;
    Char<256>VarName;
    CExtern_Pnt * pPnt = (CExtern_Pnt*)GetFirst( &Points );
    while ( pPnt )
      {
      if ( pPnt->Type[0] != 'A' )
        {
        pPnt = (CExtern_Pnt*)GetNext( );
        continue;
        }
      CAnalog * pA = (CAnalog*) pPnt;
      VarName.Prt( "%s.%s", ObjName, pA->Point );
      pT->Add( VarName, 'D', &pA->Value );
      pPnt = (CExtern_Pnt*)GetNext( );
      }
    return 0;
    }
  if ( TypeData == sd_SetLev_addr )
    {
    CColLevData * pLev = (CColLevData*)pData;
    pLev->pHCub = &Column.pStage[kStage]->h_st;
    pLev->pLevCub = &Column.pStage[kStage]->Level;
//    pLev->pT_max = &Column.T_max;
    return sd_SetLev_addr;
    }
  if ( TypeData == sd_GetComp )
    {
    return Column.SetData( TypeData, pData );
    }
	return 1;
	}

int CColumn_NE::SaveState( )
  {
//  return 1;
	S_CLASS("W_Column",_W_Column)
  if ( pLevCtrl )
    {
    pLevCtrl->SaveState( );
    }
  if ( pCtrlLev )
    pCtrlLev->SaveState( );
//  S_DATA("Hydro_dPmax",(8*kIO), Hydro_dPmax );
  for ( int n = 0; n < kIO; n++ )
    {
    if ( Src[n] )
      EpsSrc[n] = Src[n]->Eps;
    }
  S_DATA("EpsSrc", ( 8 * kIO ), EpsSrc );
  for ( int n = 0; n < kIO; n++ )
    {
    if ( Src[n] && Src[n]->pKarman )
      {
      ASS(pSRFile->WStruct( (char*)Src[n]->pKarman->Name, sizeof ( CKarman_W ), static_cast<CKarman_W*>(Src[n]->pKarman)));
      }
    }
  return 0;
  }

int CColumn_NE::RestoreState( char * StrName )
  {
  COPY_FIRST
	R_CLASS("W_Column",_W_Column)
  if ( pLevCtrl )
    {
    if ( pLevCtrl->RestoreState( StrName ) == 0 )
      return 0;
    }
  if ( pCtrlLev && pCtrlLev->RestoreState( StrName ) == 0 )
    return 0;
//  R_DATA("Hydro_dPmax",(8*kIO), Hydro_dPmax );
//  R_DATA("EpsSrc", ( 8 * kIO ), EpsSrc );
  if ( EpsSrc != NULL && lstrcmp ( StrName, "EpsSrc" ) == 0 )
    {
    ASS(pSRFile->RStruct( 8 * kIO, EpsSrc ));
    for ( int n = 0; n < kIO; n++ )
      {
      if ( Src[n] )
        Src[n]->Eps = EpsSrc[n];
      }
    return 0;
    }
  for ( int n = 0; n < kIO; n++ )
    {
    if ( Src[n]->pKarman && Src[n]->pKarman->Name == StrName )
      {
      ASS(pSRFile->RStruct( sizeof ( CKarman_W ), static_cast<CKarman_W*>(Src[n]->pKarman)));
      return 0;
      }
    }
  return 1;
  }


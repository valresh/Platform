#include "stdafx.h"
#define DLL_COLUMN
#include "Column.h"
#include "Err.h"
#include "Split.h"
//#include "Data.h"
#include "SetData.h"
#include "Connect.h"
#include "HydroStruct.h"

LPCSTR g_szHelpFileName = "Ректификационная колонна.pdf";

CColumn::CColumn( char * _ObjName ) :	IBaseModel ( _ObjName ) 
, Column ( "Колонна", this )
#include "ACS_Constr.h"
#include "ACS.h"
	{
  UNIMODEL
	ObjName = _ObjName;
    Model = "Column";
#define PROF
#define LONG
#define EPROF
#define ELONG
	#include "IO_Clear.h"
	#include "Points.h"
	#include "Column_Defects.h"
#undef PROF
#undef LONG
#undef EPROF
#undef ELONG
  pNodes = 0;
  kStage = 0;
  n1St = -1; 
  GasColumn = false;
  nGasNode_x = -1;
  nGasNode_1 = -1;
  pFlow_Heat_in = NULL;
  pFlow_Heat_out = NULL;
  pLevCtrl = NULL;
  FlowQ = 0.;
  OmegaHeatStage = NULL;
  kHeatStage = 0;
  //for ( int n = 0; n < MAX_ACS; n++ )
  //  {
  //  A_ACS[n] = 1.;
  //  }
 	}                         

CColumn::~CColumn()
	{
	}


void CColumn::InitCol( bool WaterCol )
  {
  // Опции;параметры опций;
  Char<256>Path;
  Path.Prt ( "%sDATA\\Equipment\\%s.csv", PROJECT_ROOT, ObjName.Str );
  CSplit<6,32> Data;
  bool NH3Col = false;
  bool Azeotrope = false;
  if ( Data.Open( Path ) )
    {
    while ( Data.Next( ))
      {
      if ( strcmp ( Data.Cols[0], "Вода" ) == 0 )
        {
        WaterCol = true;
        break;
        }
      if ( strcmp ( Data.Cols[0], "Азеотроп" ) == 0 )
        {
        Azeotrope = true;
        break;
        }
      }
    Data.Close();
    }
////////////////////////////
  Column.InitCol( kStage, WaterCol, NH3Col, Azeotrope );
// Чтение размеров
// Размер;n1;n2;D;H; - размеры тарелок
  if ( Data.Open( Path ) )
    {
    while ( Data.Next( ))
      {
      if ( strcmp ( Data.Cols[0], "Размер" ) == 0 )
        {
        int n1 = atoi ( Data.Cols[1] );
        int n2 = atoi ( Data.Cols[2] );
        ASS( n1 >= 1 && n1 <= n2 )
        ASS( n1 <= kStage )
        double D = atof ( Data.Cols[3] ) * 1e-3;
        double H = atof ( Data.Cols[4] ) * 1e-3;
        double h_st = atof ( Data.Cols[5] ) * 1e-3;
        double S = _PI_4 * D * D;
        double V = S * H * 0.95;
        for ( int n = n1; n <= n2; n++ )
          {
          Column.pStage[n]->Real_Vol_Stage = V;
          Column.pStage[n]->Real_S_Stage = S;
          Column.pStage[n]->Real_H_Stage = H;
          Column.pStage[n]->h_st = h_st;
          }
        continue;
        }
      if ( strcmp ( Data.Cols[0], "Тип входа" ) == 0 )
        {
        bool OK = false;
        for ( int n = 0; n < kIO; n++ )
          {
          if ( strcmp ( pNodes[n].Name, Data.Cols[1] ) == 0 )
            {
            if ( Data.Cols[2][0] == 'г' || Data.Cols[2][0] == 'Г' )
              {
              OK = true;
              pNodes[n].Type = 'G';
              break;
              }
            if ( Data.Cols[2][0] == 'ж' || Data.Cols[2][0] == 'Ж' )
              {
              OK = true;
              pNodes[n].Type = 'L';
              break;
              }
            ModMsg ( "В %s задан неправильный тип входа '%s'", ObjName.Str, Data.Cols[2] );
            }
          }
        if ( !OK )
          {
          ModMsg ( "В %s задано неправильное название входа '%s'", ObjName.Str, Data.Cols[1] );
          }
        continue;
        }
      if ( strcmp ( Data.Cols[0], "Вода" ) == 0 )
        {
        continue;
        }
      if ( strcmp ( Data.Cols[0], "Азеотроп" ) == 0 )
        {
        continue;
        }
      if ( strcmp ( Data.Cols[0], "Поглощение" ) == 0 )
        {
        Column.CalcAdsDes = true;
        bool OK = false;
        if ( strcmpi ( Data.Cols[1], "H2S" ) == 0 )
          {
          Column.AdsType = AdsH2S_MEA;
          Column.pAdsGas_1 = new CGamma();
          Column.pAdsGas_1->G_GAS = G_H2S;
          Column.pAdsGas_1->G_ABS = G_MDEA;
          OK = true;
          }
        if ( strcmpi ( Data.Cols[1], "CO2" ) == 0 )
          {
          Column.AdsType = AdsCO2_MEA;
          Column.pAdsGas_1 = new CGamma();
          Column.pAdsGas_1->G_GAS = G_CO2;
          Column.pAdsGas_1->G_ABS = G_MDEA;
          OK = true;
          }
        if ( strcmpi ( Data.Cols[1], "CO+CO2+Карсол" ) == 0 )
          {
          Column.AdsType = AdsCO2_K2CO3;
          Column.pAdsGas_1 = new CGamma();
//          Column.pAdsGas_2 = new CGamma();
          Column.pAdsGas_1->G_GAS = G_CO2;
          Column.pAdsGas_1->G_ABS = G_K2CO3;
//          Column.pAdsGas_2->G_GAS = G_CO;
//          Column.pAdsGas_2->G_ABS = G_K2CO3;
          OK = true;
          }
        if ( !OK )
          {
          ModMsg ( "В '%s' Неправильно задан тип поглощаемого газа '%s'", ObjName.Str, Data.Cols[1] );
          continue;
          }
        continue;
        }

      if ( strcmp ( Data.Cols[0], "Расширение" ) == 0 )
        {
        char * Name = Data.Cols[1];
        char * DLL = Data.Cols[2];
        char * Params = Data.Cols[3];
        I_Addition * pAdd = LoadAddition( DLL, Name );
        if ( pAdd == NULL )
          {
          ModMsg ( "Не загружено расширение '%s'(%s)", Name, DLL );
          continue;
          }
        if ( Column.pColAdditions == NULL )
          {
          Column.pColAdditions = new I_ColAdditionList();
          Column.pColAdditions->Init();
          }
        Column.pColAdditions->Add( pAdd );
        continue;
        }
      if ( strcmp ( Data.Cols[0], "Нагреватель" ) == 0 )
        {
        int Numb = atoi ( Data.Cols[1] );        
        int n_from = atoi ( Data.Cols[2] );
        int n_to = atoi ( Data.Cols[3] );
        ASS( Numb >= 1 && Numb <= 3 )
          Numb--;
        Char<32> Name;
        Name.Prt( "Вход нагрева %d", Numb + 1 );
        Column.pHeat_In[Numb] = new CExtHeat( Name, this );
        Column.pHeat_In[Numb]->nGroup = Numb;
        Column.pHeat_In[Numb]->KS = KSHeatStage[Numb];
        Name.Prt( "Выход нагрева %d", Numb + 1 );
        Column.pHeat_Out[Numb] = new CExtHeat( Name, this );
        Column.pHeat_Out[Numb]->nGroup = Numb;
        CExtHeat * pPrevHeat = Column.pHeat_In[Numb];
        CExtHeat * pH = NULL;
        if ( n_from <= n_to )
          {
          for ( int n = n_from; n <= n_to; n++ )
            { 
            Name.Prt( "Нагрев %d т. %d", Numb + 1, n );
            pH = new CExtHeat( Name, this );
            pPrevHeat->pNextHeat = pH;
            pH->pPrevHeat = pPrevHeat;
            pPrevHeat = pH;
            pH->nGroup = Numb;
            pH->nStage = n;
            }
          pPrevHeat->pNextHeat = Column.pHeat_Out[Numb];
          Column.pHeat_Out[Numb]->pPrevHeat = pH;
          }
        else
          {
          for ( int n = n_from; n >= n_to; n-- )
            { 
            Name.Prt( "Нагрев т. %d", n );
            pH = new CExtHeat( Name, this );
            pPrevHeat->pNextHeat = pH;
            pH->pPrevHeat = pPrevHeat;
            pPrevHeat = pH;
            pH->nGroup = Numb;
            pH->nStage = n;
            }
          pPrevHeat->pNextHeat = Column.pHeat_Out[Numb];
          Column.pHeat_Out[Numb]->pPrevHeat = pH;
          }
        continue;
        }
      ASS(0) //- неизвестный параметр
      }
    Data.Close( );
    }
  }

int CColumn::Init( int Regim )
	{
	int Res = 0;
  bool Water = false;
  InitCol( Water );
	Omega = NewArr(double,kIO);
  Hydro_dPmax = NewArr(double,kIO);
  nST = NewArr(int,kIO);
	Src = NewArr(CExtIO*,kIO);
  Flow_kg = NewArr(double,kIO);
  char Name[256];
	for ( int n = 0; n < kIO; n++ )
	  {
	  Omega[n] = 10000.;
    Hydro_dP[n] = 0.;
    Hydro_dPmax[n] = 0.;
	  int nStage = pNodes[n].nStage;
    nST[n] = nStage;
    if ( nStage <= 0 )
      continue;
    sprintf_s( Name, 256, "Вход %d на т.%d %s", n + 1, nStage, ObjName.Str );
	  Src[n] = new CExtIO( Name, this );
    Src[n]->pPress = &Press[n];
    Src[n]->SetComp( "Состав %s", Name );
		Src[n]->pComp->Set( pN2 );
    if ( pNodes[n].Type == 'G' )
      {
      Src[n]->Gas = true;
      Src[n]->Lev_Feed = 2.;
      }
    else
      {
      ASS( pNodes[n].Type == 'L' )
      Src[n]->Gas = false;
      Src[n]->Lev_Feed = 0.;
      }
//    if ( lstrcmp ( pNodes[n].Name, "o_1" ) == 0 )
    if ( pNodes[n].OutGasTop || ( pNodes[n].Type == 'O' && nStage == 1 ))
      {
      Src[n]->Gas = true;
      Src[n]->Lev_Feed = 2.;
      }
    Column.pStage[nStage]->Add ( Src[n] );
	  }
	return 0;
	}

int CColumn::Step0( )
	{
 // Fsend ( "Опасные объекты.csv", "%s;Да;\n", ObjName );
  if ( WaterCol )
    Column.InitWaterCol( );
	return 0;
	}

#include "Resume.h"

int CColumn::Step1( )
	{
//  SysRecord ( "Column '%s'", (char*)ObjName );
  if ( Column.pAdsGas_1 )
    Column.pAdsGas_1->CalcCoef( );
//  if ( Column.pAdsGas_2 )
//    Column.pAdsGas_2->CalcCoef( );
  if ( Column.pColAdditions )
    Column.pColAdditions->InitAdditions( &kStage );
//
/*
  CLockParams Lock;
  GetParams( NULL );
  void * P = Fopen ( "Params.csv", "wt" );
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
      Fprintf ( P, ";%s;\n", Parm.ParamName );
    }
  Fclose ( P );
*/
  return 0;
	}

int CColumn::StepT ( double dt )
	{
  Calc ( dt );
	return 0;
	}


#define PipeName pOP->pPool->Name                                        

int CColumn::SetData( int TypeData, void * pData )
	{
  if ( TypeData == sd_ParamClicked )
    {
    CParamClicked * pPC = (CParamClicked*)pData;
    Char<128>Name;
    for ( int n = 0; n < kIO ; n++ )
      {
//      if ( pNodes[n].Type != 'I' && pNodes[n].Type != 'O' && pNodes[n].Type != 'G' )
//        continue;
      if ( strstr ( pPC->ParamName, pNodes[n].Name ))
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
      VarName.Prt( "%s.%s", ObjName.Str, pA->Point.Str );
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
  if (TypeData == sd_GetHelpPath)
	  {
		  lstrcpy((char*)pData, HELP_PATH);
		  lstrcat((char*)pData, g_szHelpFileName);
		  return 1;
	  }
	return 1;
	}

int CColumn::SaveState( )
  {
//  return 1;
	S_CLASS("W_Column",_W_Column)
  if ( pLevCtrl )
    {
    pLevCtrl->SaveState( );
    }
  return 0;
  }

int CColumn::RestoreState( char * StrName )
  {
  COPY_FIRST
	R_CLASS("W_Column",_W_Column)
  if ( pLevCtrl )
    {
    if ( pLevCtrl->RestoreState( StrName ) == 0 )
      return 0;
    }
  return 1;
  }

void I_ColAdditionList::Init( )
{
  pFirst = NULL;
  pLast = NULL;
}

void I_ColAdditionList::Add( I_Addition * pAdd )
{
  if ( pFirst == NULL )
    pFirst = pAdd;
  if ( pLast )
    pLast->pNext = pAdd;
  pLast = pAdd;
  pAdd->pNext = NULL;
}

void I_ColAdditionList::Gamma( void * pData, double P, double T, bool IsComp[K_GAS], double Cmol[K_GAS], double P_sat_id[K_GAS], double Gamma[K_GAS], double & dH_liq )
  {
  I_Addition * pAdd = pFirst;
  while ( pAdd )
    {
    pAdd->Gamma(  pData, P, T, IsComp, Cmol, P_sat_id, Gamma, dH_liq );
    pAdd = pAdd->pNext;
    }
  }

void I_ColAdditionList::InitAdditions( void * pData )
  {
  I_Addition * pAdd = pFirst;
  while ( pAdd )
    {
    pAdd->Init( pData );
    pAdd = pAdd->pNext;
    }
  }

void I_ColAdditionList::GetParams( IBaseModel * pMain )
  {
  I_Addition * pAdd = pFirst;
  while ( pAdd )
    {
    pAdd->GetParams( pMain );
    pAdd = pAdd->pNext;
    }
  }


void I_ColAdditionList::UpdateParams( )
  {
  I_Addition * pAdd = pFirst;
  while ( pAdd )
    {
    pAdd->UpdateParams( );
    pAdd = pAdd->pNext;
    }
  }

#include "stdafx.h"
#define DLL_BaseCol
#include "BaseCol_NE.h"
#include "math.h"
#include "Err.h"
#include "ACS_Def.h"
#include "Extensions.h"



#define STAGE_REGIM_2(Parm) \
  TAB(#Parm,1)\
      {\
      char ParmName[256];\
      for ( int n = 1; n <= nCub; n++ )\
        {\
        sprintf_s ( ParmName, sizeof(ParmName), "#%s-%s", pStage[n]->Name, #Parm );\
        PARM( pStage[n]->Parm, ParmName );\
        }\
      }\
  ETAB

#define STAGE_PARM(Parm) \
		TAB(#Parm,1)\
			{\
			char ParmName[256];\
			for ( int n = 1; n <= nCub; n++ )\
				{\
        sprintf_s ( ParmName, sizeof(ParmName), "%s-%s", pStage[n]->Name, #Parm );\
				PARM( pStage[n]->Parm, ParmName );\
				}\
      }\
    ETAB

#define STAGE_PARM_D(Parm,NameTab,Fmt,Def) \
		TAB(NameTab,1)\
			{\
			char ParmName[256];\
			for ( int n = 1; n <= nCub; n++ )\
				{\
        sprintf_s ( ParmName, sizeof(ParmName), Fmt, pStage[n]->Name );\
				PARM_D( pStage[n]->Parm, ParmName, Def );\
				}\
      }\
    ETAB

CBaseCol_NE::CBaseCol_NE( char * Name, IBaseModel * pMain ) : IBaseModel ( Name, pMain )
	{
#define NO_DEFECT
	#include "IO_Clear.h"
	#include "BaseCol_NE_Pnt.h"
//  T_min = 15.;
//  T_max = 400.;
  //P_min = 0.0010;
  //P_max = 20.;
	h_cub = 2.;
  CalcStageBalance = false;
  n_Comp_Balance = -1;
  n_Comp_Balance_1 = 0;
  n_Comp_Delete = -1;
  P_top = 1.;
	P_Top_Z = 0.;
	CLEAR( Henry );
	CLEAR( Alfa_Comp );
	IsComp[G_N2] = true;
	UseHydrostat = true;
	kStep = 0;
	Проблемы = 0;
	ЕстьПроблемы = 0;
  Model = "BaseCol";
  Model_Gas_Liq = 1;
  Omega_Top = 10000.;
//  HenryH2S = 1.;
//  UseCO2Appr = false;
//  CalcAdsDes = false;
//  kInertHeat = 0.;
  Разгерметизация_Верха = false;
//  Omega_Def = 1000.;
  Razgerm = false;
  OmegaRazgerm = 0.;
  CLEAR(pHeat_In)
  CLEAR(pHeat_Out)
//  AdsType = AdsNone;
  pGammaCommon = NULL;
  pGammaAbs = NULL;
  pReactions = NULL;
  pColumnGamma = NULL;
  Static_Calc_Mix = true;
  Режим_по_тарелкам = 0;
//  pAdsGas_1 = NULL;
//  pAdsGas_2 = NULL;
  NumbShow = 1;
  ShowРежим.Init( this );
  MesPress = 0;
  OldShowVariantParams = 0;
  Calc_C = NULL;
//  pColAdditions = NULL;
  k_h_air = 1.;
  k_l_air = 1.;
	}

CBaseCol_W::CBaseCol_W()
	{
	memset ( this, 0, sizeof ( CBaseCol_W ));
	}

CBaseCol_P::CBaseCol_P()
	{
	memset ( this, 0, sizeof ( CBaseCol_P ));
  Fast_T = false;
  Slow_Fast_T = 0.1;
  Cond_Level = true;
	}

//#include "Описание.h"

#define SET_REGIM(Parm,NameParam) \
  if ( Show )\
    {\
    if ( Numb++ == Режим_по_тарелкам )\
      {\
      for ( int n = 1; n <= nCub; n++ )\
        Режим[n] = pStage[n]->Parm;\
      return;\
      }\
    }\
  else\
    {\
    int L = lstrlen ( NameParam );\
    ASS( Pos + L + 2 < Len )\
    memcpy ( РежимNames + Pos, NameParam, L );\
    Pos += L;\
    РежимNames[Pos++] = ';';\
    }
  

static char РежимNames[512];
static int Pos = 0;
static int Len = 512;

void CBaseCol_NE::SetРежим ( bool Show )
  {
  if ( !Show && Pos > 0 )
    return;
  int Numb = 0;
  SET_REGIM(T_liq,"Температура жидкости")
  SET_REGIM(T_gas,"Температура газа")
  SET_REGIM(_P,"Давление")
  SET_REGIM(Vout,"Поток газа с тарелки")
  SET_REGIM(L_Down,"Поток жидкости с тарелки")
  SET_REGIM(V_Cond,"Поток конденсации")
  SET_REGIM(P_Pz,"Неравновесность")
  SET_REGIM(M_liq,"Масса жидкости")
  SET_REGIM(Level,"Уровень жидкости на тарелке")
  SET_REGIM(dP_Stage,"Перепад на тарелке")
  SET_REGIM(Мерфри,"Эффективность")
  }

#undef PROF 
#undef LONG 
#undef EPROF
#undef ELONG
#define PROF if ( ShowProf ) {
#define LONG if ( LongList ) {
#define EPROF }
#define ELONG }

int CBaseCol_NE::GetParams( char * Type )
	{
		char ParmName[128];
		char Name[128];
  bool LongList = !ShortParamsList; 
  bool ShowProf = false;
  if ( CParams::SaveAllParams )
    {
    LongList = true; 
    ShowProf = true;
    }
	#include "IO_Parms.h"
  if ( lstrcmp ( Type, "Main" ) == 0 )
    {
    TAB("Параметры колонны",3)
#include "BaseCol_NE_Pnt.h"
    ETAB
    return 0;
    }
  if ( lstrcmp ( Type, "Режим по тарелкам" ) == 0 )
    {
    TAB("Режим по тарелкам",3)
    PARM_L( Режим_по_тарелкам, "Выводить", РежимNames );
    if ( !CParams::SaveAllParams )
      CParams::GetLast().pShowParam = &ShowРежим;
    Char<32>Name;
    for ( int n = 1; n <= nCub; n++ )
      {
      Name.Prt("#т.%d", n );
      PARM ( Режим[n], Name );
      if ( !CParams::SaveAllParams )
        CParams::GetLast().pShowParam = &ShowРежим;
      }
/*
*/
    ETAB
    return 0;
    }
//
/////////////////////////////////////
  if ( lstrcmp ( Type, "Параметры по тарелкам" ) == 0 )
    {
    TAB("Параметры по тарелкам",3)
    char ParmName[256];
    TAB("Тип тарелки",1)
    for ( int n = 1; n <= nCub; n++ )
      {
      if ( pStage[n]->FullStage ) break;
      sprintf_s ( ParmName, sizeof(ParmName), "Тип |%s", pStage[n]->Name );
      PARM_L( pStage[n]->StageType, ParmName, "Колпачковая;Насадка;Ситчатая;Глухая;Куб;" );
      }
    ETAB
    STAGE_PARM_D(Vol_Stage,"Объем", "V |%s", 2.)
    STAGE_PARM_D(S_Stage,"Площадь", "S |%s", 5.)
    STAGE_PARM_D(h_st,"Высота", "h |%s",0.05)
    STAGE_PARM_D(k_L,"Скорость слива", "Vслив |%s", 3000.)
    STAGE_PARM_D(Cp_korp, "Теплоемкость корпуса", "Cpkorp |%s", 2000.)
    STAGE_PARM_D( dP_nom,"Номинальный перепад на тарелке", "dP_nom |%s",0.01)
    STAGE_PARM_D( V_nom,"Номинальный поток через тарелку", "V_nom |%s",1000)
    TAB("Задания",5)
  //	STAGE_PARM_D(HenryStage,1.)
    STAGE_PARM_D(T_liq_Z,"Заданная температура", "Tz |%s", 0.)
    STAGE_PARM_D(Lev_Z,"Заданный уровень", "LevZ |%s", 0.)
/////
//    LONG
    STAGE_PARM_D(k_GL,"Теплообмен газ/жидкость", "Gas-Liq |%s", 1. )
    STAGE_PARM_D(k_Cool,"Охлаждение", "Qool |%s", 0.)
    STAGE_PARM_D(Alfa_Stage,"Скорость конденсации", "A |%s", 1.)
    STAGE_PARM_D(Hvap_Stage,"Теплота испарения", "Hvap |%s", 1.)
    STAGE_PARM_D(Cp_gas,"Теплоемкость газа", "Cpgas |%s", 30.)
    STAGE_PARM_D(Cp_liq,"Теплоемкость жидкости", "Cpliq |%s", 50.)
 //  STAGE_PARM_D(CalcTom3,"#Пересчет плотности", "Recalc |%s", false)
//    ELONG
    ETAB
    ETAB
    }
//------------------------------------------------
  if ( lstrcmp ( Type, "Контроль" ) == 0 )
    {
    TAB("Остановы",5)
      STAGE_REGIM_2(BreakPoint)
      STAGE_REGIM_2(StopSrc)
      STAGE_REGIM_2(StopLiq)
      STAGE_REGIM_2(StopGas)
      STAGE_REGIM_2(StopSloj)
      STAGE_REGIM_2(StopHeat)
      STAGE_REGIM_2(SpecStop1)
      STAGE_REGIM_2(SpecStop2)
    ETAB
	  TAB("Очистка",5)
    PARM( MustClear, "Очищать" );
	  for ( int n = 0; n < LAST_GAS; n++ )
		  {
		  Sprintf ( Name, "Чистка %s", pGas[n]->Name );
		  PARM( ClearComp[n], Name );
		  }
    ETAB
	  TAB("Баланс",5)
    PARM( WriteBalance, "#Вывести баланс")
	  PARM( Flow_Balance_mol, "#Потоки моль" );
	  PARM( Flow_Balance_kg, "#Потоки кг" );
	  PARM( H_Balance, "#Энтальпии" );
    TAB("Баланс потоков по тарелкам",5)
      for ( int n = 1; n <= nCub; n++ )
        {
        sprintf_s ( ParmName, sizeof(ParmName), "#%s баланс потоков,кмоль", pStage[n]->Name );
        PARM( Flow_Stage_Balance_mol[n], ParmName );
        }
      for ( int n = 1; n <= nCub; n++ )
        {
        sprintf_s ( ParmName, sizeof(ParmName), "#%s баланс потоков,кг", pStage[n]->Name );
        PARM( Flow_Stage_Balance_kg[n], ParmName );
        }
    ETAB
    TAB("Баланс энтальпий по тарелкам",5)
      for ( int n = 1; n <= nCub; n++ )
        {
        sprintf_s ( ParmName, sizeof(ParmName), "#%s баланс энтальпий", pStage[n]->Name );
        PARM( Heat_Stage_Balance_mol[n], ParmName );
        }
    ETAB
    ETAB
    TAB("Компоненты",5)
    for ( int n = 0; n < LAST_GAS; n++ )
	    {
      Sprintf ( Name, "#Баланс %s", pGas[n]->Name );
	    PARM( Comp_Balance[n], Name );
	    }
    PARM_L( n_Comp_Balance_1, "#Компонента для расчета баланса", PrefListComp );
    if ( n_Comp_Balance >= 0 )
	    {
	    char ParmName[256];
      sprintf_s ( ParmName, sizeof(ParmName), "//Баланс по %s", pGas[n_Comp_Balance]->Name );
	    TAB(ParmName,5)
	    for ( int n = 1; n <= nCub; n++ )
		    {
        sprintf_s ( ParmName, sizeof(ParmName), "#%s баланс", pStage[n]->Name );
		    PARM( pStage[n]->SummStageBalance, ParmName );
		    }
	    ETAB
	    }
    else
	    {
	    char ParmName[256];
	    TAB("Баланс по тарелкам",5)
	    for ( int n = 1; n <= nCub; n++ )
		    {
        sprintf_s ( ParmName, sizeof(ParmName), "#%s сумм. баланс", pStage[n]->Name );
        PARM( pStage[n]->SummStageBalance, ParmName );
		    }
      ETAB
	    }
    ETAB
    }
//
	return 0;
	}

int CBaseCol_NE::SaveState( )
  {
	S_CLASS("W",CBaseCol_W)
  return 0;
  }

int CBaseCol_NE::RestoreState( char * StrName )
  {
//  return 1;
  IGNORE
	R_CLASS("W",CBaseCol_W)
  return 1;
  }

int CBaseCol_NE::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
#include "IO_Parms.h"
#if 0
	switch ( DataType )
		{
		case sp_Params:
			int N = 0;
			CParams * Params = pSD->Params;
		  GetParams( N, Params );
//      if ( !CParams::SaveAllParams )
////////////////////////////////////////////////////////////////////
//??      if ( !CParams::SaveAllParams )
        {
  			TAB("Баланс",5)
	  		PARM( Flow_Balance_mol, "#Потоки моль" );
	  		PARM( Flow_Balance_kg, "#Потоки кг" );
		  	PARM( H_Balance, "#Энтальпии" );
			  TAB("Компоненты",5)
			  for ( int n = 0; n < LAST_GAS; n++ )
				  {
				  PARM( Comp_Balance[n], pGas[n]->Name );
				  }
			  PARM( n_Comp_Balance, "#Компонента для расчета баланса" );
			  if ( n_Comp_Balance >= 0 )
				  {
				  char ParmName[256];
				  TAB(pGas[n_Comp_Balance]->Name,5)
				  for ( int n = 1; n < nCub; n++ )
					  {
					  sprintf_s ( ParmName, sizeof(ParmName), "%s сумм. баланс", pStage[n]->Name );
					  PARM( pStage[n]->Summ_Bal_mol, ParmName );
					  }
				  }
			  else
				  {
				  char ParmName[256];
				  TAB("Баланс по тарелкам",5)
				  for ( int n = 1; n < nCub; n++ )
					  {
					  sprintf_s ( ParmName, sizeof(ParmName), "%s сумм. баланс", pStage[n]->Name );
            PARM( pStage[n]->Summ_Bal_mol, ParmName );
					  }
				  }
  			PARM( Проблемы, "#Проблемы" );
        }
//	    TAB("Модули",5)
			pSD->N = N;
		return 1;
		}
#endif
  return 0;
  }

int CBaseCol_NE::UpdateParam( CParams & Param )
  {
  int kCommon = 0;
  for ( int n = 1; n <= nCub; n++ )
    {
    if ( pCommonStage[n] )
      kCommon++;
    }
/*********
  if ( kCommon >= 2 )
    {
    __int64 Shft = -1;
    BYTE * A = NULL;
    int Len = 0;
    if ( Param.Type == 'X' )
      {
      int N = pParamsStage->Numb;
      if ( pCommonStage[N] )
        {
        IStageParam * pSP = (IStageParam*)Param.Addr;
        Shft = pSP->ShftParam[0];
        A = (BYTE*)(pParamsStage) + Shft;
        Len = pSP->Len();
        }
      }
    else
      {
      A = (BYTE*)Param.Addr;
      Len = Param.Len;
      for ( int n = 1; n <= nCub; n++ )
        {
        BYTE * pS = (BYTE *)pStage[n];
        if ( A >= pS && A < pS + sizeof ( CStage_NE ))
          {
          if ( pCommonStage[n] )
            Shft = A - pS;
          break;
          }
        }
      }
    if ( Shft >= 0 )
      {
      for ( int n = 1; n <= nCub; n++ )
        {
        if ( !pCommonStage[n] )
          continue;
        BYTE * pS = (BYTE *)pStage[n];
        if ( pS == (BYTE *)pParamsStage )
          continue;
        memmove ( pS + Shft, A, Len );
        }
      }
    }
**********/
  if ( Param.Addr == &n_Comp_Balance_1 )
    {
    int Old = n_Comp_Balance + 1;
    if ( n_Comp_Balance_1 < Old )
      {
      n_Comp_Balance = n_Comp_Balance_1 - 1;
      while ( n_Comp_Balance > -1 && !IsComp[n_Comp_Balance] )
        n_Comp_Balance_1--;
      return 0;
      }
    if ( n_Comp_Balance_1 > Old )
      {
      n_Comp_Balance = n_Comp_Balance_1 - 1;
      while ( n_Comp_Balance < LAST_GAS && !IsComp[n_Comp_Balance] )
        n_Comp_Balance_1++;
      return 0;
      }
    }
	return pMain->UpdateParam( Param );
  }

void CBaseCol_NE::DrawFixed ( CDrawObjData * pDraw )
  {
//
  static HPEN hPen = CreatePen( PS_SOLID, 1, RGB ( 0, 255, 255 )); 
  static HBRUSH hFon = (HBRUSH)GetStockObject( NULL_BRUSH ); 
	HPEN hOldPen = (HPEN)SelectObject ( pDraw->hDC, hPen );
	HBRUSH hOldFon = (HBRUSH)SelectObject ( pDraw->hDC, hFon );
	Rectangle ( pDraw->hDC, pDraw->rc.left-2, pDraw->rc.top-2, pDraw->rc.right+2, pDraw->rc.bottom+2 );
	Rectangle ( pDraw->hDC, pDraw->rc.left-4, pDraw->rc.top-4, pDraw->rc.right+4, pDraw->rc.bottom+4 );
	SelectObject ( pDraw->hDC, hOldPen );
	SelectObject ( pDraw->hDC, hOldFon );
  }

void CBaseCol_NE::DrawState ( CDrawObjData * pDraw )
	{
	if ( Проблемы )
		DrawFixed ( pDraw );
	}

int Numb ( const char * Node )
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
  return 0;
  }

bool Numb2 ( const char * Node, int & N1, int & N2 )
  {
  while ( 1 )
    {
    char s = *Node;
    if ( s == 0 )
      return false;
    if ( s >= '0' && s <= '9' )
      {
      N1 = atoi ( Node );
      break;
      }
    Node++;
    }
  while ( 1 )
    {
    char s = *Node;
    if ( s == 0 )
      return false;
    if ( s < '0' || s > '9' )
      break;
    Node++;
    }
  while ( 1 )
    {
    char s = *Node;
    if ( s == 0 )
      return false;
    if ( s >= '0' && s <= '9' )
      {
      N2 = atoi ( Node );
      return true;
      }
    Node++;
    }
  return false;
  }

char * KapmanName ( const char * Node )
  {
  if ( *Node != '_' )
    return NULL;
  return (char*)Node + 1;
  }

CExtern_Pnt * CBaseCol_NE::GetPoint( const char * PntName )
  {
  char ACS_Lett_1 = PntName[0];
  int N = -1;
  int M = 0;
  char * Name;
  char * Desc;
  eACS_Flag Type = eACS_No;
  double * pAddr = NULL;
  switch ( ACS_Lett_1 )
    {
    case 'A':
      {
      Name = "Абс. давление ";
      Desc = "Абс. давление газа на тарелке"; 
      Type = eACS_Pabs;
      N = Numb ( PntName + 1 );
      if ( N < 1 || N > nCub )
        return NULL;
      CStage_NE * pS = pStage[N];
      pAddr = &pS->P;
      }
    break;
    case 'P':
      {
      Name = "Давление ";
      Desc = "Давление газа на тарелке"; 
      Type = eACS_P;
      N = Numb ( PntName + 1 );
      if ( N < 1 || N > nCub )
        return NULL;
      CStage_NE * pS = pStage[N];
      pAddr = &pS->P;
      }
    break;
    case 'T':
      {
      Name = "Температура ";
      Desc = "Температура газа на тарелке"; 
      Type = eACS_T;
      N = Numb ( PntName + 1 );
      if ( N < 1 || N > nCub )
        return NULL;
      CStage_NE * pS = pStage[N];
      if ( PntName[1] == 'l')
        pAddr = &pS->T_liq;
      else
        pAddr = &pS->T_gas;
      }
    break;
    case 'D':
      {
// D_N1-N2
      Type = eACS_D;
      Name = "Перепад ";
      Desc = "Перепад между тарелками"; 
      if ( !Numb2 (  PntName + 1, N, M ))
        return NULL;
      if ( N < 1 || N > nCub )
        return NULL;
      if ( M < 1 || M > nCub )
        return NULL;
      pAddr = NULL;
      }
    break;
    case 'L':
      {
      Type = eACS_L;
      Name = "Уровень ";
      Desc = "Уровень жидкости в кармане"; 
      N = Numb ( PntName + 1 );
      pAddr = NULL;
      if ( N >= 1 && N < nCub )
        {
// L_<N тарелки>
        CStage_NE * pS = pStage[N];
        for ( int n = 0; n < pS->kSrc; n++ )
          {
          if ( pS->Src[n]->pKarman )
            {
            pAddr = &pS->Src[n]->pKarman->Level;
            break;
            }
          }
        break;
        }
// L_<Имя точки>
      char * KarmanName = KapmanName ( PntName + 2 );
      if ( KarmanName == NULL )
        return NULL;
      int L_PntName = lstrlen ( PntName + 2 );
      for ( int nS = 1; nS < nCub; nS++ )
        {
        CStage_NE * pS = pStage[nS];
        for ( int n = 0; n < pS->kSrc; n++ )
          {
          if ( pS->Src[n]->pKarman )
            {
            int L_Name = lstrlen ( pS->Src[n]->pKarman->Name );
            int d = L_Name - L_PntName;
            if ( d < 0 )
              continue;
            if ( lstrcmp ( pS->Src[n]->pKarman->Name + d, PntName + 2 ) == 0 )
              {
              N = nS;
              pAddr = &pS->Src[n]->pKarman->Level;
              break;
              }
            }
          }
        }
      if ( pAddr == NULL )
        return NULL;
      }
    break;
    default:
    return NULL;
    }
  if ( pAddr == NULL && ACS_Lett_1 !='D' )
    return NULL;
  Char<256> NamePnt;
  NamePnt.Prt( "%s т. %d|%s", Name, N, PntName );
  CAnalog * pPnt = (CAnalog*)pMain->FindPoint( (char*)NamePnt );
  if ( pPnt == NULL )
    {
    pPnt = new CAnalog( pMain, (char*)NamePnt, "AO", Desc );
    }
  pPnt->pSrcAddr = pAddr;
// [Type][Номер тарелки][Номер 2-ой тарелки/Номер кармана]
  DWORD Flag = ( M << 16 ) | ( N << 8 ) | Type;
  pPnt->Flag = Flag;
  return pPnt;
  }

void CBaseCol_NE::SetPoints( )
  {
   CAnalog * pPnt = (CAnalog*)pMain->pModel_Pnt;
  while ( pPnt )
    {
    DWORD Flag = pPnt->Flag;
    eACS_Flag Type = (eACS_Flag)(pPnt->Flag & 0xFF);
    switch ( Type )
      {
      case eACS_Pabs:
        *pPnt = *pPnt->pSrcAddr;
      break;
      case eACS_P:
        *pPnt = *pPnt->pSrcAddr - 1.;
        break;
      case eACS_T:
        *pPnt = *pPnt->pSrcAddr;
      break;
      case eACS_L:
        *pPnt = *pPnt->pSrcAddr * 100.;
      break;
      case eACS_D:
        {
        int N = ( Flag >> 8 ) & 0xFF;
        int M = ( Flag >>16 ) & 0xFF;
        CStage_NE * pS_N = pStage[N];
        CStage_NE * pS_M = pStage[M];
        *pPnt = pS_N->P - pS_M->P;
        }
      break;
      }
    pPnt = (CAnalog*)pPnt->pNext_Model_Pnt;
    }
  }

void CBaseCol_NE::CreateHeat ( int nFirstSt, int nLastSt, bool UpToDown )
  {
  if ( nFirstSt < 1 )
    return;
  if ( nLastSt > nCub )
    return;
  Char<128>Name;
  for ( int n = nFirstSt; n <= nLastSt; n++ )
    {
    Name.Prt( "Нагрев т. %d", n );
    CHeat * pStageHeat = new CHeat( Name, this );
    pStage[n]->pStageHeat = pStageHeat;
//    pStageHeat->Init( );
    if ( n != nLastSt )
      {
      pStage[n]->pStageHeat->pOut = NewArr(CFlow,1);
      }
    }
  if ( UpToDown )
    {
    for ( int n = nFirstSt; n < nLastSt; n++ )
      {
      pStage[n+1]->pStageHeat->pIn = pStage[n]->pStageHeat->pOut;
      }
    }
  else
    {
    for ( int n = nLastSt; n > nFirstSt; n-- )
      {
      pStage[n-1]->pStageHeat->pIn = pStage[n]->pStageHeat->pOut;
      }
    }
  }

void CBaseCol_NE::SetHeatQ ( int nFirstSt, int nLastSt, bool Use_Q_HeatZ, double Q_Z, double & k_Q_HeatZ, double KS, double & dQ )
  {
  double dQs = 0.;
  double Q_F = 0.;
  for ( int n = nFirstSt; n <= nLastSt; n++ )
    {
    pStage[n]->pStageHeat->Q_HeatZ = Q_Z * 3.6e6;
    pStage[n]->pStageHeat->KS = KS;
    pStage[n]->pStageHeat->Use_Q_HeatZ = Use_Q_HeatZ;
    pStage[n]->pStageHeat->k_Q_HeatZ = k_Q_HeatZ;
    dQs += pStage[n]->pStageHeat->dQ_Heat;
    Q_F += pStage[n]->pStageHeat->Q_Heat_F;
    }
  dQ = dQs / 3.6e6;
  if ( Use_Q_HeatZ )
    {
    k_Q_HeatZ = Q_Z * 3.6e6 / Q_F;
    }
  }

void CBaseCol_NE::SetHeatFlow ( int nFirstSt, int nLastSt, bool UpToDown, CFlow * pIn, CFlow * pOut )
  {
  if ( UpToDown )
    {
    if ( pStage[nFirstSt]->pStageHeat )
      pStage[nFirstSt]->pStageHeat->pIn = pIn;
    if ( pStage[nLastSt]->pStageHeat )
      pStage[nLastSt]->pStageHeat->pOut = pOut;
    }
  else
    {
    if ( pStage[nLastSt]->pStageHeat )
      pStage[nLastSt]->pStageHeat->pIn = pIn;
    if ( pStage[nFirstSt]->pStageHeat )
      pStage[nFirstSt]->pStageHeat->pOut = pOut;
    }
  }

void CBaseCol_NE::Utils_1( )
  {
	for ( int n = 0; n < LAST_GAS; n++ )
		{
    if ( Henry[n] < 1e-4 )
		  Henry[n] = 1.;
		}
	if ( n_Comp_Balance >= 0 )
		{
// Накопление на тарелках
		for ( int n = 1; n <= nCub; n++ )
			{
			CStage_NE * pS = pStage[n];
			pS->M_Bal_0 = pS->M_liq * pS->X[n_Comp_Balance] + 
										pS->M_gas * pS->Y[n_Comp_Balance];
			}
		}
	if ( MustClear )
		{
//    MustClear = false;
    for ( int c = 0; c < LAST_GAS; c++ )
      {
      if ( ClearComp[c] <= 0. )
        continue;
//  		  IsComp[c] = false;
      double N = 1. / ( 1. + ClearComp[c] );
		  for ( int n = 1; n <= nCub; n++ )
			  {
			  CStage_NE * pS = pStage[n];
			  pS->X[c] *= N;
			  pS->Y[c] *= N;
        ::Norm ( IsComp, pS->X );
	      ::Norm ( IsComp, pS->Y );
			  }
		  }
	  for ( int n = 1; n <= nCub; n++ )
		  {
		  CStage_NE * pS = pStage[n];
      ::Norm ( IsComp, pS->X );
		  }
    }
  if ( n_Comp_Delete >= 0 )
    {
    int c = n_Comp_Delete;
    IsComp[c] = false;
    for ( int n = 1; n <= nCub; n++ )
      {
      CStage_NE * pS = pStage[n];
      pS->X[c] = 0.;
      ::Norm ( IsComp, pS->X );
      pS->Y[c] = 0.;
      ::Norm ( IsComp, pS->Y );
      }
    }
  bool IsSrc = false;
  for ( int n = 1; n <= nCub; n++ )
    {
    CStage_NE * pS = pStage[n];
    for ( int s = 0; s < pS->kSrc; s++ )
      {
      CFlow * pF = pS->Src[s]->pFlow;
      if ( pF == NULL )
        continue;
      double F = pF->Flow_mol;
      if ( F < 0. )
        continue;
      if ( F >= Flow_src_min )
        {
        IsSrc = true;
        break;
        }
      }
    if ( IsSrc )
      break;
    }
  Work = IsSrc;
  }

void CBaseCol_NE::SetVariantParams()
  {
  if ( NumbShow < 1 || NumbShow > nCub )
    return;
  CStage_NE * pS = pStage[NumbShow];
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    if ( IsComp[n] )
      {
      switch ( ShowVariantParams )
        {
        case 0://"Состав жидкости
          {
          VariantParams[n] = pS->X[n] * 100.;
          }
        break;
        case 1://Состав газа
          {
          VariantParams[n] = pS->Y[n] * 100.;
          }
        break;
        case 2://Поток конденсации
          {
          VariantParams[n] = pS->Vc_Comp[n];
          }
        break;
        case 3://Тепло конденсации;"
          {
          VariantParams[n] = pS->h_cond[n];
          }
        break;
        case 4://Pz"
          {
          VariantParams[n] = Show_Press ( pS->P_z[n] + 1. );
          }
        break;
        case 5://PzX"
          {
          VariantParams[n] = Show_Press ( pS->P_PzX[n] + 1. );
          }
        break;
        case 6://PY"
          {
          VariantParams[n] = Show_Press ( pS->P_Y[n] + 1. );
          }
        break;
        case 7://PY/PzX"
          {
          if ( pS->P_PzX[n] > 0. )
            VariantParams[n] = pS->P_Y[n] / pS->P_PzX[n];
          }
        break;
        case 8://Lin;
          {
          if ( pS->pUp )
            VariantParams[n] = pS->pUp->L_Down * pS->pUp->X[n];
          else
            VariantParams[n] = 0.;
          }
        break;
        case 9://Lout;
          {
          VariantParams[n] = pS->L_Down * pS->X[n];
          }
        break;
        case 10://Vin;
          {
          if ( pS->pDown )
            VariantParams[n] = pS->pDown->Vout * pS->pDown->Y[n];
          else
            VariantParams[n] = 0.;
          }
        break;
        case 11://Vout;
          {
          if ( pS->Numb != 1 )
            VariantParams[n] = pS->Vout * pS->Y[n];
          else
            VariantParams[n] = 0.;
          }
        break;
        case 12://Для расчета
          {
          if ( Calc_C && ( OldShowVariantParams <= 1 ))
            Calc_C[n] = VariantParams[n];
          }
        break;
        }
      }
    }
  if ( ShowVariantParams == 12 )
    ShowVariantParams = OldShowVariantParams;
  else
    OldShowVariantParams = ShowVariantParams;
  }


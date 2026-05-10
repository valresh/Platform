#include "stdafx.h"
#define DLL_BaseCol
#include "BaseCol.h"
#include "math.h"
#include "Err.h"
#include "ACS_Def.h"

#define SET_REGIM(Parm,NameParam) \
		TAB(NameParam,1)\
			{\
			char ParmName[256];\
			for ( int n = 1; n <= nCub; n++ )\
				{\
        if ( pStage[n]->FullStage ) break;\
        sprintf_s ( ParmName, sizeof(ParmName), "#%s %s",NameParam, pStage[n]->Name );\
				PARM( pStage[n]->Parm, ParmName );\
				}\
      ETAB\
			}

#define STAGE_REGIM(Parm) \
		TAB(#Parm,1)\
			{\
			char ParmName[256];\
			for ( int n = 1; n <= nCub; n++ )\
				{\
        if ( pStage[n]->FullStage ) break;\
        sprintf_s ( ParmName, sizeof(ParmName), "#%s-%s", pStage[n]->Name, #Parm );\
				PARM( pStage[n]->Parm, ParmName );\
				}\
      if ( Vol.Work )\
        {\
        sprintf_s ( ParmName, sizeof(ParmName), "#%s-%s", Vol.Name, #Parm );\
        PARM( Vol.Parm, ParmName );\
        }\
      ETAB\
			}

#define STAGE_PARM(Parm,NameParam) \
		TAB(NameParam,1)\
			{\
			char ParmName[256];\
			for ( int n = 1; n <= nCub; n++ )\
				{\
        if ( pStage[n]->FullStage ) break;\
        sprintf_s ( ParmName, sizeof(ParmName), "%s-%s", pStage[n]->Name, #Parm );\
				PARM( pStage[n]->Parm, ParmName );\
				}\
      ETAB\
			}


#define STAGE_PARM_D(Parm,NameParam,Def) \
		{\
			char* szParam;\
			if(NameParam[0] == 0)\
			szParam = #Parm;\
			else \
			szParam = NameParam;\
			TAB(szParam,1)\
				{\
				char ParmName[256];\
				for ( int n = 1; n <= nCub; n++ )\
					{\
					if ( pStage[n]->FullStage ) break;\
					sprintf_s ( ParmName, sizeof(ParmName), "%s-%s", pStage[n]->Name, #Parm );\
					PARM_D( pStage[n]->Parm, ParmName, Def );\
					}\
					ETAB\
			}\
		}

CBaseCol::CBaseCol( char * Name, IBaseModel * pMain ) : IBaseModel ( Name, pMain )
, Vol ( "Объем", this )
//, TT( "Времена", this ) 
	{
#define NO_DEFECT
#define PROF
#define LONG
#define EPROF
#define ELONG
	#include "IO_Clear.h"
	#include "BaseCol_Pnt.h"
#undef PROF
#undef LONG
#undef EPROF
#undef ELONG
  Use_min_max = false;
  Calc_Flow_T = false;
  P_min = 0.0010;
  P_max = 20.;
	h_cub = 2.;
	n_Comp_Balance = -1;
  n_Comp_Delete = -1;
  P_top = 1.;
	Flow_out = 0;
	k_P_stage = 1.;
	P_Top_Z = 0.;
	CLEAR( Henry );
	CLEAR( Alfa_Comp );
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		Henry[n] = 1.;
	  Alfa_Comp[n] = 1.;
		}
	IsComp[G_N2] = true;
	UseHydrostat = true;
	kStep = 0;
	Проблемы = 0;
	ЕстьПроблемы = 0;
  Fast_Q_SS = false;
	TypeCol = 2; // Старая схема
  Model = "BaseCol";
  dt_Vol = 20.;
  Omega_Top = 10000.;
//  HenryH2S = 1.;
  UseCO2Appr = false;
  CalcAdsDes = false;
  Ksi_R = 10.;
  kInertHeat = 0.;
  kInertVout = 0.01;
  Разгерметизация_Верха = false;
  pOut_Gas = NULL;
  Omega_Def = 1000.;
  Eps_Gas = 0.;
  QuickGas = 2.;
  CalckVout = false;
  Vol.pCol = this;
  Flow_src_min = 0.;
  Tau_cool = 10.;
  Tau_gas = 10.;
  Tau_sliv = 2.;
  Work = true;
  Razgerm = false;
  OmegaRazgerm = 0.;
  SetPsrc = 0;
  kSrcPlus = 1.;
  kSrcMin = 1.;
  CLEAR(pHeat_In)
  CLEAR(pHeat_Out)
  CLEAR(CtrlComp)
  CLEAR(H_ads)
  pColAdditions = NULL;
  AdsType = AdsNone;
  pAdsGas_1 = NULL;
  OutPureGas = false;
  OutPureLiq = false;
  LongList = true; 
  ShowProf = false;
  Lev_Voronka = 0.1;
//  pAdsGas_2 = NULL;
	}

//void CBaseCol::CalcH2SCoef( )
//  {
//  double L_0 = log ( k_H2S_0 );
//  double L_1 = log ( k_H2S_1 );
//  B_H2S = ( L_1 - L_0 ) / ( 1. / ( T_H2S_1 + TK ) - 1. / ( T_H2S_0 + TK ));
//  A_H2S = L_0 - B_H2S / ( T_H2S_0 + TK );
//  }

CBaseCol_W::CBaseCol_W()
	{
	memset ( this, 0, sizeof ( CBaseCol_W ));
	}

CBaseCol_P::CBaseCol_P()
	{
	memset ( this, 0, sizeof ( CBaseCol_P ));
  QuickX = true;
  CanOverflow = false;
	}

int CBaseCol::GetParams( char * Type )
	{
  char ParmName[256];
  char Name[128];
  ShowProf = true;
  LongList = true;
#define SHOW
#define PROF if ( ShowProf ) {
#define LONG if ( LongList ) {
#define EPROF }
#define ELONG }
	#include "IO_Parms.h"
	#include "BaseCol_Pnt.h"
  if ( Type == NULL || Type[0] != 'M' )
    {
//  	TAB("Модули",5)
 //   return 0;
    }
  TAB("Колонна",3)
  TAB("Режим по тарелкам",3)
  SET_REGIM(T_liq,"Температура жидкости")
  SET_REGIM(T_gas,"Температура газа")
  SET_REGIM(P,"Давление(а)")
  SET_REGIM(Vout,"Поток газа наверх")
  SET_REGIM(Lout,"Поток жидкости вниз")
  SET_REGIM(V_Cond,"Поток конденсации")
//  SET_REGIM(Pz,"Давление паров")
  SET_REGIM(P_Pz,"Неравновесность")
  SET_REGIM(M_liq,"Масса жидкости")
  SET_REGIM(Level,"Уровень жидкости на тарелке")
//    STAGE_REGIM(M_liq)
//	  STAGE_REGIM(M_gas)
//	  STAGE_REGIM(Level)
//    STAGE_REGIM(Doliv)
//	  STAGE_REGIM(dh_hydro)
//	  STAGE_REGIM(dT_liq)
//	  STAGE_REGIM(dT_gas)
//    STAGE_REGIM(To_m3)
//    STAGE_REGIM(dT_liq_dH)
//    STAGE_REGIM(dT_gas_dH)
//    STAGE_REGIM(Eps)
  ETAB
  if ( CalcAdsDes )
    {
    switch ( AdsType )
      {
      case AdsH2S_MEA:
        {
        if ( pAdsGas_1 )
          {
          char ParmName[256];
          TAB("H2S", 1 )
          TAB("H2S Gamma", 1 )
          for ( int n = 1; n <= nCub; n++ )
            {
            if ( pStage[n]->FullStage ) break;
              sprintf_s ( ParmName, sizeof(ParmName), "#Gamma H2S %s", pStage[n]->Name );
            PARM( pStage[n]->Gamma[G_H2S], ParmName );
            }
          ETAB
          TAB("H2S газ", 1 )
          for ( int n = 1; n <= nCub; n++ )
            {
            if ( pStage[n]->FullStage ) break;
            sprintf_s ( ParmName, sizeof(ParmName), "#H2S в газе %s", pStage[n]->Name );
            PARM( pStage[n]->Y[G_H2S], ParmName );
            }
          ETAB
          TAB("H2S жидк", 1 )
          for ( int n = 1; n <= nCub; n++ )
            {
            if ( pStage[n]->FullStage ) break;
            sprintf_s ( ParmName, sizeof(ParmName), "#H2S в жидк. %s", pStage[n]->Name );
            PARM( pStage[n]->X[G_H2S], ParmName );
            }
          ETAB
          ETAB
          }
        }
      break;
      case AdsCO2_K2CO3:
      case AdsCO2_MEA:
        {
        if ( pAdsGas_1 )
          {
          char ParmName[256];
          TAB("CO2", 1 )
          TAB("CO2 Gamma", 1 )
          for ( int n = 1; n <= nCub; n++ )
            {
            if ( pStage[n]->FullStage ) break;
            sprintf_s ( ParmName, sizeof(ParmName), "#Gamma CO2 %s", pStage[n]->Name );
            PARM( pStage[n]->Gamma[G_CO2], ParmName );
            }
          ETAB
          TAB("CO2 газ", 1 )
          for ( int n = 1; n <= nCub; n++ )
            {
            if ( pStage[n]->FullStage ) break;
            sprintf_s ( ParmName, sizeof(ParmName), "#CO2 в газе %s", pStage[n]->Name );
            PARM( pStage[n]->Y[G_CO2], ParmName );
            }
          ETAB
          TAB("CO2 жидк", 1 )
          for ( int n = 1; n <= nCub; n++ )
            {
            if ( pStage[n]->FullStage ) break;
            sprintf_s ( ParmName, sizeof(ParmName), "#CO2 в жидк. %s", pStage[n]->Name );
            PARM( pStage[n]->X[G_CO2], ParmName );
            }
          ETAB
          ETAB
          }
        }
      break;
      }
    }
//
  LONG
  TAB("Остановы",5)
  STAGE_REGIM(StopSrc)
	STAGE_REGIM(StopLiq)
  STAGE_REGIM(StopGas)
	STAGE_REGIM(StopFlow)
	STAGE_REGIM(StopHeat)
  ETAB
  ELONG
/////////////////////////////////////
  TAB("Параметры",1)
  PARM( nCub, "#Число тарелок + куб" );
  PARM( P_max, "Макс. давление" )
  PARM( P_min, "Мин. давление" )
  LONG
  PARM( OutPureGas, "Вывод чистого газа" )
  PARM( OutPureLiq, "Вывод чистой жидкости" )
  ELONG
////////////////////////////
  PROF
  TAB("Объем",1)
    Vol.GetParams( "" );
  ETAB
  EPROF
/////////////////////////////////////////////////	
  TAB("Параметры по тарелкам",3)
  TAB("Тип тарелки",1)
	  for ( int n = 1; n <= nCub; n++ )
		  {
      if ( pStage[n]->FullStage ) break;
      sprintf_s ( ParmName, sizeof(ParmName), "Тип |%s", pStage[n]->Name );
			PARM_L( pStage[n]->StageType, ParmName, "Колпачковая;Насадка;Ситчатая;Глухая;Куб;" );
      }
  ETAB
//	STAGE_PARM(Omega,"Проходимость тарелки")
	STAGE_PARM(dP_nom,"Номинальный перепад на тарелке")
	STAGE_PARM(V_nom,"Номинальный поток через тарелку")
	STAGE_PARM_D(k_L,"Скорость слива с тарелки",3000.)
  LONG
  STAGE_PARM_D(k_LV,"",0.)
  ELONG
	STAGE_PARM_D(h_st,"Высота перелива",0.05)
	STAGE_PARM_D(Cp_korp,"Теплоёмкость корпуса",2000.)
  STAGE_PARM_D(k_GL,"Теплообмен газ-жидкость",1e6 )
	STAGE_PARM_D(Vol_Stage,"Объем тарелки",2.)
	STAGE_PARM_D(S_Stage,"Площадь тарелки",5.)
//	STAGE_PARM(Alfa_Stage)
  STAGE_PARM_D(k_Cool,"Охлаждение тарелки",0.)
  STAGE_PARM_D(Eff,"Заданный КПД тарелки",1.)
  LONG
  STAGE_PARM_D( Ksi_R_Rel,"",1.)
  ELONG
/////////////////////////////////////  
  LONG
  TAB("Дополнения",5)
//	STAGE_PARM_D(HenryStage,1.)
	STAGE_PARM_D(Hvap_Stage,"",1.)
	STAGE_PARM_D(Cp_gas,"",30.)
	STAGE_PARM_D(Cp_liq,"",50.)
  STAGE_PARM_D(T_liq_Z,"",0.)
  STAGE_PARM_D(Lev_Z,"",0.)
	STAGE_PARM_D(Doliv,"",0.)
  STAGE_PARM_D(CalcTom3,"",false)
  STAGE_PARM_D(Tau_SS,"",30000.)
  STAGE_PARM_D( dP_Hydro_Static,"", 0. )
  STAGE_PARM_D( dP_Hydro_Level,"", 0. )
  STAGE_PARM_D( h_In,"", 0.0 )
  STAGE_PARM_D( k_Hydro_statatic,"", 0.0 )
  ETAB
  ELONG

  ETAB
  ETAB
  ETAB
///////////////////////
  //------------------------------------------------
  TAB("Параметры компонент",3)
  TAB("Растворимость",5)
  PROF
  STAGE_PARM(nHenry,"")
  EPROF
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		Sprintf ( ParmName, "Henry %s", pGas[n]->Name );
		PARM_D( Henry[n], ParmName, 1. );
		}
  ETAB
////////////////
  LONG
  TAB("Адсорбция",4)
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    //				
    Sprintf ( ParmName, "Теплота %s", pGas[n]->Name );
    PARM( H_ads[n], ParmName );
    }
  ETAB
  TAB("Контроль",5)
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    //				Henry[n] = 1.;
    Sprintf ( ParmName, "Контроль %s", pGas[n]->Name );
    PARM( CtrlComp[n], ParmName );
    }
  ETAB
  ELONG
/*
  for ( int t = 0; t < MAX_HENRY; t++ )
    {
    Sprintf ( ParmName, "/Henry %d", t + 1 );
    TAB(ParmName,5)
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      //				Henry[n] = 1.;
      Sprintf ( ParmName, "%d)Henry %s", t + 1, pGas[n]->Name );
      PARM_D( Henry_Stage[t][n], ParmName, 1. );
      }
    }
*/
	TAB("Очистка",5)
  PARM( MustClear, "Очищать" );
	for ( int n = 0; n < LAST_GAS; n++ )
		{
		Sprintf ( Name, "Чистка %s", pGas[n]->Name );
		PARM( ClearComp[n], Name );
		}
  ETAB
  ETAB
////////
	TAB("Баланс",5)
	PARM( Flow_Balance_mol, "#Потоки моль" );
	PARM( Flow_Balance_kg, "#Потоки кг" );
	PARM( H_Balance, "#Энтальпии" );
LONG
  TAB("Компоненты",5)
  for ( int n = 0; n < LAST_GAS; n++ )
	  {
    Sprintf ( Name, "#Баланс %s", pGas[n]->Name );
	  PARM( Comp_Balance[n], Name );
	  }
  ETAB
  PARM( n_Comp_Balance, "#Компонента для расчета баланса" );
  PARM( Comp_Balance_Name, "Баланс по этой компоненте" )
  if ( n_Comp_Balance >= 0 )
	  {
	  char ParmName[256];
	  TAB(pGas[n_Comp_Balance]->Name,5)
	  for ( int n = 1; n <= nCub; n++ )
		  {
		  sprintf_s ( ParmName, sizeof(ParmName), "%s баланс", pStage[n]->Name );
		  PARM( pStage[n]->SummStageBalance, ParmName );
		  }
	  }
	  ETAB
  else
	  {
	  char ParmName[256];
	  TAB("Баланс по тарелкам",5)
	  for ( int n = 1; n <= nCub; n++ )
		  {
		  sprintf_s ( ParmName, sizeof(ParmName), "%s сумм. баланс", pStage[n]->Name );
      PARM( pStage[n]->SummStageBalance, ParmName );
		  }
    ETAB
	  }
ELONG
  ETAB
LONG
	PARM( Проблемы, "#Проблемы" );
ELONG
  TAB("Тарелки",5)
  for ( int n = 1; n <= nCub; n++ )
	  {
	  sprintf_s ( ParmName, sizeof(ParmName), "%s", pStage[n]->Name );
    TAB(ParmName,5)
      pStage[n]->GetParams ( "" );
    ETAB
	  }
  ETAB
#undef SHOW
#undef PROF
#undef LONG
#undef EPROF
#undef ELONG
	return 0;
	}

int CBaseCol::SaveState( )
  {
	S_CLASS("W",CBaseCol_W)
  return 0;
  }

int CBaseCol::RestoreState( char * StrName )
  {
//  return 1;
//  IGNORE
	R_CLASS("W",CBaseCol_W)
  return 1;
  }

int CBaseCol::ShowParams( DWORD DataType, struct CShowData * pSD )
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

int CBaseCol::UpdateParam( CParams & Param )
  {
  if ( Param.Addr == &UseGasOut )
    {
    if ( UseGasOut )
      CaclOnVol = true;
    }
	if ( Param.Addr == &UseHydrostat )
		{
		for ( int n = 1; n < nCub; n++ )
			{
			pStage[n]->UseGydroIndP = UseHydrostat;
			}
		}
  if ( pAdsGas_1 )
    {
    if ( Param.Addr == &pAdsGas_1->T_PNT_0 || Param.Addr == &pAdsGas_1->T_PNT_1 ||
         Param.Addr == &pAdsGas_1->k_PNT_0 || Param.Addr == &pAdsGas_1->k_PNT_1 )
      {
      pAdsGas_1->CalcCoef( );
      return 0;
      }
    }
  //if ( pAdsGas_2 )
  //  {
  //  if ( Param.Addr == &pAdsGas_2->T_PNT_0 || Param.Addr == &pAdsGas_2->T_PNT_1 ||
  //    Param.Addr == &pAdsGas_2->k_PNT_0 || Param.Addr == &pAdsGas_2->k_PNT_1 )
  //    {
  //    pAdsGas_2->CalcCoef( );
  //    return 0;
  //    }
  //  }
  //MustClear = false;
  //for ( int c = 0; c < LAST_GAS; c++ )
  //  {
  //  if ( ClearComp[c] )
  //    {
  //    MustClear = true;
  //    break;
  //    }
  //  }
	return 0;
  }

void CBaseCol::DrawFixed ( CDrawObjData * pDraw )
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

void CBaseCol::DrawState ( CDrawObjData * pDraw )
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

CExtern_Pnt * CBaseCol::GetPoint( const char * PntName )
  {
  if(strstr(ObjName, "T-101") && PntName[0] == 'L')
	  int K = 6;
  char Type = PntName[0];
  if ( lstrcmp ( PntName, "Внешняя т-ра при смешении" ) == 0 )
    {
    CAnalog * pPnt = new CAnalog( pMain, (char*)PntName, "AI", (char*)PntName );
    DWORD Flag = 0x11111111;
    pPnt->Flag = Flag;
    pMain->AddPoint( pPnt );
    return pPnt;
    }
  if ( Type == 'D' )
    {
    int N1 = 0;
    int N2 = 0;
    if ( !Numb2 ( PntName + 1, N1, N2 ))
      return NULL;
    Char<256> NamePnt;
    NamePnt.Prt( "Перепад т.%d-%d|%s", N1, N2, PntName );
    if ( N1 <= 0 || N1 > nCub )
      return NULL;
    if ( N2 <= 0 || N2 > nCub )
      return NULL;
    CAnalog * pPnt = (CAnalog*)FindPoint( (char*)NamePnt );
    if ( pPnt )
      return pPnt;
    pPnt = new CAnalog( pMain, (char*)NamePnt, "AO", "Перепад между тарелками" );
    DWORD Flag = ( N2 << 16 ) | ( N1 << 8 ) | Type;
    pPnt->Flag = Flag;
    return pPnt;
    }
  int N = 0;
  char * Name;
  char * Desc;
  if ( strncmp ( PntName, "Pakp_", 5 ) == 0 )
    {
 // Абс давление в kPa
    Type = 'A';
    N = Numb ( PntName + 5 );
    }
  else
    {
    N = Numb ( PntName + 1 );
    }
  if ( N < 1 || N > nCub )
    return NULL;
  switch ( Type )
    {
    case 'A':
      Name = "Абс. давление ";
      Desc = "Абс. давление газа на тарелке, кПа"; 
    break;
    case 'P':
      Name = "Давление ";
      Desc = "Давление газа на тарелке"; 
    break;
    case 'T':
      Name = "Температура ";
      Desc = "Температура газа на тарелке"; 
    break;
    case 'L':
      Name = "Уровень ";
      Desc = "Уровень жидкости на тарелке"; 
    break;
    default:
    return NULL;
    }
  Char<256> NamePnt;
  NamePnt.Prt( "%s т. %d|%s", Name, N, PntName );
  CAnalog * pPnt = (CAnalog*)pMain->FindPoint( (char*)NamePnt );
  if ( pPnt == NULL )
    {
    pPnt = new CAnalog( pMain, (char*)NamePnt, "AO", Desc );
    //pMain->AddPoint( pPnt );	//Добавляется в new
    }
  else
    KKK();
  DWORD Flag = ( N << 8 ) | Type;
  pPnt->Flag = Flag;
//    return NULL;
//    pPnt = (CAnalog*)FindPoint( NamePnt );
  return pPnt;
  }

void CBaseCol::SetPoints( )
  {
  CAnalog * pPnt = (CAnalog*)pMain->pModel_Pnt;
  while ( pPnt )
    {
    DWORD Flag = pPnt->Flag;
    if ( Flag == 0x11111111 )
      {
//     "Внешняя т-ра при смешении"
      Ext_T = pPnt->Value;
      pPnt = (CAnalog*)pPnt->pNext_Model_Pnt;
      continue;
      }
    char Type = pPnt->Flag & 0xFF;
    if ( Type == 'D' )
      {
      int N1 = ( Flag >> 8 ) & 0xFF;
      int N2 = ( Flag >> 16 ) & 0xFF;
      CStage * pS1 = pStage[N1];
      CStage * pS2 = pStage[N2];
      *pPnt = pS1->P - pS2->P;
      pPnt = (CAnalog*)pPnt->pNext_Model_Pnt;
      continue;
      }
    int N = Flag >> 8;
    CStage * pS = pStage[N];
    if ( N == nResmixT && Type == 'T' )
      {
      if ( n1mixT >= 1 && n1mixT <= nCub && Ext_T != 0. )
        {
        double T1 = pStage[n1mixT]->T_liq;
        double T2 = Ext_T;
        double T = Alfa_n1 * T1 + ( 1. - Alfa_n1 ) * T2;
        *pPnt = T;
        pPnt = (CAnalog*)pPnt->pNext_Model_Pnt;
        continue;
        }
      }
    switch ( Type )
      {
      case 'A':
        *pPnt = pS->P * 98.0665;
      break;
      case 'T':
        *pPnt = pS->T_liq;
      break;
      case 'P':
        *pPnt = pS->P - 1.;
      break;
      case 'L':
        *pPnt = pS->Level * 100.;
      break;
      case 'D':
        *pPnt = pS->Level * 100.;
      break;
      }
      pPnt = (CAnalog*)pPnt->pNext_Model_Pnt;
    }
  }

void CBaseCol::CreateHeat ( int nFirstSt, int nLastSt, bool UpToDown )
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

void CBaseCol::SetHeatQ ( int nFirstSt, int nLastSt, bool Use_Q_HeatZ, double Q_Z, double & k_Q_HeatZ, double KS, double & dQ )
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

void CBaseCol::SetHeatFlow ( int nFirstSt, int nLastSt, bool UpToDown, CFlow * pIn, CFlow * pOut )
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

#include "stdafx.h"
#include "SysDataTypes.h"
#include "DataTypes.h"
#define DLL_BaseCol
#include "BaseCol_NE.h"

//??extern CGas::eTypeComp Type[K_GAS];

//bool CStage_P::ShowX = false;
//bool CStage_P::ShowY = false;
//bool CStage_P::ShowPz = false;
//bool CStage_P::ShowP_Y = false;
//bool CStage_P::ShowPzX = false;
//bool CStage_P::ShowVc_Comp = false;
//bool CStage_P::ShowX_In = false;
//bool CStage_P::ShowY_In = false;

CStage_Ptr::CStage_Ptr()
{
  memset ( this, 0, sizeof ( CStage_Ptr ));
}
const double Cmin = 1e-9;

void CStage_NE::CompressXY( bool Used[K_GAS] )
{
  int k = 0;
  for ( int n = 0; n < LAST_GAS; n++ )
  {
    if ( IsComp[n] )
    {
      if ( X[n] > Cmin || Y[n] > Cmin )
      {
        k++;
        Used[n] = true;
      }
    }
  }
  if ( k == 0 )
  {
    IsComp[G_N2] = true;
    Y[G_N2] = 1.;
    X[G_N2] = 1.;
  }
}

void CStage_NE::Add ( CExtIO * pSrc )
{
  ASS(kSrc < MAX_IN)
  Src[kSrc++] = pSrc;
}

int CStage_NE::SaveState( )
{
  S_CLASS("W",CStage_W);
  return 0;
}

int CStage_NE::RestoreState( char * StrName )
{
  //  return 1;
  COPY_FIRST
  R_CLASS("W",CStage_W);
  return 1;
}

int CStage_NE::ShowParams( DWORD DataType, struct CShowData * pSD )
{
  switch ( DataType )
  {
    case sp_Actions:
      return 1;
  }
  return 0;
}

CStage_W::CStage_W()
{
  memset ( this, 0, sizeof ( CStage_W ));
  Pz = 1.;
  Cp_gas = 20.;
  Cp_liq = 20.;
  dP_Stage = 0.;
  Alfa_Level = 1.;
}

CStage_P::CStage_P()
{
  memset ( this, 0, sizeof ( CStage_P ));
  //	Gamma_1_in_2 = 50.;
  //	Gamma_2_in_1 = 50.;
  // 	CalcTom3 = false;
  ExtHeat = 0.;
  Omega_IO = 10000.;
  ЭтоКуб = false;
  kVoutIO = 1.;
  Eps_Murphree = 1.;
  //  Cp_korp_gas = 100.;
  //
  Cp_korp = 100.;
  for ( int n = 0; n < LAST_GAS; n++ )
    k_h_cond[n] = 1.;
  V_nom = 10000.;
}

void CStage_NE::Init()
{
  Sloj0.IsComp = IsComp;
  Sloj0.Alfa_Comp = Alfa_Comp;
  Sloj0.pAlfa_Stage = &Alfa_Stage;
  Sloj0.pDown = pDown;
  Sloj0.pEps = &Eps;
  Sloj0.Henry = Henry;
  Sloj0.Gamma = Gamma;
}

static bool CalcTom3 = false;

CStage_NE::CStage_NE( char * _Name, IBaseModel * pMain ) :	IBaseModel ( _Name, pMain )
{
  ObjName = _Name;
  Model = "ColumnStage";
  pUp = NULL;
  pDown = NULL;
  Yin = NULL;
  CLEAR(Name)
  //??	CLEAR(Type)
  //for ( int n = 0; n < LAST_GAS; n++ )
  //	{
  //	ASS(pGas[n]->Type != CGas::None );
  //	Type[n] = pGas[n]->Type;
  //	}
  CLEAR(Name)
  Level = 0.;
  Flow_gas_in = 0.;
  Flow_liq_in = 0.;
  //
  CLEAR(F_gas)
  CLEAR(F_liq)
  CLEAR(P_z)
  CLEAR(Vc_Comp)
  CLEAR(h_gas)
  CLEAR(h_liq)
  CLEAR(h_gas_T_liq)
  CLEAR(Xnew)
  CLEAR(P_Y)
  CLEAR(P_PzX)
  CLEAR(X)
  CLEAR(Y)
  CLEAR(Y_In)
  //
  P = 1.;
  Pz = 1.;
  T_In = T_liq = T_gas = 15.;
  Vol_Stage = 2.;
  S_Stage = 5.;
  Real_S_Stage = 10.;
  Real_H_Stage = 0.7;
  Real_Vol_Stage = 7.;
  double RT = RP * ( T_gas + TK );
  M_gas = P * Vol_Stage / RT;
  Y[G_N2] = 1.;
  int G = G_N2;
  M_liq = 0.;
  h_st = 0.05;
  k_L = 3000.;
  Cp_gas = 30.;
  Cp_liq = 50.;
  Alfa_Stage = 1.;
  k_GL = 1e6;
  Lev_min = 0.;
  Lev_max = 2.;
  Hvap_Stage = 1.;
  T_liq_Z = 0.;
  k_Cool = 0.;
  dH_liq_Z = 0.;
  dH_gas_Z = 0.;
  SetdH = false;
  Fin_Bal = 0.;
  Fout_Bal = 0.;
  dFdt_Bal = 0.;
  To_m3 = 0.;
  //	pRegLev = NULL;
  M_min = 0.1;
  Model = "Stage";
  kSrc = 0;
  Trace = false;
  Lev_Z = 0.;
  Lev_Opt = 0.;
  Max_Flow_Add = 10000.;
  StageBalance = 0.;
  FlowGasOut = 0.;
  FullStage = false;
  HenryStage = 1.;
  for ( int n = 0; n < K_GAS; n++ )
    Gamma[n] = 1.;
  //  h_In = 2.0;
  //  k_Hydro_statatic = 0.0;
  Doliv = 0;
  Sliv = 0.;
  dLev_dt_max = 0.;
  //  dP_Stage_Static = 0.01;
  //  dP_Stage_Dynamic = 0.001;
  Мерфри = 100.;
  Sloj0.SpecStop1 = &SpecStop1;
  Sloj0.SpecStop2 = &SpecStop2;
  _P = 0.;
  pCalcTom3 = &CalcTom3;
}

CExtHeat::CExtHeat( char * _Name, IBaseModel * _pMain ) :	IBaseModel ( _Name, _pMain )
  , Out( "Нагреватель", this )
{
  pPrevHeat = NULL;
  pNextHeat = NULL;
  pComp = NULL;
  nStage = 0;
  nGroup = -1;
  h_in = 0.;
  KS = 1000.;
  Cp = 30.;
}

bool CExtHeat::SetIn ( CFlow * _pFlow )
{
  pFlow = _pFlow;
  if ( pFlow == NULL )
    return false;
  h_in = pFlow->h_Flow;
  Out.PH ( pFlow );
  pComp = pFlow->Get_Comp();
  P = pFlow->P;
  Flow = pFlow->Flow_mol;
  return true;
}

bool CExtHeat::SetOut ( CFlow * _pFlow )
{
  pFlow = _pFlow;
  if ( pFlow == NULL )
    return false;
  if ( pPrev == NULL )
    return false;
  pComp = pPrevHeat -> pComp;
  h_in = pPrevHeat->Out.h;
  pFlow->h_Flow = h_in;
  FINITE(pFlow -> h_Flow)
  Out.PH( pFlow );
  pFlow->T = Out.T;
  return true;
}

/*
bool CExtHeat::CalcHeat( double T_liq, double & dH )
  {
  if ( pPrev == NULL )
    return false;
  pComp = pPrevHeat->pComp;
  P = pPrevHeat->P;
  KS = pPrevHeat->KS;
  h_in = pPrevHeat->Out.h;
  Out.PH_ext( P, h_in, KS, T_liq, pComp, dH );
  return true;
  }
*/

bool CExtHeat::SetCoef( double &A, double &B )
{
  pComp = pPrevHeat->pComp;
  P = pPrevHeat->P;
  KS = pPrevHeat->KS;
  h_in = pPrevHeat->Out.h;
  Flow = pPrevHeat->Flow;
  double FCp = Flow * Cp;
  double T_in = pPrevHeat->Out.T;
  //
  A = KS * FCp / ( FCp + KS );
  B = A * T_in;
  return true;
}

bool CExtHeat::CalcHeat( double T_ext )
{
  double FCp = Flow * Cp;
  double Tin = pPrevHeat->Out.T;
  double Tout = ( KS * T_ext + FCp * Tin ) / ( KS + FCp );
  double dh = Cp * ( Tout - Tin );
  double h_out = h_in + dh;
  Out.PH( P, h_out, pComp );
  //
  double dT = Out.T - Tout;
  double D = Tout - Tin;
  double dC = 0.1 * dT;
  if ( D < 0. )
    Cp += dC;
  else
    Cp -= dC;
  if ( Cp < 1. )
    Cp = 1.;
  if ( Cp > 1000. )
    Cp = 1000.;
  //
  return true;
}

int CExtHeat::SaveState( )
{
  S_CLASS("ExtHeat_W",CExtHeat_W)
  return 0;
}

int CExtHeat::RestoreState ( char * StrName )
{
  R_CLASS("ExtHeat_W",CExtHeat_W)
  return 1;
}

#undef STAGE_PARM_D
#define STAGE_PARM_D(Parm,Name) \
{\
    if ( CParams::SaveAllParams )\
  {\
      PARM( Parm, Name );\
  }\
    else\
  {\
      ASS( pCol->kStageRegim < MAX_REGIM )\
      IStageParam & SP = pCol->StageRegim[pCol->kStageRegim++];\
      SP.Init();\
      SP.pCol = pCol;\
      SP.ShftParam[0] = (BYTE*)&Parm - (BYTE*)(pCol->pParamsStage);\
      SP.Type = IStageParam::DOUBLE;\
      PARM( SP, Name );\
  }\
}

#undef STAGE_PARM_I
#define STAGE_PARM_I(Parm,Name) \
{\
    if ( CParams::SaveAllParams )\
  {\
      PARM( Parm, Name );\
  }\
    else\
  {\
      ASS( pCol->kStageRegim < MAX_REGIM )\
      IStageParam & SP = pCol->StageRegim[pCol->kStageRegim++];\
      SP.Init();\
      SP.pCol = pCol;\
      SP.ShftParam[0] = (BYTE*)&Parm - (BYTE*)(pCol->pParamsStage);\
      SP.Type = IStageParam::INT;\
      PARM( SP, Name );\
  }\
}

#undef STAGE_PARM_S
#define STAGE_PARM_S(Parm,Name) \
{\
    PARM( Parm, Name );\
}

#undef STAGE_PARM_D
#define STAGE_PARM_D(Parm,Name) \
{\
    PARM( Parm, Name );\
}

#undef STAGE_PARM_I
#define STAGE_PARM_I(Parm,Name) \
{\
    PARM( Parm, Name );\
}

/*
#undef STAGE_PARM_D
#define STAGE_PARM_D(Parm,Name) \
  {\
  if ( CParams::SaveAllParams )\
    {\
    PARM( Parm, Name );\
    }\
  else\
    {\
    ASS( pCol->kStageRegim < MAX_REGIM )\
    IStageParam & SP = pCol->StageRegim[pCol->kStageRegim++];\
    SP.Init();\
    SP.pCol = pCol;\
    SP.ShftParam[0] = (BYTE*)&Parm - (BYTE*)(pCol->pParamsStage);\
    SP.Type = IStageParam::DOUBLE;\
    PARM( SP, Name );\
    }\
  }

#undef STAGE_PARM_I
#define STAGE_PARM_I(Parm,Name) \
  {\
  if ( CParams::SaveAllParams )\
    {\
    PARM( Parm, Name );\
    }\
  else\
    {\
    ASS( pCol->kStageRegim < MAX_REGIM )\
    IStageParam & SP = pCol->StageRegim[pCol->kStageRegim++];\
    SP.Init();\
    SP.pCol = pCol;\
    SP.ShftParam[0] = (BYTE*)&Parm - (BYTE*)(pCol->pParamsStage);\
    SP.Type = IStageParam::INT;\
    PARM( SP, Name );\
    }\
  }

#undef STAGE_PARM_S
#define STAGE_PARM_S(Parm,Name) \
  {\
  if ( CParams::SaveAllParams )\
    {\
    PARM( Parm, Name );\
    }\
  else\
    {\
    ASS( pCol->kStageRegim < MAX_REGIM )\
    IStageParam & SP = pCol->StageRegim[pCol->kStageRegim++];\
    SP.Init();\
    SP.pCol = pCol;\
    SP.ShftParam[0] = (BYTE*)&Parm - (BYTE*)(pCol->pParamsStage);\
    SP.Type = IStageParam::STRING;\
    PARM( SP, Name );\
    }\
  }
*/

#include "IO_Parms.h"

void CStage_NE::VisualParams( char * )
{
  if ( CParams::SaveAllParams )
    return;
//  pCol->pParamsStage = this;
//  pCol->kStageRegim = 0;
#define this pCol
  TAB("Остановы",5)
  STAGE_PARM_I(StopSrc,"#Смешивание")
  STAGE_PARM_I(StopLiq,"#Жидкость")
  STAGE_PARM_I(StopGas,"#Газ")
  STAGE_PARM_I(StopSloj,"#Слой")
  STAGE_PARM_I(StopHeat,"#Нагрев")
  STAGE_PARM_I(SpecStop1,"#Спец стоп 1")
  STAGE_PARM_I(SpecStop2,"#Спец стоп 2")
  ETAB
    TAB("Режим тарелки",3)
    STAGE_PARM_D(Flow_Summ_In,"#Суммарный поток на тарелку")
    //	STAGE_PARM_D(L_In,"")
    //STAGE_PARM_D(H_Summ_In,"")
    STAGE_PARM_D(T_liq,"#Температура жидкости")
    STAGE_PARM_D(T_gas,"#Температура газа")
    STAGE_PARM_D(_P,"#Давление(а)")
    STAGE_PARM_D(Vin,"#Поток газа на вход")
    STAGE_PARM_D(Vout,"#Поток газа на выход")
    STAGE_PARM_D(Lin,"#Поток жидкости на тарелку")
    STAGE_PARM_D(L_Down,"#Поток жидкости с тарелки")
    STAGE_PARM_D(V_Cond,"#Поток конденсации")
    STAGE_PARM_D(Мерфри,"#Эффективность")
    STAGE_PARM_D(P_Pz,"#Равновесие")
    STAGE_PARM_D(M_gas,"#Масса газа")
    STAGE_PARM_D(M_liq,"#Масса жидкости")
    STAGE_PARM_D(Level,"#Уровень")
    //  STAGE_PARM_D(Lev_Z,"")
    //  STAGE_PARM_D(Doliv,"")
    //  STAGE_PARM_D(To_m3,"")
    STAGE_PARM_D(h_gas_mix,"#Энтальпия газа")
    STAGE_PARM_D(h_liq_mix,"#Энтальпия жидкости")
    ETAB
    char Name[128];
  /*
  TAB("Конденсация",4)
  for ( int n = 0; n < LAST_GAS; n++ )
    {
    //				
    Sprintf ( Name, "Теплота конденсации %s", pGas[n]->Name );
    PARM_D( k_h_cond[n], Name, 1. );
    }
  ETAB
*/
  TAB("Распределения",3)
  PARM_L ( pCol->ShowVariantParams, "Показывать", "Состав жидкости;Состав газа;Поток конденсации;Тепло конденсации;Pz;PzX;PY;PY/PzX;Lin;Lout;Vin;Vout;Для расчета;" )
  //  PARM(pCol->ShowVariantParams,"Распределение")
  for ( int n = 0; n < LAST_GAS; n++ )
  {
    //				
    Sprintf ( Name, "#%s", pGas[n]->Name );
    PARM( pCol->VariantParams[n], Name );
  }
  /*
  for ( int n = 0; n < LAST_GAS; n++ )
		{
		if ( IsComp[n] )
			{
      ASS( pCol->kStageRegim < MAX_REGIM )
      IStageParam & SP = pCol->StageRegim[pCol->kStageRegim++];
      SP.Init();
      SP.pCol = pCol;
      SP.Type = SP.VARIANT;
      SP.SetShft ( &X[n], 100. );
      SP.SetShft ( &Y[n], 100. );
      //SP.SetShft ( &P_z[n] );
      //SP.SetShft ( &P_Y[n] );
      //SP.SetShft ( &P_PzX[n] );
      SP.SetShft ( &Vc_Comp[n], 1. );
      SP.SetShft ( &h_cond[n], 1. );
			sprintf_s ( Name, 64, "#%s", pGas[n]->Name );
			PARM ( SP, Name );
			}
		}
*/
  ETAB
#undef this
}

int CStage_NE::UpdateParam( struct CParams & Param )
{
  if ( Param.Addr == &Lev_max )
    KKK();
  //  if ( pGammaCommon )
  //    pGammaCommon->UpdateParams ( Param );
  //  if ( pGammaAbs )
  //    pGammaAbs->UpdateParams ( Param );
  if ( pColumnGamma )
    pColumnGamma->UpdateParams ( Param );  
  if ( pReactions )
    pReactions->UpdateParams ( Param );  
  return pCol->UpdateParam( Param );
  //  return 0;
}

int CStage_NE::GetParams( char * )
{
  //pCol->pParamsStage = this;
  VisualParams( "" );
  TAB("Параметры тарелки",1)
  STAGE_PARM_S ( Name, "#Имя" )
  STAGE_PARM_D ( Vol_Stage, "Объем" )
  STAGE_PARM_D ( S_Stage, "Площадь" )
  STAGE_PARM_D ( h_st, "Высота перелива" )
  STAGE_PARM_D ( k_L, "Скорость слива" )
  STAGE_PARM_D ( Cp_gas, "#Cp газа" )
  STAGE_PARM_D ( Cp_liq, "#Cp жидкости" )
  STAGE_PARM_D ( Cp_korp, "Cp корпуса" )
  STAGE_PARM_D ( k_GL, "Теплообмен газ-жидкость" )
  STAGE_PARM_D ( k_Cool, "Охлаждение тарелки" ); 
  STAGE_PARM_D ( Lev_min, "Минимальный уровень" )
  STAGE_PARM_D ( Lev_max, "Максимальный уровень" )
  //  STAGE_PARM_D ( dP_Hydro_Static, "Статический гидростат" )
  //  STAGE_PARM_D ( dP_Hydro_Level, "Гидростат от уровня" )
  //  STAGE_PARM_D ( dP_Stage_Static, "Статический перепад на тарелке" )
  //  STAGE_PARM_D ( dP_Stage_Dynamic, "Динамический перепад на тарелке" )
  TAB( "Дополнение",5 )
  STAGE_PARM_D ( T_liq_Z, "T_liq_Z" );
  STAGE_PARM_D ( Lev_Opt, "Опт. уровень" );
  STAGE_PARM_D ( Max_Flow_Add, "Макс. долив, кг" );
  PARM( pCol->CalcStageBalance, "#Рассчитывать баланс" )
  PARM_L ( pCol->n_Comp_Balance_1, "Номер компоненты для баланса", PrefListComp )
  STAGE_PARM_D ( StageBalance, "#Баланс для заданной компоненты" )
  ETAB
    TAB( "Расширения",5 )
    //  if ( pGammaCommon )
    //    pGammaCommon->GetParams( this, true );
    //  if ( pGammaAbs )
    //    pGammaAbs->GetParams( this, true );
    if ( pColumnGamma )
    pColumnGamma->GetParams( this, true );
  ETAB
#if 0
  if ( pGammaAbs )
    {
    int nStartParam = CParams::kParams;
    pGammaAbs->GetParams( this, true );
    int nEndParam = CParams::kParams;
    int nParam = nStartParam;
    for ( int n = nStartParam; n < nEndParam; n++ )
      {
      CParams & P = CParams::Params[n];
      if ( P.Type == 'T' || P.Type == 'E' || P.Type == 'X' )
        {
        CParams::Params[nParam++] = P;
        continue;
        }
      ASS( pCol->kStageRegim < MAX_REGIM )
      IStageParam & SP = pCol->StageRegim[pCol->kStageRegim++];
      SP.Init();
      SP.pCol = pCol;
      SP.ShftParam[0] = (BYTE*)P.Addr - (BYTE*)(pCol->pParamsStage);
      SP.Type = IStageParam::INT;
      if ( P.Type == 'D' )
        SP.Type = IStageParam::DOUBLE;
      if ( P.Type == 'S' )
        SP.Type = IStageParam::STRING;
      CParams & __P = CParams::Params[nParam++];
      __P.SetDefaultValue(NULL);
	    __P.Set( this, P.ParamName, SP, sizeof ( SP ) );
      __P.Property = false;
      }
    CParams::kParams = nParam;
    }
#endif
    if ( pReactions )
    pReactions->GetParams( this );
  ETAB
    return 0;
}


void CStage_NE::CorrData( )
{
  bool CorrY = false;
  for ( int n = 0; n < LAST_GAS; n++ )
  {
    if ( !_finite(X[n]) || X[n] < 0. )
      X[n] = 0.;
    if ( !_finite(Y[n]) || Y[n] < 0. )
      Y[n] = 0.;
    else
      CorrY = true;
  }
  if ( !CorrY )
  {
    IsComp[G_N2] = true;
    Y[G_N2] = 1.;
  }
}

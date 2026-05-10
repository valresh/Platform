#pragma once

//#include <crossplatform.h>
//#undef COMMONOBJ_API
//#ifdef UTILS_EXPORTS
//#define COMMONOBJ_API _EXP
//#else
//#define COMMONOBJ_API _IMP
//#endif

#include <H_Declare.h>
#include <Yokogawa/YPARM_CREATE.h>
#include <YBridge2SysParam.h>
#include <yISerializer.h>
#include <YokogawaMode.h>

#undef  UMODE
#define UMODE SMode mode;

enum
{
  eAlarmY_2_Signal = 22,
};

template<class T>
DWORD Alarm2SignalFlag( T &W )
{
  DWORD flag = (W->FlagUse&&CAlarmBase::fuY_Mask) << eAlarmY_2_Signal;
  return flag;
}

/////////////////////////////////////////////////////////////
/*struct IN_DLL CINode_I
	{
	bool Used;
  int I;
  CINode_I()
    {
    Used = false;
    I = 0;
    }
	};
struct IN_DLL CINode_D
	{
	bool Used;
  double D;
  CINode_D()
    {
    Used = false;
    D = 0.;
    }
	};
struct IN_DLL CONode_I
	{
  int I;
  CONode_I()
    {
    I = 0;
    }
	};
struct IN_DLL CONode_D
	{
  double D;
  CONode_D()
    {
    D = 0.;
    }
	};*/
#undef IN
#undef OUT
/////////////////////////////////////////////////////////////
#undef  STD_DEFINE_WP
#define STD_DEFINE_WP \
  static char * ClassName;\
  int Init();\
  int GetParams( KYBridge2SysParam &params );\
  int SaveState();\
  int RestoreState( char * StrName );\
  int StateSave( IYokoStateSer* pStateSer );\
  int StateRestore( IYokoStateSer* pStateSer );\
  static void BuildList();
/////////////////////////////////////////////////////////////
#undef  STD_DEFINE
#define STD_DEFINE { STD_DEFINE_WP
//
#undef  STD_DEFINE_COMM
#define STD_DEFINE_COMM(Comm) public Comm { STD_DEFINE_WP
//
////////////////////////////////////////////////////////////////
//YOKO_TYPE( 0200 , _GS     , "%GS"      )
struct YOKOW_API C_GS_W
  {
#include "Yokogawa/_GS.hpp"
  };

struct YOKOW_API C_GS : public CAlarmBase, public C_GS_W STD_DEFINE enum { TypeID = id__GS };
  };

////////////////////////////////////////////////////////////////
//YOKO_TYPE( 0201 , _SW     , "%SW"      )
struct YOKOW_API C_SW_W
  {
#include "Yokogawa/_SW.hpp"
  };
struct YOKOW_API C_SW : public CAlarmBase, public C_SW_W STD_DEFINE enum { TypeID = id__SW };
  };

////////////////////////////////////////////////////////////////
//YOKO_TYPE( 0202 , _AN     , "%AN"      )
struct YOKOW_API C_AN_W
  {
#include "Yokogawa/_AN.hpp"
  };
struct YOKOW_API C_AN : public CAlarmBase, public C_AN_W STD_DEFINE enum { TypeID = id__AN };
  };

////////////////////////////////////////////////////////////////
//YOKO_TYPE( 0203 , _OG     , "%OG"      )
struct YOKOW_API C_OG_W
  {
#include "Yokogawa/_OG.hpp"
  };

struct YOKOW_API C_OG : public CAlarmBase, public C_OG_W STD_DEFINE enum { TypeID = id__OG };
  };
////////////////////////////////////////////////////////////////
//YOKO_TYPE( 0302 , _RQ     , "%RQ"      )
struct YOKOW_API C_RQ_W
  {
#include "Yokogawa/_RQ.hpp"
  };

struct YOKOW_API C_RQ : public CAlarmBase, public C_RQ_W STD_DEFINE enum { TypeID = id__RQ };
  };
////////////////////////////////////////////////////////////////
//YOKO_TYPE( 0280 , _Z      , "%Z"       )
struct YOKOW_API C_Z_W
  {
#include "Yokogawa/_Z.hpp"
  };
struct YOKOW_API C_Z : public CAlarmBase, public C_Z_W STD_DEFINE enum { TypeID = id__Z };
  };
////////////////////////////////////////////////////////////////
//YOKO_TYPE( 0290 , _Y      , "%Y"       )
struct YOKOW_API C_Y_W
  {
#include "Yokogawa/_Y.hpp"
  };
struct YOKOW_API C_Y : public CAlarmBase, public C_Y_W STD_DEFINE enum { TypeID = id__Y };
  };
////////////////////////////////////////////////////////////////
//YOKO_TYPE( 5050 , _WW      , "%WW"       )
struct YOKOW_API C_WW_W
  {
#include "Yokogawa/_WW.hpp"
  };
struct YOKOW_API C_WW : public CAlarmBase, public C_WW_W STD_DEFINE enum { TypeID = id__WW };
  };
////////////////////////////////////////////////////////////////
  //YOKO_TYPE( 5052 , _XW      , "%XW"       )
  struct YOKOW_API C_XW_W
  {
#include "Yokogawa/_XW.hpp"
  };
  struct YOKOW_API C_XW : public CAlarmBase, public C_XW_W STD_DEFINE enum { TypeID = id__XW };
  };
////////////////////////////////////////////////////////////////
struct YOKOW_API C_PR_W
  {
#include "Yokogawa/_PR.hpp"
  };
struct YOKOW_API C_PR : public CAlarmBase, public C_PR_W STD_DEFINE enum { TypeID = id__PR };
  };
//YOKO_TYPE( 1000 , PVI, "PVI" )
struct YOKOW_API CPVI_P
	{
  #include "Yokogawa/PVI_P.hpp"
	};
struct YOKOW_API CPVI_W
	{
  #include "Yokogawa/PVI.hpp"
	};
struct YOKOW_API CPVI : public CAlarmBase, public CPVI_P, public CPVI_W
{
  STD_DEFINE_WP enum { TypeID = id_PVI };
};
//YOKO_TYPE( 1001 , PVI_DV, "PVI-DV" )
struct YOKOW_API CPVI_DV_P
	{
  #include "Yokogawa/PVI_P.hpp"
	};
struct YOKOW_API CPVI_DV_W
	{
  #include "Yokogawa/PVI_DV.hpp"
	};
struct YOKOW_API CPVI_DV : public CAlarmBase, public CPVI_DV_P, public CPVI_DV_W
{
  STD_DEFINE_WP enum { TypeID = id_PVI_DV };
};
//
struct YOKOW_API CPID_COMM_P
  {
  #include "Yokogawa/PID_P.hpp"
  };

struct YOKOW_API CPID_COMM_W
  {
  #include "Yokogawa/PID.hpp"
  };
struct YOKOW_API CPID_COMM : public CAlarmBase, public CPID_COMM_P, public CPID_COMM_W
  {
  };

//YOKO_TYPE( 1010 , PID     , "PID"      )
struct YOKOW_API CPID : STD_DEFINE_COMM(CPID_COMM) enum { TypeID = id_PID };
  };
//YOKO_TYPE( 1018 , PID_STC , "PID_STC"  ) // ЛЧ-24/7
struct YOKOW_API CPID_STC : STD_DEFINE_COMM(CPID_COMM) enum { TypeID = id_PID_STC };
  #include "Yokogawa/PID-STC.hpp"
  };
//YOKO_TYPE( 1011 , PI_HLD  , "PI-HLD"   )
struct YOKOW_API CPI_HLD_P
  {
  #include "Yokogawa/PI-HLD_P.hpp"
  };
struct YOKOW_API CPI_HLD_W
  {
  #include "Yokogawa/PI-HLD.hpp"
  };
struct YOKOW_API CPI_HLD : public CAlarmBase, public CPI_HLD_P, public CPI_HLD_W 
  {
  STD_DEFINE_WP enum { TypeID = id_PI_HLD };
  };
//YOKO_TYPE( 1013 , ONOFF   , "ONOFF"    ) // Изомеризация
struct YOKOW_API CONOFF_P
  {
  #include "Yokogawa/onoff_p.hpp"
  };
struct YOKOW_API CONOFF_W
  {
  #include "Yokogawa/ONOFF.hpp"
  };
struct YOKOW_API CONOFF : public CAlarmBase, public CONOFF_P, public CONOFF_W 
  {
  STD_DEFINE_WP enum { TypeID = id_ONOFF };
  };
//YOKO_TYPE( 1014 , ONOFF_G   , "ONOFF-G"    )
struct YOKOW_API CONOFF_G_P
{
#include "Yokogawa/ONOFF-G_P.hpp"
};
struct YOKOW_API CONOFF_G_W
{
#include "Yokogawa/ONOFF-G.hpp"
};
struct YOKOW_API CONOFF_G : public CAlarmBase, public CONOFF_G_P, public CONOFF_G_W 
{
  STD_DEFINE_WP enum { TypeID = id_ONOFF_G };
};
//YOKO_TYPE( 1020 , MLD     , "MLD"      )
struct YOKOW_API CMLD_P
  {
  #include "Yokogawa/MLD_P.hpp"
  };
struct YOKOW_API CMLD_W
  {
  #include "Yokogawa/MLD.hpp"
  };
struct YOKOW_API CMLD : public CAlarmBase, public CMLD_P, public CMLD_W 
  {
  STD_DEFINE_WP enum { TypeID = id_MLD };
  };
//YOKO_TYPE( 1021 , MLD_PVI , "MLD-PVI"  )
struct YOKOW_API CMLD_PVI_P
  {
  #include "Yokogawa/mld_pvi_p.hpp"
  };
struct YOKOW_API CMLD_PVI_W
  {
  #include "Yokogawa/MLD_PVI.hpp"
  };
struct YOKOW_API CMLD_PVI : public CAlarmBase, public CMLD_PVI_P, public CMLD_PVI_W 
  {
  STD_DEFINE_WP enum { TypeID = id_MLD_PVI };
  };
//YOKO_TYPE( 1022 , MLD_SW  , "MLD-SW"   )
struct YOKOW_API CMLD_SW_P
  {
  #include "Yokogawa/mld_sw_p.hpp"
  };
struct YOKOW_API CMLD_SW_W
  {
  #include "Yokogawa/MLD_SW.hpp"
  };
struct YOKOW_API CMLD_SW : public CAlarmBase, public CMLD_SW_P, public CMLD_SW_W 
  {
  STD_DEFINE_WP enum { TypeID = id_MLD_SW };
  };
//YOKO_TYPE( 1030 , RATIO   , "RATIO"    )
struct YOKOW_API CRATIO_P
  {
  #include "Yokogawa/RATIO_P.hpp"
  };
struct YOKOW_API CRATIO_W
  {
  #include "Yokogawa/RATIO.hpp"
  };
struct YOKOW_API CRATIO : public CAlarmBase, public CRATIO_P, public CRATIO_W 
  {
  STD_DEFINE_WP enum { TypeID = id_RATIO };
  };
//YOKO_TYPE( 1031 , PG_L13  , "PG-L13"   )
struct YOKOW_API CPG_L13_P
  {
  #include "Yokogawa/pg_l13_p.hpp"
  };
struct YOKOW_API CPG_L13_W
  {
  #include "Yokogawa/PG_L13.hpp"
  };
struct YOKOW_API CPG_L13 : public CAlarmBase, public CPG_L13_P, public CPG_L13_W 
  {
  STD_DEFINE_WP enum { TypeID = id_PG_L13 };
  };
//YOKO_TYPE( 1032 , BSETU_2  , "BSETU-2"   )
struct YOKOW_API CBSETU_2_P
{
#include "Yokogawa/BSETU-2_P.hpp"
};
struct YOKOW_API CBSETU_2_W
{
#include "Yokogawa/BSETU-2.hpp"
};
struct YOKOW_API CBSETU_2 : public CAlarmBase, public CBSETU_2_P, public CBSETU_2_W 
{
  STD_DEFINE_WP enum { TypeID = id_BSETU_2 };
};
//YOKO_TYPE( 1040 , VELLIM  , "VELLIM"   )
struct YOKOW_API CVELLIM_P
  {
  #include "Yokogawa/vellim_p.hpp"
  };
struct YOKOW_API CVELLIM_W
  {
  #include "Yokogawa/VELLIM.hpp"
  };
struct YOKOW_API CVELLIM : public CAlarmBase, public CVELLIM_P, public CVELLIM_W 
  {
  STD_DEFINE_WP enum { TypeID = id_VELLIM };
  };
//
struct YOKOW_API CSS_P
  {
  #include "Yokogawa/SS_P.hpp"
  };
struct YOKOW_API CSS_W
  {
  #include "Yokogawa/SS.hpp"
  };
struct YOKOW_API CSS_COMM : public CAlarmBase, public CSS_P, public CSS_W 
  {
  };
struct YOKOW_API CSS_H : STD_DEFINE_COMM(CSS_COMM) enum { TypeID = id_SS_H }; };// 1050
struct YOKOW_API CSS_M : STD_DEFINE_COMM(CSS_COMM) enum { TypeID = id_SS_M }; };// 1051
struct YOKOW_API CSS_L : STD_DEFINE_COMM(CSS_COMM) enum { TypeID = id_SS_L }; };// 1052
//
struct YOKOW_API CAS_COMM_P
	{
  #include "Yokogawa/AS_P.hpp"
	};
struct YOKOW_API CAS_COMM_W
	{
  #include "Yokogawa/AS.hpp"
	};
struct YOKOW_API CAS_COMM : public CAlarmBase, public CAS_COMM_P, public CAS_COMM_W 
	{
	};
struct YOKOW_API CAS_H : STD_DEFINE_COMM(CAS_COMM) enum { TypeID = id_AS_H }; };// 1053
struct YOKOW_API CAS_M : STD_DEFINE_COMM(CAS_COMM) enum { TypeID = id_AS_M }; };// 1054
struct YOKOW_API CAS_L : STD_DEFINE_COMM(CAS_COMM) enum { TypeID = id_AS_L }; };// 1055
//YOKO_TYPE( 1056 , SS_DUAL , "SS-DUAL"  )
struct YOKOW_API CSS_DUAL_P
  {
  #include "Yokogawa/SS_DUAL_P.hpp"
  };
struct YOKOW_API CSS_DUAL_W
  {
  #include "Yokogawa/SS_DUAL.hpp"
  };
struct YOKOW_API CSS_DUAL : public CAlarmBase, public CSS_DUAL_P, public CSS_DUAL_W 
  {
  STD_DEFINE_WP enum { TypeID = id_SS_DUAL };
  };
//YOKO_TYPE( 1060 , FOUT    , "FOUT"     )// Был неправильный 1012
struct YOKOW_API CFOUT_P
  {
  #include "Yokogawa/FOUT_P.hpp"
  };
struct YOKOW_API CFOUT_W
  {
  #include "Yokogawa/FOUT.hpp"
  };
struct YOKOW_API CFOUT : public CAlarmBase, public CFOUT_P, public CFOUT_W 
  {
  STD_DEFINE_WP enum { TypeID = id_FOUT };
  };
//YOKO_TYPE( 1061 , FFSUM   , "FFSUM"    )
struct YOKOW_API CFFSUM_P
{
#include "Yokogawa/FFSUM_P.hpp"
};
struct YOKOW_API CFFSUM_W
{
#include "Yokogawa/FFSUM.hpp"
};
struct YOKOW_API CFFSUM : public CAlarmBase, public CFFSUM_P, public CFFSUM_W 
{
  STD_DEFINE_WP enum { TypeID = id_FFSUM };
};
//YOKO_TYPE( 1062 , XCPL   , "XCPL"    )
struct YOKOW_API CXCPL_P
{
#include "Yokogawa/XCPL_P.hpp"
};
struct YOKOW_API CXCPL_W
{
#include "Yokogawa/XCPL.hpp"
};
struct YOKOW_API CXCPL : public CAlarmBase, public CXCPL_P, public CXCPL_W 
{
  STD_DEFINE_WP enum { TypeID = id_XCPL };
};
//YOKO_TYPE( 1063 , SPLIT   , "SPLIT"    )
struct YOKOW_API CSPLIT_P
  {
  #include "Yokogawa/SPLIT_P.hpp"
  };
struct YOKOW_API CSPLIT_W
  {
  #include "Yokogawa/SPLIT.hpp"
  };
struct YOKOW_API CSPLIT : public CAlarmBase, public CSPLIT_P, public CSPLIT_W 
  {
  STD_DEFINE_WP enum { TypeID = id_SPLIT };
  };
//YOKO_TYPE( 1070 , ALM_R   , "ALM-R"    )
struct YOKOW_API CALM_R_P
{
#include "Yokogawa/ALM-R_P.hpp"
};
struct YOKOW_API CALM_R_W
{
#include "Yokogawa/ALM-R.hpp"
};
struct YOKOW_API CALM_R : public CAlarmBase, public CALM_R_P, public CALM_R_W 
{
  STD_DEFINE_WP enum { TypeID = id_ALM_R };
};
//YOKO_TYPE( 1400 , ST16    , "ST16"     )
struct YOKOW_API CST16_P
  {
  #include "Yokogawa/ST16_P.hpp"
  };
struct YOKOW_API CST16_W
  {
  #include "Yokogawa/ST16.hpp"
  };
struct YOKOW_API CST16 : public CAlarmBase, public CST16_P, public CST16_W 
  {
  STD_DEFINE_WP enum { TypeID = id_ST16 };
  };
struct YOKOW_API CST16E    : STD_DEFINE_COMM(CST16) enum { TypeID = id_ST16E    }; };
//YOKO_TYPE( 1411 , LC64    , "LC64"     )
struct YOKOW_API CLC64_P
  {
  #include "Yokogawa/LC64_P.hpp"
  };
struct YOKOW_API CLC64_W
  {
  #include "Yokogawa/lc64.hpp"
  };
struct YOKOW_API CLC64 : public CAlarmBase, public CLC64_P, public CLC64_W 
  {
  STD_DEFINE_WP enum { TypeID = id_LC64 };
  };
//
struct YOKOW_API CSIO_P
  {
  #include "Yokogawa/SIO_P.hpp"
  };
struct YOKOW_API CSIO_W
  {
  #include "Yokogawa/SIO.hpp"
  };
struct YOKOW_API CSIO_COMM : public CAlarmBase, public CSIO_P, public CSIO_W 
  {
  };

struct YOKOW_API CSI_1    : STD_DEFINE_COMM(CSIO_COMM) enum { TypeID = id_SI_1    }; };// 1420
struct YOKOW_API CSI_2    : STD_DEFINE_COMM(CSIO_COMM) enum { TypeID = id_SI_2    }; };// 1421
struct YOKOW_API CSO_1    : STD_DEFINE_COMM(CSIO_COMM) enum { TypeID = id_SO_1    }; };// 1422
struct YOKOW_API CSO_2    : STD_DEFINE_COMM(CSIO_COMM) enum { TypeID = id_SO_2    }; };// 1423
struct YOKOW_API CSIO_11  : STD_DEFINE_COMM(CSIO_COMM) enum { TypeID = id_SIO_11  }; };// 1424
struct YOKOW_API CSIO_12  : STD_DEFINE_COMM(CSIO_COMM) enum { TypeID = id_SIO_12  }; };// 1425
struct YOKOW_API CSIO_21  : STD_DEFINE_COMM(CSIO_COMM) enum { TypeID = id_SIO_21  }; };// 1426
struct YOKOW_API CSIO_22  : STD_DEFINE_COMM(CSIO_COMM) enum { TypeID = id_SIO_22  }; };// 1427
struct YOKOW_API CSIO_12P : STD_DEFINE_COMM(CSIO_COMM) enum { TypeID = id_SIO_12P }; };// 1428
struct YOKOW_API CSIO_22P : STD_DEFINE_COMM(CSIO_COMM) enum { TypeID = id_SIO_22P }; };// 1429
struct YOKOW_API CSI_1ALM : STD_DEFINE_COMM(CSIO_COMM) enum { TypeID = id_SI_1ALM }; };// 5004
struct YOKOW_API CSI_1E   : STD_DEFINE_COMM(CSIO_COMM) enum { TypeID = id_SI_1E   }; };// 50E4
struct YOKOW_API CSI_2E   : STD_DEFINE_COMM(CSIO_COMM) enum { TypeID = id_SI_2E   }; };// 50E5
struct YOKOW_API CSO_1E   : STD_DEFINE_COMM(CSIO_COMM) enum { TypeID = id_SO_1E   }; };// 50E6
struct YOKOW_API CSO_2E   : STD_DEFINE_COMM(CSIO_COMM) enum { TypeID = id_SO_2E   }; };// 50E7
struct YOKOW_API CSIO_11E : STD_DEFINE_COMM(CSIO_COMM) enum { TypeID = id_SIO_11E }; };// 50E8
struct YOKOW_API CSIO_12E : STD_DEFINE_COMM(CSIO_COMM) enum { TypeID = id_SIO_12E }; };// 50E9
struct YOKOW_API CSIO_21E : STD_DEFINE_COMM(CSIO_COMM) enum { TypeID = id_SIO_21E }; };// 50EA
struct YOKOW_API CSIO_22E : STD_DEFINE_COMM(CSIO_COMM) enum { TypeID = id_SIO_22E }; };// 50EB
struct YOKOW_API CSIO_12PE: STD_DEFINE_COMM(CSIO_COMM) enum { TypeID = id_SIO_12PE}; };// 50EC
struct YOKOW_API CSIO_22PE: STD_DEFINE_COMM(CSIO_COMM) enum { TypeID = id_SIO_22PE}; };// 50ED
//
//YOKO_TYPE( 1431 , TM      , "TM"       ) // ЛЧ-24/7
struct YOKOW_API CTM_P
  {
  #include "Yokogawa/TM_P.hpp"
  };
struct YOKOW_API CTM_W
  {
  #include "Yokogawa/TM.hpp"
  };
struct YOKOW_API CTM : public CAlarmBase, public CTM_P, public CTM_W 
  {
  STD_DEFINE_WP enum { TypeID = id_TM };
  };
struct YOKOW_API CTON_P
  {
  #include "Yokogawa/TON_P.hpp"
  };
struct YOKOW_API CTON_W
  {
  #include "Yokogawa/TON.hpp"
  };
struct YOKOW_API CTON : public CAlarmBase, public CTON_P, public CTON_W 
  {
  STD_DEFINE_WP enum { TypeID = id_TON };
  };
struct YOKOW_API CTOFF_P
  {
  #include "Yokogawa/TOFF_P.hpp"
  };
struct YOKOW_API CTOFF_W
  {
  #include "Yokogawa/TOFF.hpp"
  };
struct YOKOW_API CTOFF : public CAlarmBase, public CTOFF_P, public CTOFF_W 
  {
  STD_DEFINE_WP enum { TypeID = id_TOFF };
  };
struct YOKOW_API CCTS_P
  {
  #include "Yokogawa/CTS_P.hpp"
  };
struct YOKOW_API CCTS_W
  {
  #include "Yokogawa/CTS.hpp"
  };
struct YOKOW_API CCTS : public CAlarmBase, public CCTS_P, public CCTS_W 
  {
  STD_DEFINE_WP enum { TypeID = id_CTS };
  };
//YOKO_TYPE( 1434 , CI      , "CI"       )
struct YOKOW_API CCI_P
  {
  #include "Yokogawa/CI_P.hpp"
  };
struct YOKOW_API CCI_W
  {
  #include "Yokogawa/CI.hpp"
  };
struct YOKOW_API CCI : public CAlarmBase, public CCI_P, public CCI_W 
  {
  STD_DEFINE_WP enum { TypeID = id_CI };
  };
///////////////////////////////////////////////////
//YOKO_TYPE( 1435 , CO      , "CO"       )
struct YOKOW_API CCO_P
  {
  #include "Yokogawa/CO_P.hpp"
  };
struct YOKOW_API CCO_W
  {
  #include "Yokogawa/CO.hpp"
  };
struct YOKOW_API CCO : public CAlarmBase, public CCO_P, public CCO_W 
  {
  STD_DEFINE_WP enum { TypeID = id_CO };
  };
//YOKO_TYPE( 1436 , RL      , "RL"       ) // Изомеризация
struct YOKOW_API CRL_P
  {
  #include "Yokogawa/rl_p.hpp"
  };
struct YOKOW_API CRL_W
  {
  #include "Yokogawa/rl.hpp"
  };
struct YOKOW_API CRL : public CAlarmBase, public CRL_P, public CRL_W 
  {
  STD_DEFINE_WP enum { TypeID = id_RL };
  };
//YOKO_TYPE( 1437 , RS      , "RS"       )
struct YOKOW_API CRS_P
  {
  #include "Yokogawa/RS_P.hpp"
  };
struct YOKOW_API CRS_W
  {
  #include "Yokogawa/RS.hpp"
  };
struct YOKOW_API CRS : public CAlarmBase, public CRS_P, public CRS_W 
  {
  STD_DEFINE_WP enum { TypeID = id_RS };
  };

struct YOKOW_API CARITH_P
  {
  #include "Yokogawa/ARITH_P.hpp"
  };
struct YOKOW_API CARITH_W
  {
  #include "Yokogawa/ARITH.hpp"
  };

struct YOKOW_API CARITH : public CAlarmBase, public CARITH_P, public CARITH_W 
  {
  };

struct YOKOW_API CGE  : STD_DEFINE_COMM(CARITH) enum { TypeID = id_GE   }; };
struct YOKOW_API CGT  : STD_DEFINE_COMM(CARITH) enum { TypeID = id_GT   }; };
struct YOKOW_API CAND : STD_DEFINE_COMM(CARITH) enum { TypeID = id_AND   }; };
struct YOKOW_API COR  : STD_DEFINE_COMM(CARITH) enum { TypeID = id_OR   }; };
struct YOKOW_API CNOT : STD_DEFINE_COMM(CARITH) enum { TypeID = id_NOT   }; };
struct YOKOW_API CADD : STD_DEFINE_COMM(CARITH) enum { TypeID = id_ADD   }; };
struct YOKOW_API CDIV : STD_DEFINE_COMM(CARITH) enum { TypeID = id_DIV   }; };
struct YOKOW_API CSQRT: STD_DEFINE_COMM(CARITH) enum { TypeID = id_SQRT  }; };
struct YOKOW_API CMUL : STD_DEFINE_COMM(CARITH) enum { TypeID = id_MUL   }; };
struct YOKOW_API CEQ  : STD_DEFINE_COMM(CARITH) enum { TypeID = id_EQ   }; };

struct YOKOW_API CLD_P
{
  #include "Yokogawa/LD_P.hpp"
};
struct YOKOW_API CLD_W
{
  #include "Yokogawa/LD.hpp"
};

struct YOKOW_API CLD : public CAlarmBase, public CLD_P, public CLD_W 
{
  STD_DEFINE_WP enum { TypeID = id_LD };
};

struct YOKOW_API CAVE_P
  {
  #include "Yokogawa/AVE_P.hpp"
  };
struct YOKOW_API CAVE_W
  {
  #include "Yokogawa/AVE.hpp"
  };
//YOKO_TYPE( 1803 , AVE     , "AVE"      )
struct YOKOW_API CAVE : public CAlarmBase, public CAVE_P, public CAVE_W 
  {
  STD_DEFINE_WP enum { TypeID = id_AVE };
  };
//YOKO_TYPE( 1812 , LAG     , "LAG"      )
struct YOKOW_API CLAG_P
  {
  #include "Yokogawa/LAG_P.hpp"
  };
struct YOKOW_API CLAG_W
  {
  #include "Yokogawa/LAG.hpp"
  };
struct YOKOW_API CLAG : public CAlarmBase, public CLAG_P, public CLAG_W 
  {
  STD_DEFINE_WP enum { TypeID = id_LAG };
  };
//YOKO_TYPE( 1813 , INTEG   , "INTEG"    )
struct YOKOW_API CINTEG_P
{
#include "Yokogawa/INTEG_P.hpp"
};
struct YOKOW_API CINTEG_W
{
#include "Yokogawa/INTEG.hpp"
};
struct YOKOW_API CINTEG : public CAlarmBase, public CINTEG_P, public CINTEG_W 
{
  STD_DEFINE_WP enum { TypeID = id_INTEG };
};
//YOKO_TYPE( 1815 , RAMP    , "RAMP"     )
struct YOKOW_API CRAMP_P
{
#include "Yokogawa/RAMP_P.hpp"
};
struct YOKOW_API CRAMP_W
{
#include "Yokogawa/RAMP.hpp"
};
struct YOKOW_API CRAMP : public CAlarmBase, public CRAMP_P, public CRAMP_W 
{
  STD_DEFINE_WP enum { TypeID = id_RAMP };
};
//YOKO_TYPE( 1816 , LDLAG   , "LDLAG"    )
struct YOKOW_API CLDLAG_P
{
#include "Yokogawa/LDLAG_P.hpp"
};
struct YOKOW_API CLDLAG_W
{
#include "Yokogawa/LDLAG.hpp"
};
struct YOKOW_API CLDLAG : public CAlarmBase, public CLDLAG_P, public CLDLAG_W 
{
  STD_DEFINE_WP enum { TypeID = id_LDLAG };
};
//YOKO_TYPE( 1817 , DLAY    , "DLAY"     )
struct YOKOW_API CDLAY_P
  {
  #include "Yokogawa/DLAY_P.hpp"
  };
struct YOKOW_API CDLAY_W
  {
  #include "Yokogawa/DLAY.hpp"
  };
struct YOKOW_API CDLAY : public CAlarmBase, public CDLAY_P, public CDLAY_W 
  {
  STD_DEFINE_WP enum { TypeID = id_DLAY };
  };
//YOKO_TYPE( 1818 , DLAY_C    , "DLAY-C"     )
struct YOKOW_API CDLAY_C_P
{
#include "Yokogawa/DLAY_C_P.hpp"
};
struct YOKOW_API CDLAY_C_W
{
#include "Yokogawa/DLAY_C.hpp"
};
struct YOKOW_API CDLAY_C : public CAlarmBase, public CDLAY_C_P, public CDLAY_C_W 
{
  STD_DEFINE_WP enum { TypeID = id_DLAY_C };
};
//YOKO_TYPE( 181C , FUNC_VAR, "FUNC-VAR" ) // ЛЧ-24/7 (не обязятельно, это что-то АСУ-шное)
struct YOKOW_API CFUNC_VAR_P
  {
  #include "Yokogawa/FUNC_VAR_P.hpp"
  };
struct YOKOW_API CFUNC_VAR_W
  {
  #include "Yokogawa/FUNC_VAR.hpp"
  };
struct YOKOW_API CFUNC_VAR : public CAlarmBase, public CFUNC_VAR_P, public CFUNC_VAR_W 
  {
  STD_DEFINE_WP enum { TypeID = id_FUNC_VAR };
  };
//YOKO_TYPE( 181D , TPCFL   , "TPCFL"    )
struct YOKOW_API CTPCFL_P
  {
  #include "Yokogawa/TPCFL_P.hpp"
  };
struct YOKOW_API CTPCFL_W
  {
  #include "Yokogawa/TPCFL.hpp"
  };
struct YOKOW_API CTPCFL : public CAlarmBase, public CTPCFL_P, public CTPCFL_W 
  {
  STD_DEFINE_WP enum { TypeID = id_TPCFL };
  };
//YOKO_TYPE( 181E , ASTM1   , "ASTM1"    )
struct YOKOW_API CASTM1_P
{
#include "Yokogawa/ASTM1_P.hpp"
};
struct YOKOW_API CASTM1_W
{
#include "Yokogawa/ASTM1.hpp"
};
struct YOKOW_API CASTM1 : public CAlarmBase, public CASTM1_P, public CASTM1_W 
{
  STD_DEFINE_WP enum { TypeID = id_ASTM1 };
};
//YOKO_TYPE( 181F , ASTM2   , "ASTM2"    )
struct YOKOW_API CASTM2_P
{
#include "Yokogawa/ASTM2_P.hpp"
};
struct YOKOW_API CASTM2_W
{
#include "Yokogawa/ASTM2.hpp"
};
struct YOKOW_API CASTM2 : public CAlarmBase, public CASTM2_P, public CASTM2_W 
{
  STD_DEFINE_WP enum { TypeID = id_ASTM2 };
};
//YOKO_TYPE( 1835 , SRS2_S  , "SRS2-S"   )
struct YOKOW_API CSRSN_X_P
{
#include "Yokogawa/SRSn_X_P.hpp"
};
struct YOKOW_API CSRSN_X_W
{
#include "Yokogawa/SRSn_X.hpp"
};
struct YOKOW_API CSRSN_X : public CAlarmBase, public CSRSN_X_P, public CSRSN_X_W {};

//YOKO_TYPE( 1833 , SRS1_S  , "SRS1-S"   )
struct YOKOW_API CSRS1_S : CSRSN_X 
{
  STD_DEFINE_WP enum { TypeID = id_SRS1_S };
};
//YOKO_TYPE( 1834 , SRS1_R  , "SRS1-R"   )
struct YOKOW_API CSRS1_R : CSRSN_X 
{
  STD_DEFINE_WP enum { TypeID = id_SRS1_R };
};
//YOKO_TYPE( 1835 , SRS2_S  , "SRS2-S"   )
struct YOKOW_API CSRS2_S : CSRSN_X 
{
  STD_DEFINE_WP enum { TypeID = id_SRS2_S };
};
//YOKO_TYPE( 1836 , SRS2_R  , "SRS2-R"   )
struct YOKOW_API CSRS2_R : CSRSN_X 
{
  STD_DEFINE_WP enum { TypeID = id_SRS2_R };
};
//YOKO_TYPE( 1837 , WOUT    , "WOUT"     )
/*struct YOKOW_API CWOUT_P
{
#include "Yokogawa/WOUT_P.hpp"
};*/
struct YOKOW_API CWOUT_W
{
#include "Yokogawa/WOUT.hpp"
};
struct YOKOW_API CWOUT : public CAlarmBase/*, public CWOUT_P*/, public CWOUT_W 
{
  STD_DEFINE_WP enum { TypeID = id_WOUT };
};
//////////////////////////////////////////////////////////////////////////
//YOKO_TYPE( 1838 , OND     , "OND"      ) //Sergej
struct YOKOW_API COND_P
  {
  #include "Yokogawa/OND_P.hpp"
  };
struct YOKOW_API COND_W
  {
  #include "Yokogawa/OND.hpp"
  };
struct YOKOW_API COND : public CAlarmBase, public COND_P, public COND_W 
  {
  STD_DEFINE_WP enum { TypeID = id_OND };
  };
//YOKO_TYPE( 1839 , OFFD    , "OFFD"     ) //Sergej
struct YOKOW_API COFFD_P
  {
  #include "Yokogawa/OFFD_P.hpp"
  };
struct YOKOW_API COFFD_W
  {
  #include "Yokogawa/OFFD.hpp"
  };
struct YOKOW_API COFFD : public CAlarmBase, public COFFD_P, public COFFD_W 
  {
  STD_DEFINE_WP enum { TypeID = id_OFFD };
  };
//YOKO_TYPE( 183F , BAND    , "BAND"     )
struct YOKOW_API CBAND_P
  {
  #include "Yokogawa/BAND_P.hpp"
  };
struct YOKOW_API CBAND_W
  {
  #include "Yokogawa/BAND.hpp"
  };
struct YOKOW_API CBAND : public CAlarmBase, public CBAND_P, public CBAND_W
  {
  STD_DEFINE_WP enum { TypeID = id_BAND };
  };
//YOKO_TYPE( 1840 , BOR     , "BOR"      )
struct YOKOW_API CBOR_P
  {
  #include "Yokogawa/BAND_P.hpp"
  };
struct YOKOW_API CBOR_W
  {
  #include "Yokogawa/BAND.hpp"
  };
struct YOKOW_API CBOR : public CAlarmBase, public CBOR_P, public CBOR_W 
  {
  STD_DEFINE_WP enum { TypeID = id_BOR };
  };
//YOKO_TYPE( 1841 , BNOT    , "BNOT"     )
struct YOKOW_API CBNOT_P
  {
  #include "Yokogawa/BNOT_P.hpp"
  };
struct YOKOW_API CBNOT_W
  {
  #include "Yokogawa/BNOT.hpp"
  };
struct YOKOW_API CBNOT : public CAlarmBase, public CBNOT_P, public CBNOT_W 
  {
  STD_DEFINE_WP enum { TypeID = id_BNOT };
  };
//
struct YOKOW_API CCALCU_P
  {
  #include "Yokogawa/calcu_p.hpp"
  };
struct YOKOW_API CCALCU_W
  {
  #include "Yokogawa/CALCU.hpp"
  };
struct YOKOW_API CCALCU : public CAlarmBase, public CCALCU_P, public CCALCU_W 
{
  STD_DEFINE_WP enum { TypeID = id_CALCU   };// 1850
};
//
struct YOKOW_API CCALCU_C_P
{
#include "Yokogawa/CALCU_C_P.hpp"
};
struct YOKOW_API CCALCU_C_W
{
#include "Yokogawa/calcu_c.hpp"
};
struct YOKOW_API CCALCU_C : public CAlarmBase, public CCALCU_C_P, public CCALCU_C_W 
{
  STD_DEFINE_WP enum { TypeID = id_CALCU_C };// 1851
};
//
//YOKO_TYPE( 1870 , SW_33   , "SW-33"    )//был неправильный 1875
struct YOKOW_API CSW_33_P
  {
  #include "Yokogawa/SW_33_P.hpp"
  };
struct YOKOW_API CSW_33_W
  {
  #include "Yokogawa/SW_33.hpp"
  };
struct YOKOW_API CSW_33 : public CAlarmBase, public CSW_33_P, public CSW_33_W 
  {
  STD_DEFINE_WP enum { TypeID = id_SW_33 };
  };
//YOKO_TYPE( 1871 , SW_91   , "SW-91"    )
struct YOKOW_API CSW_91_P
  {
  #include "Yokogawa/SW_91_P.hpp"
  };
struct YOKOW_API CSW_91_W
  {
  #include "Yokogawa/SW_91.hpp"
  };
struct YOKOW_API CSW_91 : public CAlarmBase, public CSW_91_P, public CSW_91_W 
  {
  STD_DEFINE_WP enum { TypeID = id_SW_91 };
  };
//YOKO_TYPE( 1872 , DSW_16  , "DSW-16"   ) //Sergej
struct YOKOW_API CDSW_16_P
  {
  #include "Yokogawa/DSW_16_P.hpp"
  };
struct YOKOW_API CDSW_16_W
  {
  #include "Yokogawa/DSW_16.hpp"
  };
struct YOKOW_API CDSW_16 : public CAlarmBase, public CDSW_16_P, public CDSW_16_W 
  {
  STD_DEFINE_WP enum { TypeID = id_DSW_16 };
  };
//YOKO_TYPE( 1873 , DSW_16C  , "DSW-16C"   )
struct YOKOW_API CDSW_16C_P
{
#include "Yokogawa/DSW_16C_P.hpp"
};
struct YOKOW_API CDSW_16C_W
{
#include "Yokogawa/DSW_16C.hpp"
};
struct YOKOW_API CDSW_16C : public CAlarmBase, public CDSW_16C_P, public CDSW_16C_W 
{
  STD_DEFINE_WP enum { TypeID = id_DSW_16C };
};

//YOKO_TYPE( 1874 , DSET    , "DSET"     )
struct YOKOW_API CDSET_P
  {
  #include "Yokogawa/DSET_P.hpp"
  };
struct YOKOW_API CDSET_W
  {
  #include "Yokogawa/dset.hpp"
  };
struct YOKOW_API CDSET : public CAlarmBase, public CDSET_P, public CDSET_W 
  {
  STD_DEFINE_WP enum { TypeID = id_DSET };
  };
//YOKO_TYPE( 1875 , DSET_PVI , "DSET-PVI"     )
struct YOKOW_API CDSET_PVI_P
{
#include "Yokogawa/DSET_PVI_P.hpp"
};
struct YOKOW_API CDSET_PVI_W
{
#include "Yokogawa/DSET_PVI.hpp"
};
struct YOKOW_API CDSET_PVI : public CAlarmBase, public CDSET_PVI_P, public CDSET_PVI_W 
{
  STD_DEFINE_WP enum { TypeID = id_DSET_PVI };
};
//YOKO_TYPE( 1877 , BDSET_1C, "BDSET-1C" ) // ЛЧ-24/7
struct YOKOW_API CBDSET_1C_P
  {
  #include "Yokogawa/BDSET_1C_P.hpp"
  };
struct YOKOW_API CBDSET_1C_W
  {
  #include "Yokogawa/BDSET_1C.hpp"
  };
struct YOKOW_API CBDSET_1C : public CAlarmBase, public CBDSET_1C_P, public CBDSET_1C_W 
  {
  STD_DEFINE_WP enum { TypeID = id_BDSET_1C };
  };
//YOKO_TYPE( 1879 , BDSET_2C, "BDSET-2C" )
struct YOKOW_API CBDSET_2C_P
  {
  #include "Yokogawa/BDSET_2C_P.hpp"
  };
struct YOKOW_API CBDSET_2C_W
  {
  #include "Yokogawa/BDSET_2C.hpp"
  };
struct YOKOW_API CBDSET_2C : public CAlarmBase, public CBDSET_2C_P, public CBDSET_2C_W 
  {
  STD_DEFINE_WP enum { TypeID = id_BDSET_2C };
  };
//YOKO_TYPE( 1D80 , BDSET_1L, "BDSET-1L" ) // ЛЧ-24/7
struct YOKOW_API CBDSET_1L_P
  {
  #include "Yokogawa/BDSET_1L_P.hpp"
  };
struct YOKOW_API CBDSET_1L_W
  {
  #include "Yokogawa/BDSET_1L.hpp"
  };
struct YOKOW_API CBDSET_1L : public CAlarmBase, public CBDSET_1L_P, public CBDSET_1L_W 
  {
  STD_DEFINE_WP enum { TypeID = id_BDSET_1L };
  };
//YOKO_TYPE( 1D81 , BDSET_2L, "BDSET-2L" )
struct YOKOW_API CBDSET_2L_P
  {
  #include "Yokogawa/BDSET_2L_P.hpp"
  };
struct YOKOW_API CBDSET_2L_W
  {
  #include "Yokogawa/BDSET_2L.hpp"
  };
struct YOKOW_API CBDSET_2L : public CAlarmBase, public CBDSET_2L_P, public CBDSET_2L_W 
  {
  STD_DEFINE_WP enum { TypeID = id_BDSET_2L };
  };
//YOKO_TYPE( 1D82 , BDA_L   , "BDA-L"    )
struct YOKOW_API CBDA_L_P
{
#include "Yokogawa/BDA-L_P.hpp"
};
struct YOKOW_API CBDA_L_W
{
#include "Yokogawa/BDA-L.hpp"
};
struct YOKOW_API CBDA_L : public CAlarmBase, public CBDA_L_P, public CBDA_L_W 
{
  STD_DEFINE_WP enum { TypeID = id_BDA_L };
};
//YOKO_TYPE( 187B , BDA_C   , "BDA-C"    )
struct YOKOW_API CBDA_C_P
{
#include "Yokogawa/BDA-C_P.hpp"
};
struct YOKOW_API CBDA_C_W
{
#include "Yokogawa/BDA-C.hpp"
};
struct YOKOW_API CBDA_C : public CAlarmBase, public CBDA_C_P, public CBDA_C_W 
{
  STD_DEFINE_WP enum { TypeID = id_BDA_C };
};
//YOKO_TYPE( 1C00 , INDST2  , "INDST2"   )
struct YOKOW_API CINDST2_P
{
#include "Yokogawa/INDST2_P.hpp"
};
struct YOKOW_API CINDST2_W
{
#include "Yokogawa/INDST2.hpp"
};
struct YOKOW_API CINDST2 : public CAlarmBase, public CINDST2_P, public CINDST2_W 
{
  STD_DEFINE_WP enum { TypeID = id_INDST2 };
};
//YOKO_TYPE( 1C01 , INDST2S , "INDST2S"  ) // ЛЧ-24/7
struct YOKOW_API CINDST2S_W
{
  #include "Yokogawa/INDST2S.hpp"
  };
struct YOKOW_API CINDST2S_P
{
#include "Yokogawa/INDST2S_P.hpp"
};
struct YOKOW_API CINDST2S : public CAlarmBase, CINDST2S_P, CINDST2S_W STD_DEFINE enum { TypeID = id_INDST2S };
  };
//YOKO_TYPE( 1C02 , INDST3  , "INDST3"   )
struct YOKOW_API CINDST3_P
  {
  #include "Yokogawa/INDST3_P.hpp"
  };
struct YOKOW_API CINDST3_W
  {
  #include "Yokogawa/INDST3.hpp"
  };
struct YOKOW_API CINDST3 : public CAlarmBase, public CINDST3_P, public CINDST3_W 
  {
  STD_DEFINE_WP enum { TypeID = id_INDST3 };
  };
//YOKO_TYPE( 1E20 , PTC     , "PTC"      )
struct YOKOW_API CPTC_P
{
#include "Yokogawa/PTC_P.hpp"
};
struct YOKOW_API CPTC_W
{
#include "Yokogawa/PTC.hpp"
};
struct YOKOW_API CPTC : public CAlarmBase, public CPTC_P, public CPTC_W 
{
  STD_DEFINE_WP enum { TypeID = id_PTC };
};
//YOKO_TYPE( 1E21 , PBS5C   , "PBS5C"    )
struct YOKOW_API CPBS5C_P
  {
  #include "Yokogawa/PBS5C_P.hpp"
  };
struct YOKOW_API CPBS5C_W
  {
  #include "Yokogawa/PBS5C.hpp"
  };
struct YOKOW_API CPBS5C : public CAlarmBase, public CPBS5C_P, public CPBS5C_W 
  {
  STD_DEFINE_WP enum { TypeID = id_PBS5C };
  };
//YOKO_TYPE( 1E22 , PBS10C  , "PBS10C"   )
struct YOKOW_API CPBS10C_P
{
#include "Yokogawa/PBS10C_P.hpp"
};
struct YOKOW_API CPBS10C_W
{
#include "Yokogawa/PBS10C.hpp"
};
struct YOKOW_API CPBS10C : public CAlarmBase, public CPBS10C_P, public CPBS10C_W 
{
  STD_DEFINE_WP enum { TypeID = id_PBS10C };
};
//YOKO_TYPE( 1E23 , HAS3C   , "HAS3C"    )
struct YOKOW_API CHAS3C_P
  {
  #include "Yokogawa/HAS3C_P.hpp"
  };
struct YOKOW_API CHAS3C_W
  {
  #include "Yokogawa/HAS3C.hpp"
  };
struct YOKOW_API CHAS3C : public CAlarmBase, public CHAS3C_P, public CHAS3C_W 
  {
  STD_DEFINE_WP enum { TypeID = id_HAS3C };
  };
//YOKO_TYPE( 5051 , _WB     , "%WB"      )
struct YOKOW_API C_WB_W
{
  #include "Yokogawa/_WB.hpp"
};
struct YOKOW_API C_WB : public CAlarmBase, C_WB_W STD_DEFINE enum { TypeID = id__WB };
  };
//
struct YOKOW_API CMC_NE_P
  {
  #include "Yokogawa/MC_P.hpp"
  };
struct YOKOW_API CMC_NE_W
  {
  #include "Yokogawa/MC.hpp"
  };
struct YOKOW_API CMC_NE_COMM : public CAlarmBase, public CMC_NE_P, public CMC_NE_W 
  {
  };
struct YOKOW_API CMC_2  : STD_DEFINE_COMM(CMC_NE_COMM) enum { TypeID = id_MC_2  }; };// 1023
struct YOKOW_API CMC_3  : STD_DEFINE_COMM(CMC_NE_COMM) enum { TypeID = id_MC_3  }; };// 1024
struct YOKOW_API CMC_2E : STD_DEFINE_COMM(CMC_NE_COMM) enum { TypeID = id_MC_2E }; };// 50E2
struct YOKOW_API CMC_3E : STD_DEFINE_COMM(CMC_NE_COMM) enum { TypeID = id_MC_3E }; };// 50E3
//
// Настраиваемые типы (замечены номера min=0x2200, max=0x2909 )
struct SUserDef
{
  char szDataItem[20];
  struct STypeSize
  {
    BYTE type;
    BYTE sizeinBytes;
  };
  union
  {
    USHORT DataType;
    STypeSize sizedType;
  };
  USHORT Array1, Array2;
  char szComment[36];
  BYTE Check;
  USHORT offsetInUSER;
  USHORT elementSize;
  char szEngUnit[16];//88
};

template<size_t _C>
struct YOKOW_API CSFC_BASE
{
  SUserDef UserDefinition[_C];
};

struct YOKOW_API CSFCSW_W
{
#include "Yokogawa/_SFCSW.hpp"
};

struct YOKOW_API CSFCSW_P : public CSFC_BASE<32>
{
};
struct YOKOW_API C_SFCSW : public CAlarmBase, public  CSFCSW_P, public  CSFCSW_W
{
  STD_DEFINE_WP enum { TypeID = id__SFCSW };
};
//////////////////////////////////////////////////////////////////////////
struct YOKOW_API CSFCPB_W
{
#include "Yokogawa/_SFCPB.hpp"
};

struct YOKOW_API CSFCPB_P : public CSFC_BASE<32>
{
};
struct YOKOW_API C_SFCPB : public CAlarmBase, public  CSFCPB_P, public  CSFCPB_W
{
  STD_DEFINE_WP enum { TypeID = id__SFCPB };
};
//////////////////////////////////////////////////////////////////////////
struct YOKOW_API CUTAS_W
{
#include "Yokogawa/_UTAS.hpp"
};

struct YOKOW_API CUTAS_P : public CSFC_BASE<256>
{
  #include "Yokogawa/_UTAS_P.hpp"
};

struct YOKOW_API C_UTAS : public CAlarmBase, public  CUTAS_P, public  CUTAS_W
{
  STD_DEFINE_WP enum { TypeID = id__UTAS };
};
//////////////////////////////////////////////////////////////////////////
struct YOKOW_API CUTSW_W
{
#include "Yokogawa/_UTSW.hpp"
};

struct YOKOW_API CUTSW_P : public CSFC_BASE<256>
{
#include "Yokogawa/_UTSW_P.hpp"
};

struct YOKOW_API C_UTSW : public CAlarmBase, public  CUTSW_P, public  CUTSW_W
{
  STD_DEFINE_WP enum { TypeID = id__UTSW };
};
//////////////////////////////////////////////////////////////////////////
struct YOKOW_API CSFCAS_W
{
#include "Yokogawa/_SFCAS.hpp"
};

struct YOKOW_API CSFCAS_P : public CSFC_BASE<32>
{
  #include "Yokogawa/_SFCAS_P.hpp"
};
struct YOKOW_API C_SFCAS : public CAlarmBase, CSFCAS_P, CSFCAS_W
{
  STD_DEFINE_WP enum { TypeID = id__SFCAS };
};
//////////////////////////////////////////////////////////////////////////
struct YOKOW_API CUTPB_W
{
#include "Yokogawa/_UTPB.hpp"
};

struct YOKOW_API CUTPB_P : public CSFC_BASE<256>
{
#include "Yokogawa/_UTPB_P.hpp"
};

struct YOKOW_API C_UTPB : public CAlarmBase, public CUTPB_P, public CUTPB_W
{
  STD_DEFINE_WP enum { TypeID = id__UTPB };
};
//////////////////////////////////////////////////////////////////////////
struct YOKOW_API CUTPB_N_W
{
#include "Yokogawa/_UTPB_N.hpp"
};

struct YOKOW_API CUTPB_N_P : public CSFC_BASE<256>
{
#include "Yokogawa/_UTPB_N_P.hpp"
};

struct YOKOW_API C_UTPB_N : public CAlarmBase, public  CUTPB_N_P, public  CUTPB_N_W
{
  STD_DEFINE_WP enum { TypeID = id__UTPB_N };
};
//////////////////////////////////////////////////////////////////////////
struct YOKOW_API COPSFC_W
{
#include "Yokogawa/OPSFC.hpp"
};

struct YOKOW_API COPSFC_P
{
#include "Yokogawa/OPSFC_P.hpp"
};

struct YOKOW_API COPSFC : public CAlarmBase, public  COPSFC_P, public  COPSFC_W
{
  STD_DEFINE_WP enum { TypeID = id_OPSFC };
};
//////////////////////////////////////////////////////////////////////////
struct YOKOW_API COPSBL_W
{
#include "Yokogawa/OPSBL.hpp"
};

struct YOKOW_API COPSBL_P
{
#include "Yokogawa/OPSBL_P.hpp"
};

struct YOKOW_API COPSBL : public CAlarmBase, public  COPSBL_P, public  COPSBL_W
{
  STD_DEFINE_WP enum { TypeID = id_OPSBL };
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
struct YOKOW_API CS_ANLGI_P
{
  #include "Yokogawa/S_ANLGI_P.hpp"
};

struct YOKOW_API CS_ANLGI_W
{
  #include "Yokogawa/S_ANLGI.hpp"
};

struct YOKOW_API CS_ANLGI_COMM : public CAlarmBase, public CS_ANLGI_W, public CS_ANLGI_P
{
};

struct YOKOW_API CS_ANLGI : STD_DEFINE_COMM(CS_ANLGI_COMM) enum { TypeID = id_S_ANLGI };
  };

struct YOKOW_API CS_MOB_21_W
{
  #include "Yokogawa/S_MOB_21.hpp"
};
struct YOKOW_API CS_MOB_21_P
{
#include "Yokogawa/S_MOB_21_P.hpp"
};
struct YOKOW_API CS_MOB_21 : public CAlarmBase, CS_MOB_21_W, CS_MOB_21_P STD_DEFINE enum { TypeID = id_S_MOB_21 };
  };

struct YOKOW_API CS_MOB_RS_W
{
  #include "Yokogawa/S_MOB_RS.hpp"
};
struct YOKOW_API CS_MOB_RS_P
{
#include "Yokogawa/S_MOB_RS_P.hpp"
};
struct YOKOW_API CS_MOB_RS : public CAlarmBase, CS_MOB_RS_W, CS_MOB_RS_P STD_DEFINE enum { TypeID = id_S_MOB_RS };
  };

struct YOKOW_API CS_VEL_W
{
  #include "Yokogawa/S_VEL.hpp"
};
struct YOKOW_API CS_VEL_P
{
#include "Yokogawa/S_VEL_P.hpp"
};
struct YOKOW_API CS_VEL : public CAlarmBase, CS_VEL_W, CS_VEL_P STD_DEFINE enum { TypeID = id_S_VEL };
  };

struct YOKOW_API CS_MOB_11_W
{
  #include "Yokogawa/S_MOB_11.hpp"
};
struct YOKOW_API CS_MOB_11_P
{
#include "Yokogawa/S_MOB_11_P.hpp"
};
struct YOKOW_API CS_MOB_11 : public CAlarmBase, CS_MOB_11_W, CS_MOB_11_P STD_DEFINE enum { TypeID = id_S_MOB_11 };
  };

struct YOKOW_API CS_OVR_B_W
{
#include "Yokogawa/S_OVR_B.hpp"
};
struct YOKOW_API CS_OVR_B_P
{
#include "Yokogawa/S_OVR_B_P.hpp"
};
struct YOKOW_API CS_OVR_B : public CAlarmBase, CS_OVR_B_W, CS_OVR_B_P STD_DEFINE enum { TypeID = id_S_OVR_B };
};

struct YOKOW_API CS_ANLG_S_W
{
#include "Yokogawa/S_ANLG_S.hpp"
};
struct YOKOW_API CS_ANLG_S_P
{
#include "Yokogawa/S_ANLG_S_P.hpp"
};
struct YOKOW_API CS_ANLG_S : public CAlarmBase, CS_ANLG_S_W, CS_ANLG_S_P STD_DEFINE enum { TypeID = id_S_ANLG_S };
  };

struct YOKOW_API CANN_FUP_W
{
#include "Yokogawa/ANN_FUP.hpp"
};
struct YOKOW_API CANN_FUP_P
{
#include "Yokogawa/ANN_FUP_P.hpp"
};
struct YOKOW_API CANN_FUP : public CAlarmBase, CANN_FUP_W, CANN_FUP_P STD_DEFINE enum { TypeID = id_ANN_FUP };
  };

struct YOKOW_API CS_MOA_W
{
#include "Yokogawa/S_MOA.hpp"
};
struct YOKOW_API CS_S_MOA_P
{
#include "Yokogawa/S_MOA_P.hpp"
};
struct YOKOW_API CS_MOA : public CAlarmBase, CS_MOA_W, CS_S_MOA_P STD_DEFINE enum { TypeID = id_S_MOA };
  };

struct YOKOW_API CAVE_M_P
  {
  #include "Yokogawa/AVE_M_P.hpp"
  };
struct YOKOW_API CAVE_M_W
  {
  #include "Yokogawa/AVE_M.hpp"
  };
//YOKO_TYPE( 1819 , AVE_M   , "AVE-M"    )
struct YOKOW_API CAVE_M : public CAlarmBase, public CAVE_M_P, public CAVE_M_W 
  {
  STD_DEFINE_WP enum { TypeID = id_AVE_M };
  };

struct YOKOW_API CAVE_C_P
  {
  #include "Yokogawa/AVE_C_P.hpp"
  };
struct YOKOW_API CAVE_C_W
  {
  #include "Yokogawa/AVE_C.hpp"
  };
//YOKO_TYPE( 181A , AVE_C   , "AVE-C"    )
struct YOKOW_API CAVE_C : public CAlarmBase, public CAVE_C_P, public CAVE_C_W 
  {
  STD_DEFINE_WP enum { TypeID = id_AVE_C };
  };
//////////////////////////////////////////////////////////////////////////
struct YOKOW_API CS_PASSWD_W
{
#include "Yokogawa/S_PASSWD.hpp"
};
struct YOKOW_API CS_PASSWD_P
{
#include "Yokogawa/S_PASSWD_P.hpp"
};

struct YOKOW_API CS_PASSWD : public CAlarmBase, public CS_PASSWD_W, public CS_PASSWD_P 
{
  STD_DEFINE_WP enum { TypeID = id_S_PASSWD };
};
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
struct YOKOW_API CFF_AI_P
  {
#include "Yokogawa/FF_AI_P.hpp"
  };

struct YOKOW_API CFF_AI_W
  {
#include "Yokogawa/FF_AI.hpp"
  };

struct YOKOW_API CFF_AI : public CAlarmBase, public CFF_AI_P, public CFF_AI_W STD_DEFINE enum { TypeID = id_FF_AI };
  };
//////////////////////////////////////////////////////////////////////////////////////////////
struct YOKOW_API CFF_DI_P
  {
#include "Yokogawa/FF_DI_P.hpp"
  };

struct YOKOW_API CFF_DI_W
  {
#include "Yokogawa/FF_DI.hpp"
  };

struct YOKOW_API CFF_DI : public CAlarmBase, public CFF_DI_P, public CFF_DI_W STD_DEFINE enum { TypeID = id_FF_DI };
  };
//////////////////////////////////////////////////////////////////////////////////////////////
struct YOKOW_API CFF_AO_P
  {
#include "Yokogawa/FF_AO_P.hpp"
  };

struct YOKOW_API CFF_AO_W
  {
#include "Yokogawa/FF_AO.hpp"
  };

struct YOKOW_API CFF_AO : public CAlarmBase, public CFF_AO_P, public CFF_AO_W STD_DEFINE enum { TypeID = id_FF_AO };
  };
//////////////////////////////////////////////////////////////////////////////////////////////
struct YOKOW_API CFF_DO_P
  {
#include "Yokogawa/FF_DO_P.hpp"
  };

struct YOKOW_API CFF_DO_W
  {
#include "Yokogawa/FF_DO.hpp"
  };

struct YOKOW_API CFF_DO : public CAlarmBase, public CFF_DO_P, public CFF_DO_W STD_DEFINE enum { TypeID = id_FF_DO };
  };
  //////////////////////////////////////////////////////////////////////////////////////////////
  struct YOKOW_API CFF_IS_P
  {
#include "Yokogawa/FF_IS_P.hpp"
  };

  struct YOKOW_API CFF_IS_W
  {
#include "Yokogawa/FF_IS.hpp"
  };

  struct YOKOW_API CFF_IS : public CAlarmBase, public CFF_IS_P, public CFF_IS_W STD_DEFINE enum { TypeID = id_FF_IS };
  };
//////////////////////////////////////////////////////////////////////////
//YOKO_TYPE( 50C6 , FF_PID  , "FF-PID"   )
struct YOKOW_API CFF_PID : STD_DEFINE_COMM(CPID_COMM) enum { TypeID = id_FF_PID };
  };
//////////////////////////////////////////////////////////////////////////////////////////////
//YOKO_TYPE( 50D7 , FF_MAI  , "FF-MAI"   )
struct YOKOW_API CFF_MAI_P
{
#include "Yokogawa/FF_MAI_P.hpp"
};

struct YOKOW_API CFF_MAI_W
{
#include "Yokogawa/FF_MAI.hpp"
};

struct YOKOW_API CFF_MAI : public CAlarmBase, public CFF_MAI_P, public CFF_MAI_W 
{
  STD_DEFINE_WP enum { TypeID = id_FF_MAI };
};
//////////////////////////////////////////////////////////////////////////
//YOKO_TYPE( 5003 , LC64_E  , "LC64-E"   )//91
struct YOKOW_API CLC64_E_P
{
#include "Yokogawa/LC64_E_P.hpp"
};
struct YOKOW_API CLC64_E_W
{
#include "Yokogawa/LC64_E.hpp"
};
struct YOKOW_API CLC64_E : public CAlarmBase, public CLC64_E_P, public CLC64_E_W 
{
  STD_DEFINE_WP enum { TypeID = id_LC64_E };
};
//////////////////////////////////////////////////////////////////////////
//530D
struct YOKOW_API CS_GOV_B_W
{
#include "Yokogawa/S_GOV_B.hpp"
};
struct YOKOW_API CS_GOV_B_P
{
#include "Yokogawa/S_GOV_B_P.hpp"
};
struct YOKOW_API CS_GOV_B : public CAlarmBase, CS_GOV_B_W, CS_GOV_B_P STD_DEFINE enum { TypeID = id_S_GOV_B };
};
//////////////////////////////////////////////////////////////////////////
//50E1
struct YOKOW_API CONOFF_GE_W
{
#include "Yokogawa/ONOFF-GE.hpp"
};
struct YOKOW_API CONOFF_GE_P
{
#include "Yokogawa/ONOFF-GE_P.hpp"
};
struct YOKOW_API CONOFF_GE : public CAlarmBase, CONOFF_GE_W, CONOFF_GE_P STD_DEFINE enum { TypeID = id_ONOFF_GE };
};
////////////////////////////////////////////////////////////////
//5280
struct YOKOW_API CBOPCSYS_W
{
#include <Yokogawa/BOPCSYS.hpp>
};
struct YOKOW_API CBOPCSYS : public CAlarmBase, public CBOPCSYS_W STD_DEFINE enum { TypeID = id_BOPCSYS };
};
////////////////////////////////////////////////////////////////
//5287
struct YOKOW_API CBOPCCHR_W
{
#include <Yokogawa/BOPCCHR.hpp>
};
struct YOKOW_API CBOPCCHR : public CAlarmBase, public CBOPCCHR_W STD_DEFINE enum { TypeID = id_BOPCCHR };
};
////////////////////////////////////////////////////////////////
//5289
struct YOKOW_API CBOPCBOL_W
{
#include <Yokogawa/BOPCBOL.hpp>
};
struct YOKOW_API CBOPCBOL : public CAlarmBase, public CBOPCBOL_W STD_DEFINE enum { TypeID = id_BOPCBOL };
};
////////////////////////////////////////////////////////////////
//528A
struct YOKOW_API CBOPCI16_W
{
#include <Yokogawa/BOPCI16.hpp>
};
struct YOKOW_API CBOPCI16 : public CAlarmBase, public CBOPCI16_W STD_DEFINE enum { TypeID = id_BOPCI16 };
};
////////////////////////////////////////////////////////////////
//528B
struct YOKOW_API CBOPCU16_W
{
#include <Yokogawa/BOPCU16.hpp>
};
struct YOKOW_API CBOPCU16 : public CAlarmBase, public CBOPCU16_W STD_DEFINE enum { TypeID = id_BOPCU16 };
};
////////////////////////////////////////////////////////////////
//528C
struct YOKOW_API CBOPCI32_W
{
#include <Yokogawa/BOPCI32.hpp>
};
struct YOKOW_API CBOPCI32 : public CAlarmBase, public CBOPCI32_W STD_DEFINE enum { TypeID = id_BOPCI32 };
};
////////////////////////////////////////////////////////////////
//528D
struct YOKOW_API CBOPCU32_W
{
#include <Yokogawa/BOPCU32.hpp>
};
struct YOKOW_API CBOPCU32 : public CAlarmBase, public CBOPCU32_W STD_DEFINE enum { TypeID = id_BOPCU32 };
};
////////////////////////////////////////////////////////////////
//528E
struct YOKOW_API CBOPCF32_W
{
#include <Yokogawa/BOPCF32.hpp>
};
struct YOKOW_API CBOPCF32 : public CAlarmBase, public CBOPCF32_W STD_DEFINE enum { TypeID = id_BOPCF32 };
};
////////////////////////////////////////////////////////////////
//528F
struct YOKOW_API CBOPCF64_W
{
#include <Yokogawa/BOPCF64.hpp>
};
struct YOKOW_API CBOPCF64 : public CAlarmBase, public CBOPCF64_W STD_DEFINE enum { TypeID = id_BOPCF64 };
};
////////////////////////////////////////////////////////////////
//52A0
struct YOKOW_API CSR_FNCVR_W
{
#include <Yokogawa/SR_FNCVR.hpp>
};
struct YOKOW_API CSR_FNCVR_P
{
#include <Yokogawa/SR_FNCVR_P.hpp>
};
struct YOKOW_API CSR_FNCVR : public CAlarmBase, public CSR_FNCVR_P, public CSR_FNCVR_W STD_DEFINE enum { TypeID = id_SR_FNCVR };
};
////////////////////////////////////////////////////////////////
//52AB
struct YOKOW_API CSR_PID_W
{
#include <Yokogawa/SR_PID.hpp>
};
struct YOKOW_API CSR_PID_P
{
#include <Yokogawa/SR_PID_P.hpp>
};
struct YOKOW_API CSR_PID : public CAlarmBase, public CSR_PID_P, public CSR_PID_W STD_DEFINE enum { TypeID = id_SR_PID };
};
//YOKO_TYPE( 1C82 , BSI     , "BSI"      )
struct YOKOW_API CBSI_P
{
#include "Yokogawa/BSI_P.hpp"
};
struct YOKOW_API CBSI_W
{
#include "Yokogawa/BSI.hpp"
};
struct YOKOW_API CBSI : public CAlarmBase, public CBSI_P, public CBSI_W 
{
  STD_DEFINE_WP enum { TypeID = id_BSI };
};

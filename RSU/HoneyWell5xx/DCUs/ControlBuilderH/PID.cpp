#include <rsuErr.h>
#include "H_Class.h"
#include <_alarmentry.h>
#include <ConnectH.h>
#include <math.h>

static SBlockCreate PID( "PID", SH_PID::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_PID,W_PID,275)

void SH_PID::InitParm()
{
#include "Blocks/PID.h" 
s_defFlag = SVarInfo::efParam;
#include "Blocks/PID_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class PID_IMPL : public W_PID
{
public:
  void StepT( SStepCalcParams &dt, bool bHaveSpConnection, bool bHaveSpTransfer, bool bOPEUconnect, LPCSTR pFullName );
protected:
  void Режим();
  void CalcK();
  void Обработка_SP( bool bHaveSpConnection, bool bHaveSpTransfer, bool bOPEUconnect, double dt_min );
  void Отслеживание_PV();
  void PV_SP();
  void PID_CTRL( double dt_min );
  void Обработка_выхода( bool CV_Proc, double dt_min );
  void Alarms( LPCSTR pFullName );
  void Modefl();
};

void SH_PID::StepT( SStepCalcParams &dt )
{
  char OLDModeattr = W->MODEATTR.Operator;
  if( !strcmp(BlockName, "30UC950A.PIDA") )
    KKK();
//  if( !strcmp(BlockName, "PIRCAS116.PIDA") )
//    KKK();
  bool bHaveSpConnection = false;
  bool bHaveSpTransfer = false;
  bool bOPEUconnect = false;
  if ( W->MODEATTR.V > 3 || W->MODEATTR.V < 1 )
    W->MODEATTR.V = OLDModeattr;
  for( size_t i=0; i<inConsC; ++i )
  {
    if( 'S'==pInConns[i].szInFld[0] && 'P'==pInConns[i].szInFld[1] )
    {
      if( (W->MODEATTR.Operator == W->MODEATTR.V || W->MODEATTR.Program == W->MODEATTR.V) && !('P'==pInConns[i].szOutFld[0] && 'V'==pInConns[i].szOutFld[1]) )
        bHaveSpConnection = true;
      pInConns[i].enabledTrasfer = W->MODE.V == W->MODE.Cas ? true : false;
      if (pInConns[i].enabledTrasfer)
        bHaveSpTransfer = true;
	  if ( 'O'==pInConns[i].szOutFld[0] && 'P'==pInConns[i].szOutFld[1] && 'E'==pInConns[i].szOutFld[2])
        bOPEUconnect = true;
    }
    if( 'M'==pInConns[i].szInFld[0] && 'O'==pInConns[i].szInFld[1] && 'A'!=pInConns[i].szInFld[4])
    {
      if( W->MODEATTR.Program == W->MODEATTR.V )
        pInConns[i].enabledTrasfer = true;
      else
        pInConns[i].enabledTrasfer = false;
    }
    if( 'O'==pInConns[i].szInFld[0] && 'P'==pInConns[i].szInFld[1] )
    {
      if( '['==pInConns[i].szInFld[2] || 0==pInConns[i].szInFld[2] )
      {
        if( W->MODEATTR.Operator == W->MODEATTR.V )
          pInConns[i].enabledTrasfer = W->MODE.V != W->MODE.Man ? true : false;
        else if( W->MODEATTR.Program == W->MODEATTR.V )
          pInConns[i].enabledTrasfer = true;
      }
    }
  }
  char OLDMode = W->MODE.V;
  if ( W->MODEATTR.V < 3 && W->MODEATTR.V > 1 )
    OLDModeattr = W->MODEATTR.V;
  InputConnectionsTransfer();
  if ( W->MODE.V > 6 )
    W->MODE.V = OLDMode;
  if ( W->MODEATTR.V > 3 || W->MODEATTR.V < 1 )
    W->MODEATTR.V = OLDModeattr;
  PID_IMPL *impl = reinterpret_cast<PID_IMPL*>(W);
  impl->StepT( dt, bHaveSpConnection, bHaveSpTransfer, bOPEUconnect, BlockName );
  OutputConnectionsTransfer();
  if( pINITMAN_Master && pOP_Master )
  {
    if( W->MODE.Cas==W->MODE.V )
      *pINITMAN_Master = false;
    else
    {
      *pINITMAN_Master = true;
      *pOP_Master = W->SPP;
    }
  }
  if( pINITREQ1_Master && pOP_Master )
  {
    if( W->MODE.Cas==W->MODE.V )
      *pINITREQ1_Master = false;
    else
    {
      *pINITREQ1_Master = true;
      *pOP_Master = W->SPP;
    }
  }
  if( pINITREQ2_Master && pOP_Master )
  {
    if( W->MODE.Cas==W->MODE.V )
      *pINITREQ2_Master = false;
    else
    {
      *pINITREQ2_Master = true;
      *pOP_Master = W->SPP;
    }
  }
  if( pINITREQ3_Master && pOP_Master )
  {
    if( W->MODE.Cas==W->MODE.V )
      *pINITREQ3_Master = false;
    else
    {
      *pINITREQ3_Master = true;
      *pOP_Master = W->SPP;
    }
  }
  if( pINITREQ4_Master && pOP_Master )
  {
    if( W->MODE.Cas==W->MODE.V )
      *pINITREQ4_Master = false;
    else
    {
      *pINITREQ4_Master = true;
      *pOP_Master = W->SPP;
    }
  }
/*  if( pINITREQ5_Master && pOP_Master )
  {
    if( W->MODE.Cas==W->MODE.V )
      *pINITREQ5_Master = false;
    else
    {
      *pINITREQ5_Master = true;
      *pOP_Master = W->SPP;
    }
  }
  if( pINITREQ6_Master && pOP_Master )
  {
    if( W->MODE.Cas==W->MODE.V )
      *pINITREQ6_Master = false;
    else
    {
      *pINITREQ6_Master = true;
      *pOP_Master = W->SPP;
    }
  }
  if( pINITREQ7_Master && pOP_Master )
  {
    if( W->MODE.Cas==W->MODE.V )
      *pINITREQ7_Master = false;
    else
    {
      *pINITREQ7_Master = true;
      *pOP_Master = W->SPP;
    }
  }
  if( pINITREQ8_Master && pOP_Master )
  {
    if( W->MODE.Cas==W->MODE.V )
      *pINITREQ8_Master = false;
    else
    {
      *pINITREQ8_Master = true;
      *pOP_Master = W->SPP;
    }
  }*/
}

void SH_PID::StepAfterRestoreState()
{
  W->HIALM.PR = __ALPRIOR::None;
  W->HIALM.TYPE = __DACALMTYPE::None;
  W->HIALM.SV = 0;
  W->PVHHALM.FL = 0;
  W->PVHIALM.FL = 0;
  W->OPHIALM.FL = 0;
  W->OPLOALM.FL = 0;
  W->DEVHIALM.FL = 0;
  W->DEVLOALM.FL = 0;
  W->SIALM.FL = 0;
  for( size_t i=0; i<inConsC; ++i )
  {
    SConnectionMB &con = pInConns[i];
    if( !strcmp( con.szInFld, "SP") && !strncmp( con.szOutFld, "OP", 2) && con.szTypeObjOut && (!strncmp(con.szTypeObjOut, "PID", 3) || !strncmp(con.szTypeObjOut, "FANOUT", 6) || !strncmp(con.szTypeObjOut, "SWITCH", 6) || !strncmp(con.szTypeObjOut, "REGCALC", 7) || !strncmp(con.szTypeObjOut, "OVRDSEL", 7)) && con.objO )
    {
      double *pCVEUHI = NULL, *pCVEULO = NULL;
      eVarType vt = evtHZ;
      if( !con.objO->GetVar( "CVEUHI", (BYTE**)&pCVEUHI, &vt, NULL ) )
        *pCVEUHI = W->SPHILM;
      if( !con.objO->GetVar( "CVEULO", (BYTE**)&pCVEULO, &vt, NULL ) )
        *pCVEULO = W->SPLOLM;
      if ( !strcmp( con.szOutFld, "OP[1]") )
        con.objO->GetVar( "INITREQ[1]", &pINITREQ1_Master, &vt, NULL );
      else if ( !strcmp( con.szOutFld, "OP[2]") )
        con.objO->GetVar( "INITREQ[2]", &pINITREQ2_Master, &vt, NULL );
      else if ( !strcmp( con.szOutFld, "OP[3]") )
        con.objO->GetVar( "INITREQ[3]", &pINITREQ3_Master, &vt, NULL );
      else if ( !strcmp( con.szOutFld, "OP[4]") )
        con.objO->GetVar( "INITREQ[4]", &pINITREQ4_Master, &vt, NULL );
   /*   else if ( !strcmp( con.szOutFld, "OP[5]") )
        con.objO->GetVar( "INITREQ[5]", &pINITREQ5_Master, &vt, NULL );
      else if ( !strcmp( con.szOutFld, "OP[6]") )
        con.objO->GetVar( "INITREQ[6]", &pINITREQ6_Master, &vt, NULL );
      else if ( !strcmp( con.szOutFld, "OP[7]") )
        con.objO->GetVar( "INITREQ[7]", &pINITREQ7_Master, &vt, NULL );
      else if ( !strcmp( con.szOutFld, "OP[8]") )
        con.objO->GetVar( "INITREQ[8]", &pINITREQ8_Master, &vt, NULL );*/
      else
        con.objO->GetVar( "INITMAN", &pINITMAN_Master, &vt, NULL );
	  if ( !strcmp( con.szOutFld, "OPEU" ) )
        con.objO->GetVar( "OP", (BYTE**)&pOP_Master, &vt, NULL );
      else
      con.objO->GetVar( con.szOutFld, (BYTE**)&pOP_Master, &vt, NULL );
    }
  }
}

void SH_PID::GetParams( KHBridge2SysParam &params )
{
  Add2Params( W->K, "K", params );
  Add2Params( W->CTLACTN.V, "Control Action", params );
  Add2Params( W->T1, "T1", params );
  Add2Params( W->T2, "T2", params );
}
//////////////////////////////////////////////////////////////////////////
void PID_IMPL::StepT( SStepCalcParams &dt, bool bHaveSpConnection, bool bHaveSpTransfer, bool bOPEUconnect, LPCSTR pFullName )
{
  /*if ( OPHILM == 105. )
    OPHILM = 100.;
  if ( OPLOLM == -5. )
    OPLOLM = 0.;*/

  PVP = ( PV - PVEULO ) / ( PVEUHI - PVEULO ) * 100.;
  Режим();
  if ( finite( PV ))
    PVSTS.V = PVSTS.Normal;
  else
    PVSTS.V = PVSTS.Bad;
  if ( PVSTS.V == PVSTS.Bad && MODE.V != MODE.Man )
  {
    CV = NaN;
    Modefl();
    return ;
  }
  else
  {
    if ( !finite( CV ))
      CV = OP;
  }
  CalcK();
  Обработка_SP( bHaveSpConnection, bHaveSpTransfer, bOPEUconnect, dt / 60. );
  Отслеживание_PV();
  PV_SP();

  if( !INITMAN )
  {
	  if ( MODE.V == MODE.Auto ||	MODE.V == MODE.Cas )
	  {
			PID_CTRL( dt / 60 );
      if ( CTLACTN.V == CTLACTN.Reverse )
        DELCV = -DELCV;

      double devcvEU = ( CVEUHI - CVEULO ) /100. * DELCV;

      if ( DELCV > 0. )
      {
        if ( ARWOP.V != ARWOP.Hi && ARWOP.V != ARWOP.HiLo )
		      CV += devcvEU;//DELCV;
      }
      if ( DELCV < 0. )
      {
        if ( ARWOP.V != ARWOP.Lo && ARWOP.V != ARWOP.HiLo )
		      CV += devcvEU;//DELCV;
      }
	  }
    double limit = ( CVEUHI - CVEULO ) /100. * OPHILM + CVEULO;
    if ( CV > limit )
	  {
	    CV = limit;
      if ( CTLACTN.V == CTLACTN.Direct )
        ARWNET[0].V = ARWNET[0].Lo;	 //???
      else
        ARWNET[0].V = ARWNET[0].Hi;	 //???
	  }
  
    limit = ( CVEUHI - CVEULO ) /100. * OPLOLM + CVEULO;
    if ( CV < limit )
	  {
        CV = limit;
      if ( CTLACTN.V == CTLACTN.Direct )
        ARWNET[0].V = ARWNET[0].Hi;
      else
        ARWNET[0].V = ARWNET[0].Lo;
	  }
  }

  Обработка_выхода( CVTYPE.Percent==CVTYPE.V, dt / 60 );
  SPPm = SPP;
  PVPm = PVP;
  Alarms( pFullName );
  Modefl();
}

void PID_IMPL::Режим()
{
  if( SIFL )
  {
    if( SIALM.OPT )
    SIALM.FL = 1;
    switch( SIOPT.V )
    {
    case SIOPT.No_Shed:
      break;
    case SIOPT.ShedHigh:
      OP = OPEXHILM;
      MODE = MODE.Man;
      MODEATTR = MODEATTR.Operator;
      break;
    case SIOPT.ShedLow:
      OP = OPEXLOLM;
      MODE = MODE.Man;
      MODEATTR = MODEATTR.Operator;
      break;
    case SIOPT.ShedSafe:
      if ( finite( SAFEOP ) )
        OP = SAFEOP;
      MODE = MODE.Man;
      MODEATTR = MODEATTR.Operator;
      break;
    case SIOPT.ShedHold:
      // OP Не менять!!!
      MODE = MODE.Man;
      MODEATTR = MODEATTR.Operator;
      break;
    }
    return;
  }
  else
  {
    SIALM.FL = 0;
    if( _ESWPERM::PERMIT==ESWPERM.V && _ESWENB::ENABLE==ESWENB.V )
    {
      if( ESWFL.AUTO )
        MODE.V = MODE.Auto;
      else if( ESWFL.BCAS )
        MODE.V = MODE.BCas;
      else if( ESWFL.CAS )
        MODE.V = MODE.Cas;
      else if( ESWFL.MAN )
        MODE.V = MODE.Man;
    }
  }

  if( PVSTS.V == PVSTS.Bad )
  {
    switch ( BADCTLOPT.V )
    {
    case BADCTLOPT.No_Shed:
      break;
    case BADCTLOPT.ShedHigh:
      OP = 106.9;
      MODE = MODE.Man;
      MODEATTR = MODEATTR.Operator;
      break;
    case BADCTLOPT.ShedLow:
      OP = -6.9;
      MODE = MODE.Man;
      MODEATTR = MODEATTR.Operator;
      break;
    case BADCTLOPT.ShedSafe:
      if ( finite( SAFEOP ) )
        OP = SAFEOP;
      MODE = MODE.Man;
      MODEATTR = MODEATTR.Operator;
      break;
    case BADCTLOPT.ShedHold:
      // OP Не менять!!!
      MODE = MODE.Man;
      MODEATTR = MODEATTR.Operator;
      break;
    }
  }
}

void PID_IMPL::CalcK()
{
  if( GAINOPT.V == GAINOPT.Lin )
    return;
  if ( GAINOPT.V == GAINOPT.Gap )
  {
    if ( PV >= SP - GAPLOLM && PV <= SP + GAPHILM )
      K = KLIN * KMODIFGAP;
    else
      K = KLIN;
    if ( K > 240. )
      K = 240.;
    return ;
  }
  if ( GAINOPT.V == GAINOPT.Nonlin )
  {
    double KNL = NLFORM + NLGAIN * fabs ( PVP - SPP ) * 0.01; 
    K = KLIN * KNL;
    if ( K > 240. )
      K = 240.;
    return ;
  }
  if ( GAINOPT.V == GAINOPT.Ext )
  {
    K = KLIN * KMODIFEXT;
    if ( K > 240. )
      K = 240.;
    return ;
  }
}

void PID_IMPL::Обработка_SP( bool bHaveSpConnection, bool bHaveSpTransfer, bool bOPEUconnect, double dt_min )
{
  if( MODE.V == MODE.Cas && bHaveSpTransfer )
  {
    if( bHaveSpConnection )
    {
      double t = 0;
      if ( bOPEUconnect )
        t = SP;
      else
        t = ( PVEUHI - PVEULO ) / 100. * SP;
      SP = t;
    }
  }

  if ( SP > SPHILM )
  {
    if ( MODE.V == MODE.Cas )
    {
      SP = SPHILM;
    }
    else
      SP = SP_OLD;
  }
  if ( SP < SPLOLM )
  {
    if ( MODE.V == MODE.Cas )
    {
      SP = SPLOLM;
    }
    else
      SP = SP_OLD;
  }
  SP_OLD = SP;
  SPP = ( SP - PVEULO ) / ( PVEUHI - PVEULO ) * 100.;

  if( SPTVOPT.Enable!=SPTVOPT.V || MODE.Auto!=MODE.V )
    return;
  switch( SPTVSTATE.V )
  {
  default:
    return;
  case SPTVSTATE.Off:
    return;
  case SPTVSTATE.Preset:
    SPTVP = ( SPTV - PVEULO ) / ( PVEUHI - PVEULO ) * 100.;
    SPTVRATE = (SPTV - SP) / SPTVTIME;
    SPTVDEVFL = 0;
    STARTRATE = SP;
    return;
  case SPTVSTATE.Run:
    break;
  }
  if( 0==SPTVRATE )
    return;
  if( SPTVDEVMAX > 0 )
  {
    if( abs(SP-PV) >= SPTVDEVMAX )
    {
      SPTVDEVFL = 1;
      return;
    }
    SPTVDEVFL = 0;
  }
  SPTVTIME -= dt_min;
  double delta = SPTVRATE * dt_min;
  SP += delta;
  if( SPTVRATE > 0 && SP >= SPTV )
  {
    SPTVSTATE = SPTVSTATE.Off;
    SP = SPTV;
  }
  if( SPTVRATE < 0 && SP <= SPTV )
  {
    SPTVSTATE = SPTVSTATE.Off;
    SP = SPTV;
  }
  if( SPTVTIME < 0 )
    SPTVSTATE = SPTVSTATE.Off;
}

void PID_IMPL::Отслеживание_PV()
{
  /*if ( PVTRAKOPT.V == PVTRAKOPT.NoTrack )
    return;*/
  if ( PVSTS.V == PVSTS.Bad )
    return;
  if ( (MODE.V == MODE.Man || INITMAN) && PVTRAKOPT.V==PVTRAKOPT.Track )
  {
    SP = PV;
    if ( SP > SPHILM )
    {
      SP = SPHILM;
    }
    if ( SP < SPLOLM )
    {
      SP = SPLOLM;
    }
  }
}

void PID_IMPL::PV_SP()
{
  DEV = PV - SP;
  if ( DEV > 0. && finite( DEVHIALM.TP ) )
  {
    if ( DEVHIALM.FL )
    {
      if ( DEV < 0.9 * DEVHIALM.TP )
        DEVHIALM.FL = 0;
    }
    else
    {
      if ( DEV > DEVHIALM.TP )
        DEVHIALM.FL = 1;
    }
  }
  if ( DEV < 0. && finite( DEVLOALM.TP ) )
  {
    DEV = -DEV;
    if ( DEVLOALM.FL )
    {
      if ( DEV < 0.9 * DEVLOALM.TP )
        DEVLOALM.FL = 0;
    }
    else
    {
      if ( DEV > DEVLOALM.TP )
        DEVLOALM.FL = 1;
    }
  }
}

void PID_IMPL::PID_CTRL( double dt_min )
{
  switch ( CTLEQN.V )
  {
  case CTLEQN.EqA:
  case CTLEQN.EqB:
    {
      double D = PVP - SPP;
      double Dm = PVPm - SPPm;
      if( 0 != T1)
        DELCV = K * (( D - Dm ) + 0.5 * dt_min * ( D + Dm ) / T1 ); 
      else
        DELCV = K * (( D - Dm ) + 0.5 * dt_min * ( D + Dm ));
    }
    break;
  case CTLEQN.EqC:
    {
      DELCV = K * (( PVP - PVPm ) + 0.5 * dt_min * (( PVP + PVPm ) - ( SPP + SPPm ))/ T1 );
    }
    break;
  case CTLEQN.EqD:
    {
      double D = PVP - SPP;
      double Dm = PVPm - SPPm;
      DELCV = K * 0.5 * dt_min * ( D + Dm ) / T1; 
    }
    break;
  }
}

void PID_IMPL::Обработка_выхода( bool CV_Proc, double dt_min )
{
  if ( CV_Proc )
  {
		if( MODE.Man==MODE.V || INITMAN)
      CV = OP * 0.01 * ( CVEUHI - CVEULO ) + CVEULO;
    else
    {
      if ( finite( OPROCLM ) && OPROCLM != 0. )
      {
        double D = CV - OP;
        double Dm = OPROCLM * dt_min;
        if ( D > Dm )
          D = Dm;
        if ( D < -Dm )
          D = -Dm;
        OP = OP + D;
      }
      else
			  OP = CV;
    }
  // CV -  в %
		if( finite( CVEUHI ) )
	  {
			double A = OP * 0.01;

			OPEU = ( 1. - A ) * CVEULO + A * CVEUHI;
		}
		else
			OPEU = OP;
  }
	else
	{
// CV - в физ. единицах ???
		OPEU = CV;
		if ( MODE.Man==MODE.V || INITMAN)
      CV = OP * 0.01 * ( CVEUHI - CVEULO ) + CVEULO;
    else
		{
			if ( finite( CVEULO ) && finite( CVEUHI ) )
				OP = ( CV - CVEULO ) / ( CVEUHI - CVEULO ) * 100.;
			else
				OP = CV;
		}
	}
// Пределы 
  if( OPHILM > OPLOLM )
  {
    if ( OP > OPHILM )
    {
      if( MODE.V != MODE.Man )
        OP = OPHILM;
    }
    if ( OP < OPLOLM )
    {
      if( MODE.V != MODE.Man )
        OP = OPLOLM;
    }
  }
  if( OPEXHILM > OPEXLOLM && !SIFL )
  {
    if ( OP > OPEXHILM )
    {
      if( MODE.V == MODE.Man )
        OP = OPEXHILM;
    }
    if ( OP < OPEXLOLM )
    {
      if( MODE.V == MODE.Man )
        OP = OPEXLOLM;
    }
  }
}

void PID_IMPL::Alarms( LPCSTR pFullName )
{
  double scalaPer = ( OPHILM - OPLOLM ) /100.;
  if( finite( OPHIALM.TP) )
  {
    double d = OPHIALM.DB;
    if( _ALMDBU::Percent==OPHIALM.DBU.V )
      d *= scalaPer;
    double limit = scalaPer * OPHIALM.TP + OPLOLM;
    if( OP > limit )
    {
      if( !OPHIALM.FL && pAlarm )
        (*pAlarm)( pFullName, A_OH, OPHIALM.PR, true, OP, NULL, 0, NULL );
      OPHIALM.FL = 1;
    }
    else if( OP < (limit-d) )
    {
      if( OPHIALM.FL && pAlarm )
        (*pAlarm)( pFullName, A_OH, OPHIALM.PR, false, OP, NULL, 0, NULL );
      OPHIALM.FL = 0;
    }
  }
  if( finite( OPLOALM.TP) )
  {
    double d = OPHIALM.DB;
    if( _ALMDBU::Percent==OPLOALM.DBU.V )
      d *= scalaPer;
    double limit = scalaPer * OPLOALM.TP + OPLOLM;
    if( OP < limit )
    {
      if( !OPLOALM.FL && pAlarm )
        (*pAlarm)( pFullName, A_OL, OPLOALM.PR, true, OP, NULL, 0, NULL );
      OPLOALM.FL = 1;
    }
    else if( OP > (limit+d) )
    {
      if( OPLOALM.FL && pAlarm )
        (*pAlarm)( pFullName, A_OL, OPLOALM.PR, false, OP, NULL, 0, NULL );
      OPLOALM.FL = 0;
    }
  }
}

void PID_IMPL::Modefl()
{
  MODEFL.MAN = MODE.V == MODE.Man;
  MODEFL.AUTO = MODE.V == MODE.Auto;
  MODEFL.CAS = MODE.V == MODE.Cas;
  MODEATTRFL.OPER = MODEATTR.V == MODEATTR.Operator;
  MODEATTRFL.PROG = MODEATTR.V == MODEATTR.Program;
  MODEFL.NORM = MODE.V == NORMMODE.V;
  MODEATTRFL.NORM = MODEATTR.V == NORMMODEATTR.V;
}

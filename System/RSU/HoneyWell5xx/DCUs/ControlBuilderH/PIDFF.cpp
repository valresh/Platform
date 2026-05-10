#include <rsuErr.h>
#include "H_Class.h"
#include <_alarmentry.h>
#include <ConnectH.h>
#include <math.h>

static SBlockCreate PIDFF( "PIDFF", SH_PIDFF::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_PIDFF,W_PIDFF,285)

void SH_PIDFF::InitParm()
{
#include "Blocks/PIDFF.h" 
s_defFlag = SVarInfo::efParam;
#include "Blocks/PIDFF_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class PIDFF_IMPL : public W_PIDFF
{
public:
  void StepT( SStepCalcParams &dt, double FFprev, bool bHaveSpConnection, LPCSTR pFullName );
protected:
  void Режим();
  void CalcK();
  void Обработка_SP( bool bHaveSpConnection, double dt_min );
  void Отслеживание_PV();
  void PV_SP();
  void PIDFF_CTRL( double dt_min, double FFprev );
  void Обработка_выхода( bool CV_Proc, double dt_min );
  void Alarms( LPCSTR pFullName );
  void Modefl();
};

void SH_PIDFF::StepT( SStepCalcParams &dt )
{
  char OLDModeattr;
  bool bHaveSpConnection = false;
  for( size_t i=0; i<inConsC; ++i )
  {
    if( 'S'==pInConns[i].szInFld[0] && 'P'==pInConns[i].szInFld[1] )
    {
      if( W->MODEATTR.Operator == W->MODEATTR.V )
        bHaveSpConnection = true;
      pInConns[i].enabledTrasfer = W->MODE.V == W->MODE.Cas ? true : false;
    }
    if( 'M'==pInConns[i].szInFld[0] && 'O'==pInConns[i].szInFld[1] && 'A'!=pInConns[i].szInFld[4])
    {
      if( W->MODEATTR.Program == W->MODEATTR.V )
//        bHaveModeConnection = true;
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
  double FFprev = W->FF;
  char OLDMode = W->MODE.V;
  if ( W->MODEATTR.V < 3 && W->MODEATTR.V > 1 )
    OLDModeattr = W->MODEATTR.V;
  InputConnectionsTransfer();
  if ( W->MODE.V > 6 )
    W->MODE.V = OLDMode;
  if ( W->MODEATTR.V > 3 || W->MODEATTR.V < 1 )
    W->MODEATTR.V = OLDModeattr;
  PIDFF_IMPL *impl = reinterpret_cast<PIDFF_IMPL*>(W);
  impl->StepT( dt, FFprev, bHaveSpConnection, BlockName );
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
}

void SH_PIDFF::GetParams( KHBridge2SysParam &params )
{
  Add2Params( W->K, "K", params );
  Add2Params( W->CTLACTN.V, "Control Action", params );
  Add2Params( W->T1, "T1", params );
  Add2Params( W->T2, "T2", params );
}

void SH_PIDFF::StepAfterRestoreState()
{
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
    if( !strcmp( con.szInFld, "SP") && !strcmp( con.szOutFld, "OP") && !strncmp(con.szTypeObjOut, "PID", 3) && con.objO )
    {
      double *pCVEUHI = NULL, *pCVEULO = NULL;
      eVarType vt = evtHZ;
      if( !con.objO->GetVar( "CVEUHI", (BYTE**)&pCVEUHI, &vt, NULL ) )
        continue;
      if( !con.objO->GetVar( "CVEULO", (BYTE**)&pCVEULO, &vt, NULL ) )
        continue;
      *pCVEUHI = W->SPHILM;
      *pCVEULO = W->SPLOLM;
      con.objO->GetVar( "INITMAN", &pINITMAN_Master, &vt, NULL );
      con.objO->GetVar( "OP", (BYTE**)&pOP_Master, &vt, NULL );
    }
  }
}
//////////////////////////////////////////////////////////////////////////
void PIDFF_IMPL::StepT( SStepCalcParams &dt, double FFprev, bool bHaveSpConnection, LPCSTR pFullName )
{
  /*if ( OPHILM == 105. )
    OPHILM = 100.;
  if ( OPLOLM == -5. )
    OPLOLM = 0.;*/

  PVP = ( PV - PVEULO ) / ( PVEUHI - PVEULO ) * 100.;
  Режим();
  if ( _finite( PV ))
    PVSTS.V = PVSTS.Normal;
  else
    PVSTS.V = PVSTS.Bad;
  if( PVSTS.V == PVSTS.Bad && MODE.V != MODE.Man )
  {
    CV = NaN;
    Modefl();
    return ;
  }
  else
  {
    if ( !_finite( CV ))
      CV = OP;
  }
  CalcK();
  Обработка_SP( bHaveSpConnection, dt / 60. );
  Отслеживание_PV();
  PV_SP();

  if( !INITMAN )
  {
	  if ( MODE.V == MODE.Auto ||	MODE.V == MODE.Cas )
	  {
			PIDFF_CTRL( dt / 60., FFprev );
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

void PIDFF_IMPL::Режим()
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
      if ( _finite( SAFEOP ) )
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
      if ( _finite( SAFEOP ) )
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

void PIDFF_IMPL::CalcK()
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

void PIDFF_IMPL::Обработка_SP( bool bHaveSpConnection, double dt_min )
{
  if( MODE.V == MODE.Cas )
  {
    if( bHaveSpConnection )
    {
      double t = ( PVEUHI - PVEULO ) / 100. * SP;
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

void PIDFF_IMPL::Отслеживание_PV()
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

void PIDFF_IMPL::PV_SP()
{
  DEV = PV - SP;
  if ( DEV > 0. && _finite( DEVHIALM.TP ) )
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
  if ( DEV < 0. && _finite( DEVLOALM.TP ) )
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

void PIDFF_IMPL::PIDFF_CTRL( double dt_min, double FFprev )
{
  double addFF = 0;
  if( _finite(FF) && _finite(FFprev) )
  {
    addFF = KFF * (FF - FFprev);
    FFSTS = _FFSTS::NORMAL;
  }
  else
    FFSTS = _FFSTS::BAD;

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

      if( _FFOPT::ADD==FFOPT.V )
        DELCV += addFF;
    }
    break;
  case CTLEQN.EqC:
    {
      DELCV = K * (( PVP - PVPm ) + 0.5 * dt_min * (( PVP + PVPm ) - ( SPP + SPPm ))/ T1 );

      if( _FFOPT::ADD==FFOPT.V )
        DELCV += addFF;
    }
    break;
  case CTLEQN.EqD:
    {
      double D = PVP - SPP;
      double Dm = PVPm - SPPm;
      DELCV = K * 0.5 * dt_min * ( D + Dm ) / T1; 

      if( _FFOPT::ADD==FFOPT.V )
        DELCV += addFF;
    }
    break;
  case CTLEQN.EqE:
    if( _FFOPT::ADD==FFOPT.V )
    {
      if( _finite(FF) )
        DELCV = KFF * FF;
    }
    break;
  }
}

void PIDFF_IMPL::Обработка_выхода( bool CV_Proc, double dt_min )
{
  if ( CV_Proc )
  {
		if ( MODE.V == MODE.Man || INITMAN )
      CV = OP * 0.01 * ( CVEUHI - CVEULO ) + CVEULO;
    else
    {
      if ( _finite( OPROCLM ) && OPROCLM != 0. )
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
		if( _finite( CVEUHI ) )
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
			if ( _finite( CVEULO ) && _finite( CVEUHI ) )
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
}

void PIDFF_IMPL::Alarms( LPCSTR pFullName )
{
  double scalaPer = ( OPHILM - OPLOLM ) /100.;
  if( _finite( OPHIALM.TP) )
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
  if( _finite( OPLOALM.TP) )
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

void PIDFF_IMPL::Modefl()
{
  MODEFL.MAN = MODE.V == MODE.Man;
  MODEFL.AUTO = MODE.V == MODE.Auto;
  MODEFL.CAS = MODE.V == MODE.Cas;
  MODEATTRFL.OPER = MODEATTR.V == MODEATTR.Operator;
  MODEATTRFL.PROG = MODEATTR.V == MODEATTR.Program;
  MODEFL.NORM = MODE.V == NORMMODE.V;
  MODEATTRFL.NORM = MODEATTR.V == NORMMODEATTR.V;
}

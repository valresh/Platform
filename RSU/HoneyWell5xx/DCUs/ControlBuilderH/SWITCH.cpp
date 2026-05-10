#include <rsuErr.h>
#include "H_Class.h"
#include <math.h>

static SBlockCreate SWITCH( "SWITCH", SH_SWITCH::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_SWITCH,W_SWITCH,285)

void SH_SWITCH::InitParm()
{
#include "Blocks/SWITCH.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/SWITCH_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class SWITCH_IMPL : public W_SWITCH
{
public:
  void StepT( SStepCalcParams &dt );
protected:
  void Modefl();
};

void SH_SWITCH::StepT( SStepCalcParams &dt )
{
  for( size_t i=0; i<inConsC; ++i )
  {
    if( 'O'==pInConns[i].szInFld[0] && 'P'==pInConns[i].szInFld[1] )
    {
      pInConns[i].enabledTrasfer = W->MODEATTR.V == W->MODEATTR.PROGRAM ? true : false;
    }
  }
  InputConnectionsTransfer();

  for( size_t i=0; i<inConsC; ++i )
  {
    if( 'X'!=pInConns[i].szInFld[0] || '['!=pInConns[i].szInFld[1] )
      continue;
    int index = pInConns[i].szInFld[2] - '0';
    if( !('O'==pInConns[i].szOutFld[0] && 'P'==pInConns[i].szOutFld[1]) )
      continue;
    if( !('['==pInConns[i].szOutFld[2] || 0==pInConns[i].szOutFld[2]) )
      continue;
    if( index < 0 )
      continue;
    if( index >= _countof(W->X) )
      continue;
    W->X[index] = (W->XEUHI-W->XEULO) * 0.01 * W->X[index] + W->XEULO;
  }

  SWITCH_IMPL *impl = reinterpret_cast<SWITCH_IMPL*>(W);
  impl->StepT( dt );
  ActualizeConnectionActives();
  if( pINITREQ1_Master )
  {
    if( W->MODE.CAS==W->MODE.V )
      *pINITREQ1_Master = false;
    else
    {
      *pINITREQ1_Master = true;
    }
  }
  if( pINITREQ2_Master )
  {
    if( W->MODE.CAS==W->MODE.V )
      *pINITREQ2_Master = false;
    else
    {
      *pINITREQ2_Master = true;
    }
  }
  if( pINITREQ3_Master )
  {
    if( W->MODE.CAS==W->MODE.V )
      *pINITREQ3_Master = false;
    else
    {
      *pINITREQ3_Master = true;
    }
  }
  if( pINITREQ4_Master )
  {
    if( W->MODE.CAS==W->MODE.V )
      *pINITREQ4_Master = false;
    else
    {
      *pINITREQ4_Master = true;
    }
  }
}
//////////////////////////////////////////////////////////////////////////
void SWITCH_IMPL::StepT( SStepCalcParams &dt )
{
  if( SIFL )
  {
    if( SIALM.OPT )
      SIALM.FL = true;
    switch( SIOPT.V )
    {
    case SIOPT.NO_SHED:
      break;
    case SIOPT.SHEDHOLD:
      MODE.V = MODE.MAN;
      MODEATTR = MODEATTR.OPERATOR;
      break;
    case SIOPT.SHEDLOW:
      MODE.V = MODE.MAN;
      MODEATTR = MODEATTR.OPERATOR;
      OP = OPEXLOLM;
      break;
    case SIOPT.SHEDHIGH:
      MODE.V = MODE.MAN;
      MODEATTR = MODEATTR.OPERATOR;
      OP = OPEXHILM;
      break;
    case SIOPT.SHEDSAFE:
      MODE.V = MODE.MAN;
      MODEATTR = MODEATTR.OPERATOR;
      if ( finite( SAFEOP ))
        OP = SAFEOP;
      break;
    }
  }
  else
  {
    SIALM.FL = false;
    if( ESWENB.ENABLE==ESWENB.V )
    {
      if( ESWFL.AUTO )
        MODE.V = MODE.AUTO;
      else if( ESWFL.BCAS )
        MODE.V = MODE.BCAS;
      else if( ESWFL.CAS )
        MODE.V = MODE.CAS;
      else if( ESWFL.MAN )
        MODE.V = MODE.MAN;
    }
  }

  if ( INITMAN )
    MODE.V = MODE.MAN;
  else
    MODE.V = MODE.CAS;

  Modefl();

  switch( CTLEQN.V )
  {
  case _CTLEQN::EQA:
    for( int i=0; i<_countof(SELXFL); ++i )
    {
      if( i!=SELXINP )
        SELXFL[i] = false;
      else
        SELXFL[i] = true;
    }
    break;
  case _CTLEQN::EQB:
    {
      int otherSEL = 0;
      int nSelected = 0;
      for( int i=1; i<_countof(SELXFL); ++i )
      {
        if( !SELXFL[i] )
          continue;
        ++nSelected;
        if( SELXINP != i )
        {
          otherSEL = i;
          break;
        }
      }
      if( !nSelected )
        SELXINP = 0;
      if( !otherSEL )
        break;
      for( int i=1; i<_countof(SELXFL); ++i )
      {
        if( otherSEL != i )
          SELXFL[i] = false;
        else
        {
          SELXFL[i] = true;
          SELXINP = i;
        }
      }
    }
    break;
  case _CTLEQN::EQC:
    {
      bool bSelected = false;
      for( int i=0; i<_countof(SELXFL); ++i )
      {
        if( !bSelected && SELXFL[i] )
        {
          if( _BADINPTOPT::IGNOREBAD==BADINPTOPT[i].V && IsNaN(X[i]) )
            continue;
          SELXINP = i;
          bSelected = true;
        }
      }
    }
    break;
  default:
    ASSD(0);
  }
  
  if( SELXINP>=_countof(X) || SELXINP < 1 )
  {
    CV = NaN;
    ZeroMemory( SELXDESC, sizeof(SELXDESC) );
    return;
  }

  CV = X[SELXINP] + OPBIAS.FIX + OPBIAS.FLOAT;
  strcpy_s( SELXDESC, XDESC[SELXINP] );
  if( MODE.V == MODE.CAS )
  {
    CVEUHI = XEUHI; //29_03_24
    CVEULO = XEULO; //29_03_24
    OP = ( CV - CVEULO ) / ( CVEUHI - CVEULO ) * 100.;
    double A = OP * 0.01;
    OPEU = ( 1. - A ) * CVEULO + A * CVEUHI;
  }
}

void SH_SWITCH::StepAfterRestoreState()
{
//  if( !strcmp(BlockName, "L104JAT.SWITCHA") )
//    KKK();
//  if( !strcmp(BlockName, "L104JT.SWITCHA") )
//    KKK();
  W->HIALM.PR = __ALPRIOR::None;
  W->HIALM.TYPE = __DACALMTYPE::None;
  W->HIALM.SV = 0;
  switch( W->CTLEQN.V )
  {
  case W_SWITCH::_CTLEQN::EQB:
    {
      if( !W->SELXINP )
      {
        for( int i=_countof(W->SELXFL)-1; i>0; --i )
        {
          if( !W->SELXFL[i] )
            continue;
          W->SELXINP = i;
        }
        break;
      }
      for( int i=1; i<_countof(W->SELXFL); ++i )
      {
        if( W->SELXINP == i )
          W->SELXFL[i] = true;
        else
          W->SELXFL[i] = false;
      }
    }
    break;
  }
  for( size_t i=0; i<inConsC; ++i )
  {
    SConnectionMB &con = pInConns[i];
    if( 'X'==pInConns[i].szInFld[0] && '['==pInConns[i].szInFld[1] && !strncmp( con.szOutFld, "OP", 2) && (!strncmp(con.szTypeObjOut, "PID", 3) || !strncmp(con.szTypeObjOut, "FANOUT", 6)) && con.objO )
    {
      int index = atoi( pInConns[i].szInFld + 2 );
      ASSD( index>0 && index <=8 );
      eVarType vt = evtHZ;
      if ( !strcmp( con.szOutFld, "OP[1]") || !strcmp( con.szOutFld, "OPEU[1]") )
        con.objO->GetVar( "INITREQ[1]", &pINITREQ1_Master, &vt, NULL );
      else if ( !strcmp( con.szOutFld, "OP[2]") || !strcmp( con.szOutFld, "OPEU[2]") )
        con.objO->GetVar( "INITREQ[2]", &pINITREQ2_Master, &vt, NULL );
      else if ( !strcmp( con.szOutFld, "OP[3]") || !strcmp( con.szOutFld, "OPEU[3]") )
        con.objO->GetVar( "INITREQ[3]", &pINITREQ3_Master, &vt, NULL );
      else if ( !strcmp( con.szOutFld, "OP[4]") || !strcmp( con.szOutFld, "OPEU[4]") )
        con.objO->GetVar( "INITREQ[4]", &pINITREQ4_Master, &vt, NULL );
      //else
        con.objO->GetVar( "INITMAN", &Pids[index].pINITMAN_Master, &vt, NULL );
      con.objO->GetVar( con.szOutFld, (BYTE**)&Pids[index].pOP_Master, &vt, NULL );
    }
  }
  ActualizeConnectionActives();
}

void SH_SWITCH::ActualizeConnectionActives()
{
  for( int i=1; i<_countof(Pids); ++i )
  {
    SRegOpInit &pid = Pids[i];
    if( !pid.pINITMAN_Master )
      continue;
    if( i==W->SELXINP && !W->INITMAN )
      *pid.pINITMAN_Master = false;
    else
    {
      *pid.pINITMAN_Master = true;
      *pid.pOP_Master = W->OP;
    }
  }
}

void SWITCH_IMPL::Modefl()
{
  MODEFL.MAN = MODE.V == MODE.MAN;
  MODEFL.AUTO = MODE.V == MODE.AUTO;
  MODEFL.CAS = MODE.V == MODE.CAS;
  MODEATTRFL.OPER = MODEATTR.V == MODEATTR.OPERATOR;
  MODEATTRFL.PROG = MODEATTR.V == MODEATTR.PROGRAM;
  MODEFL.NORM = MODE.V == NORMMODE.V;
  MODEATTRFL.NORM = MODEATTR.V == NORMMODEATTR.V;
}

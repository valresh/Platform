#include <rsuErr.h>
#include "H_Class.h"
#include "../ScriptoH/Trans.h"
#include <SR_DCU.h>
#include <math.h>

using namespace std;
using namespace ns_translator;

static SBlockCreate REGCALC( "REGCALC", SH_REGCALC::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_REGCALC,W_REGCALC,435)

void SH_REGCALC::InitParm()
{
#include "Blocks/REGCALC.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/REGCALC_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class REGCALC_IMPL : public W_REGCALC
{
public:
  void StepT( SStepCalcParams &dt );
protected:
  void Modefl();
};

void SH_REGCALC::StepT( SStepCalcParams &dt )
{
  SAuxCalcCommon::StepT( dt );
  
  for( size_t i=0; i<inConsC; ++i ) //пересчет только для X[1] согласно документации
  {
    if( !strncmp( pInConns[i].szInFld, "X[1]", 4) && (!strncmp( pInConns[i].szOutFld, "OP[", 3) || !strcmp( pInConns[i].szOutFld, "OP")) )
      W->X[1] = W->X[1] * ( W->XEUHI - W->XEULO )/100 + W->XEULO;
  }
  for( int n=0; n<_countof(m_Code); ++n )
  {
    if( Calc( m_Code[n], W->C[n] ) )
    {
      W->EXECSTS[n] = W_REGCALC::_EXECSTS::Ok;
      W->CSTS[n] = W_REGCALC::_CSTS::NORMAL;
    }
    else
    {
      W->EXECSTS[n] = W_REGCALC::_EXECSTS::Fail;
      W->CSTS[n] = W_REGCALC::_CSTS::BAD;
    }
  }

  for( size_t i=0; i<inConsC; ++i )
  {
    if( !strncmp( pInConns[i].szInFld, "X[1]", 4) && (!strncmp( pInConns[i].szOutFld, "OP[", 3) || !strcmp( pInConns[i].szOutFld, "OP")) )
      W->X[i] = ( W->X[i] - W->XEULO ) / ( W->XEUHI - W->XEULO ) * 100.;
  }
  
  InputConnectionsTransfer();
  REGCALC_IMPL *impl = reinterpret_cast<REGCALC_IMPL*>(W);
  impl->StepT( dt );
  
  if( pINITMAN_Master && pOP_Master )
  {
    if( W->MODE.CAS==W->MODE.V )
      *pINITMAN_Master = false;
    else
    {
      *pINITMAN_Master = true;
      //*pOP_Master = W->X[1];
      *pOP_Master = W->OP;
    }
  }
  if( pINITREQ1_Master && pOP_Master )
  {
    if( W->MODE.CAS==W->MODE.V && !W->INITMAN )
      *pINITREQ1_Master = false;
    else
    {
      *pINITREQ1_Master = true;
      //*pOP_Master = W->X[1];
      *pOP_Master = W->OP;
    }
  }
  if( pINITREQ2_Master && pOP_Master )
  {
    if( W->MODE.CAS==W->MODE.V && !W->INITMAN )
      *pINITREQ2_Master = false;
    else
    {
      *pINITREQ2_Master = true;
      //*pOP_Master = W->X[1];
      *pOP_Master = W->OP;
    }
  }
  if( pINITREQ3_Master && pOP_Master )
  {
    if( W->MODE.CAS==W->MODE.V && !W->INITMAN )
      *pINITREQ3_Master = false;
    else
    {
      *pINITREQ3_Master = true;
      //*pOP_Master = W->X[1];
      *pOP_Master = W->OP;
    }
  }
  if( pINITREQ4_Master && pOP_Master )
  {
    if( W->MODE.CAS==W->MODE.V && !W->INITMAN )
      *pINITREQ4_Master = false;
    else
    {
      *pINITREQ4_Master = true;
      //*pOP_Master = W->X[1];
      *pOP_Master = W->OP;
    }
  }
}

void SH_REGCALC::StepAfterRestoreState()
{
  W->HIALM.PR = __ALPRIOR::None;
  W->HIALM.TYPE = __DACALMTYPE::None;
  W->HIALM.SV = 0;
  for( size_t i=0; i<inConsC; ++i )
  {
    SConnectionMB &con = pInConns[i];
    if( !strncmp( con.szInFld, "X[", 2) && !strncmp( con.szOutFld, "OP", 2) && (!strncmp(con.szTypeObjOut, "PID", 3) || !strncmp(con.szTypeObjOut, "FANOUT", 6) || !strncmp(con.szTypeObjOut, "SWITCH", 6) || !strncmp(con.szTypeObjOut, "OVRDSEL", 7) || !strncmp(con.szTypeObjOut, "REGCALC", 7)) && con.objO )
    {
      eVarType vt = evtHZ;
      //con.objO->GetVar( "INITMAN", &pINITMAN_Master, &vt, NULL );
      //con.objO->GetVar( "OP", (BYTE**)&pOP_Master, &vt, NULL );
      if ( !strcmp( con.szOutFld, "OP[1]") || !strcmp( con.szOutFld, "OPEU[1]") )
      {
        con.objO->GetVar( "INITREQ[1]", &pINITREQ1_Master, &vt, NULL );
        if ( !strncmp( con.szOutFld, "OPEU", 4 ) )
          con.objO->GetVar( "OP[1]", (BYTE**)&pOP_Master, &vt, NULL );
        else
          con.objO->GetVar( con.szOutFld, (BYTE**)&pOP_Master, &vt, NULL );
      }
      else if ( !strcmp( con.szOutFld, "OP[2]") || !strcmp( con.szOutFld, "OPEU[2]") )
      {
        con.objO->GetVar( "INITREQ[2]", &pINITREQ2_Master, &vt, NULL );
        if ( !strncmp( con.szOutFld, "OPEU", 4 ) )
          con.objO->GetVar( "OP[2]", (BYTE**)&pOP_Master, &vt, NULL );
        else
          con.objO->GetVar( con.szOutFld, (BYTE**)&pOP_Master, &vt, NULL );
      }
      else if ( !strcmp( con.szOutFld, "OP[3]") || !strcmp( con.szOutFld, "OPEU[3]") )
      {
        con.objO->GetVar( "INITREQ[3]", &pINITREQ3_Master, &vt, NULL );
        if ( !strncmp( con.szOutFld, "OPEU", 4 ) )
          con.objO->GetVar( "OP[3]", (BYTE**)&pOP_Master, &vt, NULL );
        else
          con.objO->GetVar( con.szOutFld, (BYTE**)&pOP_Master, &vt, NULL );
      }
      else if ( !strcmp( con.szOutFld, "OP[4]") || !strcmp( con.szOutFld, "OPEU[4]") )
      {
        con.objO->GetVar( "INITREQ[4]", &pINITREQ4_Master, &vt, NULL );
        if ( !strncmp( con.szOutFld, "OPEU", 4 ) )
          con.objO->GetVar( "OP[4]", (BYTE**)&pOP_Master, &vt, NULL );
        else
          con.objO->GetVar( con.szOutFld, (BYTE**)&pOP_Master, &vt, NULL );
      }
      else
      {
        con.objO->GetVar( "INITMAN", &pINITMAN_Master, &vt, NULL );
        if ( !strncmp( con.szOutFld, "OPEU", 4 ) )
          con.objO->GetVar( "OP", (BYTE**)&pOP_Master, &vt, NULL );
        else
          con.objO->GetVar( con.szOutFld, (BYTE**)&pOP_Master, &vt, NULL );
      }
      //con.objO->GetVar( con.szOutFld, (BYTE**)&pOP_Master, &vt, NULL );
    }
  }
}

void SH_REGCALC::OnReadProject( KBmBase *pRoot, KBmBase *pModule )
{
  ASS( _countof(W->EXPR)==_countof(m_Code) );
  LPSTR pp[_countof(W->EXPR)];
  for( int i=0; i<_countof(W->EXPR); ++i )
  {
    pp[i] = W->EXPR[i];
  }
  SAuxCalcCommon::CreateCodes( pRoot, pModule, m_Code, pp, _countof(W->EXPR) );
}

void SH_REGCALC::SaveBlock(std::fstream& hFile )
{
  LPSTR pp[_countof(W->EXPR)];
  for( int i=0; i<_countof(W->EXPR); ++i )
  {
    pp[i] = W->EXPR[i];
  }
  SAuxCalcCommon::SaveBlock( hFile, m_Code, pp, _countof(pp) );
}

bool SH_REGCALC::RestoreBlock(std::fstream& hFile )
{
  LPSTR pp[_countof(W->EXPR)];
  for( int i=0; i<_countof(W->EXPR); ++i )
  {
    pp[i] = W->EXPR[i];
  }
  return SAuxCalcCommon::RestoreBlock( hFile, m_Code, pp, _countof(pp) );
}

void SH_REGCALC::StepBeforeRestoreState( LPCSTR pSystemName, KBmBase *pRoot, KBmBase *pModule )
{
  for( int n=0; n<_countof(m_Code); ++n )
  {
    for( int i=0; i<m_Code[n].cVars; i++ )
    {
      KCalcVar &cv = ((KCalcVar*)m_Code[n].pVars)[i];
      bool r = cv.Init( pRoot );
      if( !r )
      {
        OutputDebugString( " (" );
        OutputDebugString( BlockName );
        OutputDebugString( ")\n" );
        bErrorInitialization = true;
#ifndef _DEBUG
        break;
#endif
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
void REGCALC_IMPL::StepT( SStepCalcParams &dt )
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

  Modefl();

  double lcv[_countof(X)] = {};
  for( int i=0; i<_countof(X); ++i )
  {
    if ( !finite( X[i] ) )
    {
      XSTS[i].V = XSTS[i].BAD;
      continue;
    }
    XSTS[i].V = XSTS[i].NORMAL;
    //X1P = ( X[i] - XEULO ) / ( XEUHI - XEULO ) * 100.;
    lcv[i] = XK[i] * X[i] + OPBIAS.FIX + XB[i];
  }
  
  if ( !INITMAN )
  {
	  switch( CVSRC.V )
	  {
	  case _CVSRC::NONE:
		break;
	  case _CVSRC::C1:
		CV = C[1];
		break;
	  case _CVSRC::C2:
		CV = C[2];
		break;
	  case _CVSRC::C3:
		CV = C[3];
		break;
	  case _CVSRC::C4:
		CV = C[4];
		break;
	  case _CVSRC::C5:
		CV = C[5];
		break;
	  case _CVSRC::C6:
		CV = C[6];
		break;
	  case _CVSRC::C7:
		CV = C[7];
		break;
	  case _CVSRC::C8:
		CV = C[8];
		break;
	  case _CVSRC::X1:
		CV = X[1];
		break;
	  case _CVSRC::X2:
		CV = X[2];
		break;
	  case _CVSRC::X3:
		CV = X[3];
		break;
	  case _CVSRC::X4:
		CV = X[4];
		break;
	  case _CVSRC::X5:
		CV = X[5];
		break;
	  case _CVSRC::X6:
		CV = X[6];
		break;
	  case _CVSRC::X7:
		CV = X[7];
		break;
	  case _CVSRC::X8:
		CV = X[8];
		break;
	  case _CVSRC::X9:
		CV = X[9];
		break;
	  case _CVSRC::X10:
		CV = X[10];
		break;
	  default:
		ASSD(0);
		break;
	  }
  
	  if ( !IsNaN( OPBIAS.FLOAT ))
		CV += OPBIAS.FLOAT;

	  if ( MODE.V != MODE.MAN )
	  {
		OP = ( CV - CVEULO ) / ( CVEUHI - CVEULO ) * 100.;
	  }
  }
  else
    CV = 0;

	if ( MODE.V == MODE.MAN || INITMAN )
		X[1] = OP;
  if ( OP < OPLOLM )
    OP = OPLOLM;
  if ( OP > OPHILM )
    OP = OPHILM;

  double A = OP * 0.01;
  OPEU = ( 1. - A ) * CVEULO + A * CVEUHI;
}


void REGCALC_IMPL::Modefl()
{
  MODEFL.MAN = MODE.V == MODE.MAN;
  MODEFL.AUTO = MODE.V == MODE.AUTO;
  MODEFL.CAS = MODE.V == MODE.CAS;
  MODEATTRFL.OPER = MODEATTR.V == MODEATTR.OPERATOR;
  MODEATTRFL.PROG = MODEATTR.V == MODEATTR.PROGRAM;
  MODEFL.NORM = MODE.V == NORMMODE.V;
  MODEATTRFL.NORM = MODEATTR.V == NORMMODEATTR.V;
}

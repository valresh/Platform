#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate FANOUT( "FANOUT", SH_FANOUT::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_FANOUT,W_FANOUT,322)

void SH_FANOUT::InitParm()
{
#include "Blocks/FANOUT.h" 
s_defFlag = SVarInfo::efParam;
#include "Blocks/FANOUT_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class FANOUT_IMPL : public W_FANOUT
{
public:
  void StepT( SStepCalcParams &dt, bool bX1ConnectedAsPercent );
};

void SH_FANOUT::StepAfterRestoreState()
{
  W->X1STS = W_FANOUT::_X1STS::Bad;
  for( size_t i=0; i<inConsC; ++i )
  {
    SConnectionMB &con = pInConns[i];
    if( !strcmp( con.szInFld, "X1") && !strcmp( con.szOutFld, "OP") && (!strncmp(con.szTypeObjOut, "PID", 3) || !strncmp(con.szTypeObjOut, "OVRDSEL", 7)) && con.objO )
    {
      eVarType vt = evtHZ;
      con.objO->GetVar( "INITMAN", &pINITMAN_Master, &vt, NULL );
      con.objO->GetVar( "OP", (BYTE**)&pOP_Master, &vt, NULL );
    }
  }
}

void SH_FANOUT::StepT( SStepCalcParams &dt )
{
  if( !strcmp(BlockName, "30UY950E.FANOUTA") )
    KKK();
  bool bX1ConnectedAsPercent = false;
  for( size_t i=0; i<inConsC; ++i )
  {
    if( 'O'==pInConns[i].szInFld[0] && 'P'==pInConns[i].szInFld[1] )
    {
      pInConns[i].enabledTrasfer = W->MODE.V != W->MODE.Man ? true : false;
    }
    if( 'X'==pInConns[i].szInFld[0] && '1'==pInConns[i].szInFld[1] && !strcmp( pInConns[i].szOutFld, "OP") && (!strncmp(pInConns[i].szTypeObjOut, "PID", 3) || !strncmp(pInConns[i].szTypeObjOut, "OVRDSEL", 7)) )
    {
      bX1ConnectedAsPercent = true;
    }
    if( 'X'==pInConns[i].szInFld[0] && '1'==pInConns[i].szInFld[1] )
    {
      pInConns[i].enabledTrasfer = W->MODE.V == W->MODE.Cas ? true : false;
    }
  }
  InputConnectionsTransfer();
  FANOUT_IMPL *impl = reinterpret_cast<FANOUT_IMPL*>(W);
  impl->StepT( dt, bX1ConnectedAsPercent );
  OutputConnectionsTransfer();
  int countin = 0;
  int countall = 0;
  if( pINITMAN_Master && pOP_Master )
  {
    for ( int j=1; j<=8; ++j )
    {
      if ( W->INITREQ[j] != W->REQCHECK[j] )
      {
        countin +=1;
        if ( W->INITREQ[j] )
          countall +=1;
        else
          W->lastOUT = j;
      }
      if ( W->INITREQ[j] )
      {
        W->INITREQ[j] = false;
        W->REQCHECK[j] = false;
      }
      else
      {
        W->INITREQ[j] = true;
        W->REQCHECK[j] = true;
      } 
    } 
    if ( countin == countall && countin !=0 && countin < 5 && W->REQCHECK[0] == 1 )
      W->INITMAN = true;
    if ( countin > countall && countin !=0  )
      W->INITMAN = false;
    if( !(W->INITMAN) )
      *pINITMAN_Master = false;
    else
    {
      *pINITMAN_Master = true;
	  if ( W->lastOUT == 0 )
	    W->lastOUT = 1;
      *pOP_Master = W->OP[W->lastOUT];
    }
    W->REQCHECK[0] = 1;
  }
}

void SH_FANOUT::GetParams( KHBridge2SysParam &params )
{
  for( int i=0;i<_countof(W->РасчетCV); ++i )
  {
    char name[24*4] = { 0 };
    sprintf_s( name, "РасчетCV_%d", i);
    Add2Params( W->РасчетCV[i].V, name, params );
  }
}
//////////////////////////////////////////////////////////////////////////
void FANOUT_IMPL::StepT( SStepCalcParams &dt, bool bX1ConnectedAsPercent )
{
  if( MODE.V==MODE.Man )
  {
    X1P = OP[1];
    X1 = ( XEUHI - XEULO ) / 100. * X1P + XEULO;
  }
  else if( bX1ConnectedAsPercent )
  {
    X1P = X1;
    X1 = ( XEUHI - XEULO ) / 100. * X1P + XEULO;
  }

  if( IsNaN(X1) )
  {
    for( int i=0; i<_countof(CV); ++i )
    {
      CV[i] = NaN;
    }
    X1STS = _X1STS::Bad;
    return;
  }


  X1STS = _X1STS::Normal;
  for( int i=0; i<_countof(CV); ++i )
  {
    if( _РасчетCV::Честно==РасчетCV[i].V )
      CV[i] = X1*K[i] + ( OPBIAS[i].FIX + OPBIAS[i].FLOAT );
    else if( _РасчетCV::Брать_с_X1==РасчетCV[i].V )
      CV[i] = X1;
    else
    {
      ;
    }
    double CVEUSPANBY100 = (CVEUHI[i]-CVEULO[i])/ 100.;
    if( MODE.V != MODE.Man && !INITMAN )
      OP[i] = (CV[i] - CVEULO[i]) /CVEUSPANBY100;
    else
      KKK();

    if( OPHILM > OPLOLM )
    {
      if( OP[i] > OPHILM )
      {
        OP[i] = OPHILM;
        OPEXHIFL[i] = true;
      }
      else
        OPEXHIFL[i] = false;
      if( OP[i] < OPLOLM )
      {
        OP[i] = OPLOLM;
        OPEXLOFL[i] = true;
      }
      else
        OPEXLOFL[i] = false;
    }

    double A = OP[i] * 0.01;
    OPEU[i] = ( 1. - A ) * CVEULO[i] + A * CVEUHI[i];
  }
}

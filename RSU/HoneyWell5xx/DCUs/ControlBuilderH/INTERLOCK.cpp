#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate INTERLOCK( "INTERLOCK", SH_INTERLOCK::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_INTERLOCK,W_INTERLOCK,305)

void SH_INTERLOCK::InitParm()
{
#include "Blocks/INTERLOCK.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/INTERLOCK_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class INTERLOCK_IMPL : public W_INTERLOCK
{
  template<BYTE _S>
  BYTE GateWork( BYTE (&ins)[_S], BYTE ALGID, int NUMINPUTS );
  BYTE Gate4Work( BYTE ALGID, int &G4NUMINPTS, BYTE (&GxSO)[3], BYTE (&ALGxID)[3] );
public:
  void StepT( SStepCalcParams &dt );
};

void SH_INTERLOCK::StepT( SStepCalcParams &dt )
{
    SH_Block::StepT( dt );
  INTERLOCK_IMPL *impl = reinterpret_cast<INTERLOCK_IMPL*>(W);
  impl->StepT( dt );
}
//////////////////////////////////////////////////////////////////////////
template<BYTE _S>
BYTE INTERLOCK_IMPL::GateWork( BYTE (&ins)[_S], BYTE ALGID, int NUMINPUTS )
{
  BYTE res = 0;
  if( !NUMINPUTS )
    return res;
  switch( ALGID )
  {
  case _G1ALGID::NO_ALG:
    KKK();
    break;
  case _G1ALGID::AND:
    res = ins[0];
    for( int i=1; i<NUMINPUTS; ++i )
      res = res && ins[i];
    break;
  case _G1ALGID::OR:
    res = ins[0];
    for( int i=1; i<NUMINPUTS; ++i )
      res = res || ins[i];
    break;
  case _G1ALGID::CONNECT:
    for( int i=0; i<NUMINPUTS; ++i )
      res = ins[i];
    break;
  case _G1ALGID::NOR:
    res = ins[0];
    for( int i=1; i<NUMINPUTS; ++i )
      res = !(res || ins[i]);
    break;
  default:
    ASSD(0);
    break;
  }
  return res;
}

BYTE INTERLOCK_IMPL::Gate4Work( BYTE ALGID, int &G4NUMINPTS, BYTE (&GxSO)[3], BYTE (&ALGxID)[3] )
{
  BYTE res = 0;
  int inputs = 0;
  for( int i=0; i<3; ++i )
  {
    switch( ALGID )
    {
    case _G4ALGID::NO_ALG:
      continue;
    case _G4ALGID::AND:
      if( _G4ALGID::NO_ALG==ALGxID[i] )
        continue;
      if( !inputs )
        res = GxSO[i];
      else
        res = res && GxSO[i];
      break;
    case _G4ALGID::OR:
      if( _G4ALGID::NO_ALG==ALGxID[i] )
        continue;
      if( !inputs )
        res = GxSO[i];
      else
        res = res || GxSO[i];
      break;
    case _G4ALGID::CONNECT:
      if( _G4ALGID::NO_ALG==ALGxID[i] )
        continue;
      res = GxSO[i];
      break;
    default:
      ASSD(0);
      break;
    }
    ++inputs;
  }
  G4NUMINPTS = inputs;
  return res;
}

void INTERLOCK_IMPL::StepT( SStepCalcParams &dt )
{
  BYTE *pINVERTs = &C1INVERT;
  BYTE *pSOs = &C1SO;
  BYTE *pFLs = &C1FL;
  BYTE *pBYPPERMs = &C1BYPPERM;
  BYTE *pBYPREQs = &C1BYPREQ.V;
  for( int i=0; i<12; ++i )
  {
    if( pINVERTs[i] )
      pSOs[i] = !pFLs[i];
    else
      pSOs[i] = pFLs[i];
  }

  BYTE gateF[3] = {};
  BYTE ins[12] = {};
  int N = 0;
  BYTE *pCnGATEx = &C1GATE1;
  for( int c=0; c<12; ++c )
  {
    if( !pCnGATEx[c] )
      continue;
    if( !pBYPPERMs[c] || pBYPREQs[c] == C1BYPREQ.No_CMD )
      ins[N] = pSOs[c];
    else
    {
      switch( pBYPREQs[c] )
      {
      case _C1BYPREQ::No_CMD:
        ins[N] = 255;
        break;
      case _C1BYPREQ::TO_ON:
        ins[N] = 1;
        break;
      case _C1BYPREQ::TO_OFF:
        ins[N] = 0;
        break;
      }
    }
    if( !N )
      G1FIRSTCOND = c+1;
    ++N;
    if( _countof(ins)==N )
      break;
  }
  if( !N )
    G1FIRSTCOND = 0;
  G1NUMINPUTS = N;
  gateF[0] = GateWork( ins, G1ALGID.V, G1NUMINPUTS );
  if( 255!=gateF[0] )
    G1SO = gateF[0];
  
  N = 0;
  pCnGATEx = &C1GATE2;
  for( int c=0; c<12; ++c )
  {
    if( !pCnGATEx[c] )
      continue;
    if( !pBYPPERMs[c] || pBYPREQs[c] == C1BYPREQ.No_CMD )
      ins[N] = pSOs[c];
    else
    {
      switch( pBYPREQs[c] )
      {
      case _C1BYPREQ::No_CMD:
        ins[N] = 255;
        break;
      case _C1BYPREQ::TO_ON:
        ins[N] = 1;
        break;
      case _C1BYPREQ::TO_OFF:
        ins[N] = 0;
        break;
      }
    }
    if( !N )
      G2FIRSTCOND = c+1;
    ++N;
    if( _countof(ins)==N )
      break;
  }
  if( !N )
    G2FIRSTCOND = 0;
  G2NUMINPUTS = N;
  gateF[1] = GateWork( ins, G2ALGID.V, G2NUMINPUTS );
  if( 255!=gateF[1] )
    G2SO = gateF[1];
  
  N = 0;
  pCnGATEx = &C1GATE3;
  for( int c=0; c<12; ++c )
  {
    if( !pCnGATEx[c] )
      continue;
    if( !pBYPPERMs[c] || pBYPREQs[c] == C1BYPREQ.No_CMD )
      ins[N] = pSOs[c];
    else
    {
      switch( pBYPREQs[c] )
      {
      case _C1BYPREQ::No_CMD:
        ins[N] = 255;
        break;
      case _C1BYPREQ::TO_ON:
        ins[N] = 1;
        break;
      case _C1BYPREQ::TO_OFF:
        ins[N] = 0;
        break;
      }
    }
    if( !N )
      G3FIRSTCOND = c+1;
    ++N;
    if( _countof(ins)==N )
      break;
  }
  if( !N )
    G3FIRSTCOND = 0;
  G3NUMINPUTS = N;
  gateF[2] = GateWork( ins, G3ALGID.V, G3NUMINPUTS );
  if( 255!=gateF[2] )
    G3SO = gateF[2];

  BYTE GxSO[] = { G1SO, G2SO, G3SO };
  BYTE ALGxID[] = { G1ALGID.V, G2ALGID.V, G3ALGID.V };
  G4SO = Gate4Work( G4ALGID.V, G4NUMINPTS, GxSO, ALGxID );

  if( FINALBYPPERM && FINALBYPREQ.V != FINALBYPREQ.No_CMD )
  {
    switch( FINALBYPREQ.V )
    {
    case _FINALBYPREQ::No_CMD:
      break;
    case _FINALBYPREQ::TO_ON:
      G4SOFINAL = 1;
      break;
    case _FINALBYPREQ::TO_OFF:
      G4SOFINAL = 0;
      break;
    }
  }
  else
  {
    if( G4LATCHOPT )
    {
      if( G4SO )
        G4SOFINAL = G4SO;
      else
      {
        if( G4LATCHRESET )
          G4SOFINAL = 0;
      }
    }
    else
      G4SOFINAL = G4SO;
  }

  G4SOFINAL_INV = !G4SOFINAL;

  if( G4LATCHRESET )
    G4LATCHRESET = 0;
}

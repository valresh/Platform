#include <rsuErr.h>
#include "H_Class.h"
#include <limits.h>

static SBlockCreate SIGNALSEL( "SIGNALSEL", SH_SIGNALSEL::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_SIGNALSEL,W_SIGNALSEL,205)

void SH_SIGNALSEL::InitParm()
{
#include "Blocks/SIGNALSEL.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/SIGNALSEL_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class _SIGNALSEL_IMPL : public W_SIGNALSEL
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_SIGNALSEL::StepT( SStepCalcParams &dt )
{
    SH_Block::StepT( dt );
  _SIGNALSEL_IMPL *impl = reinterpret_cast<_SIGNALSEL_IMPL*>(W);
  impl->StepT( dt );
}
//////////////////////////////////////////////////////////////////////////
void _SIGNALSEL_IMPL::StepT( SStepCalcParams &dt )
{
  if ( 0 == NUMPINPT )
  {
    for( int i=1; i<_countof(P);++i )
    {
      if ( !strcmp( PDESC[i], "" ) )
        continue;
      ++NUMPINPT;
    }
  }
  switch( SELMETHOD.V )
  {
  case _SELMETHOD::MIN:
    {
      double inP = std::numeric_limits<double>::max();
      for( int i=1; i<_countof(P);++i )
      {
        if( IsNaN( P[i] ) )
          continue;
        if( P[i] < inP )
        {
          inP = PV = P[i];
          SELIN = i;
          PVSTS = _PVSTS::NORMAL;
        }
      }
      if(std::numeric_limits<double>::max() ==inP )
      {
        PV = NaN;
        SELIN = 0;
        PVSTS = _PVSTS::BAD;
      }
    }
    break;
  case _SELMETHOD::MAX:
    {
      double inP = -std::numeric_limits<double>::max();
      for( int i=1; i<_countof(P);++i )
      {
        if( IsNaN( P[i] ) )
          continue;
        if( P[i] > inP )
        {
          inP = PV = P[i];
          SELIN = i;
          PVSTS = _PVSTS::NORMAL;
        }
      }
      if( -std::numeric_limits<double>::max() ==inP )
      {
        PV = NaN;
        SELIN = 0;
        PVSTS = _PVSTS::BAD;
      }
    }
    break;
  case _SELMETHOD::AVG:
    {
      double inP = NaN;
      int inN = -1;
      for( int i=1; i<_countof(P);++i )
      {
        if( IsNaN( P[i] ) )
          continue;
        if( inN == -1 )
        {
          inN = 0;
          inP = 0;
        }
        inP += P[i];
        ++inN;
      }
      if( inN>=NMIN && !IsNaN(inP) )
      {
        PV = inP / inN;
        PVSTS = _PVSTS::NORMAL;
      }
      else
      {
        PV = NaN;
        SELIN = 0;
        PVSTS = _PVSTS::BAD;
      }
    }
    break;
  case _SELMETHOD::MUX:
    if( BOOLMUX )
    {
      bool r = true;
      bool badPV = true;
      for( int i=1; i<_countof(SELXFL); ++i )
      {
        if( !SELXFL[i] )
          continue;
        badPV = false;
        if( IsNaN( P[i] ) )
        {
          badPV = true;
        }
        else
        {
          PV = P[i];
          SELIN = i;
          PVSTS = _PVSTS::NORMAL;
        }
        break;
      }
      if( badPV )
      {
        PV = NaN;
        SELIN = 0;
        PVSTS = _PVSTS::BAD;
      }
    }
    else
    {
      if ( SELIN == 0 )
        SELIN = 1;
      int i = SELIN;
      if ( MUXSEL > 0 )
        i = MUXSEL;
      if( i<1 || i>=_countof(P) || IsNaN( P[i] ) )
      {
        PV = NaN;
        SELIN = 0;
        PVSTS = _PVSTS::BAD;
      }
      else
      {
        PV = P[i];
        SELIN = i;
        PVSTS = _PVSTS::NORMAL;
      }
    }
    break;
  case _SELMETHOD::MED:
    {
      int inN = -1;
      for( int i=1; i<_countof(P);++i )
      {
        if( IsNaN( P[i] ) )
          continue;
        ++inN;
      }
      if( !inN )
      {
        PV = NaN;
        SELIN = 0;
        PVSTS = _PVSTS::BAD;
        break;
      }
      PVSTS = _PVSTS::NORMAL;
      if( inN % 2 )
      {
        int n = inN / 2 + 2;
        for( int i=1; i<n;++i )
        {
          if( IsNaN( P[i] ) )
            continue;
          if( i==n )
          {
            PV = P[i];
            SELIN = i;
            break;
          }
        }
      }
      else
      {
        int n = inN / 2;
        switch( MEDOPT.V )
        {
        case _MEDOPT::MIN:
          if( P[n] < P[n+1] )
          {
            PV = P[n];
            SELIN = n;
          }
          else
          {
            PV = P[n+1];
            SELIN = n+1;
          }
          break;
        case _MEDOPT::MAX:
          if( P[n] > P[n+1] )
          {
            PV = P[n];
            SELIN = n;
          }
          else
          {
            PV = P[n+1];
            SELIN = n+1;
          }
          break;
        case _MEDOPT::AVG:
          PV = (P[n]+P[n+1])/2.;
          break;
        }
      }
    }
    break;
  default:
    ASSD(0);
    break;
  }
}

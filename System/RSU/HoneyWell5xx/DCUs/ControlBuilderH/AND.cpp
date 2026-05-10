#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate AND( "AND", SH_AND::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_AND,W_AND,52)

void SH_AND::InitParm()
{
#include "Blocks/AND.h" 
s_defFlag = SVarInfo::efParam;
#include "Blocks/AND_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

void SH_AND::StepAfterRestoreState()
{
/*  bool bConnected[_countof(W->IN)] = {};
  for( DWORD n=0; n<inConsC; ++ n )
  {
    SConnectionMB &con = pInConns[n];
    int i = atoi( con.szInFld+3 );
    if( i<1 || i>=_countof(W->IN) )
    {
      ASSD( !"странно");
      continue;
    }
    bConnected[i] = true;
  }
  for( DWORD n=0; n<_countof(bConnected); ++ n )
  {
    if( bConnected[n] )
      continue;
    W->IN[n] = true;
  }*/
}

class AND_IMPL : public W_AND
{
public:
  void StepT( SStepCalcParams &dt, bool (&connectedIn)[9] );
};

void SH_AND::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  AND_IMPL *impl = reinterpret_cast<AND_IMPL*>(W);
  bool connectedIn[_countof(W->IN)] = {};

  for( size_t i=0; i<inConsC; ++i )
  {
    if( !isdigit(pInConns[i].szInFld[3]) )
      continue;
    int n = pInConns[i].szInFld[3] - '0';
    ASSD( n>0 && n<_countof(connectedIn) );
    connectedIn[n] = true;
  }

  impl->StepT( dt, connectedIn );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void AND_IMPL::StepT( SStepCalcParams &dt, bool (&connectedIn)[9] )
{
  int nIns = 0;
  for( int n = 1; n < _countof(connectedIn); n++ )
  {
    ++nIns;
    BYTE Actual_IN = IN[n];
    if( INPTINVSTS[n] )
      Actual_IN = !IN[n];
    if( !Actual_IN )
    {
      OUT = 0;
      return;
    }
  }
  if( nIns )
    OUT = 1;
}

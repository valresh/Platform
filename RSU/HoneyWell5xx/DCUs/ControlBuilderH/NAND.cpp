#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate NAND( "NAND", SH_NAND::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_NAND,W_NAND,52)

void SH_NAND::InitParm()
{
#include "Blocks/NAND.h" 
s_defFlag = SVarInfo::efParam;
#include "Blocks/NAND_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class NAND_IMPL : public W_NAND
{
public:
  void StepT( SStepCalcParams &dt, bool (&connectedIn)[9] );
};

void SH_NAND::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  NAND_IMPL *impl = reinterpret_cast<NAND_IMPL*>(W);
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
void NAND_IMPL::StepT( SStepCalcParams &dt, bool (&connectedIn)[9] )
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
      OUT = 1;
      return;
    }
  }
  if( nIns )
    OUT = 0;
}

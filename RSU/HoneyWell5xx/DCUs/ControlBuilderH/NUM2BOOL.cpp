#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate NUM2BOOL( "NUM2BOOL", SH_NUM2BOOL::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_NUM2BOOL,W_NUM2BOOL,76)

void SH_NUM2BOOL::InitParm()
{
#include "Blocks/NUM2BOOL.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/NUM2BOOL_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class NUM2BOOL_IMPL : public W_NUM2BOOL
{
public:
  void StepT( SStepCalcParams &dt );
};

void SH_NUM2BOOL::StepT( SStepCalcParams &dt )
{
    SH_Block::StepT( dt );
  NUM2BOOL_IMPL *impl = reinterpret_cast<NUM2BOOL_IMPL*>(W);
  impl->StepT( dt );
}
//////////////////////////////////////////////////////////////////////////
void NUM2BOOL_IMPL::StepT( SStepCalcParams &dt )
{
  if( FLOAT64STS.Manual== FLOAT64STS.V )
    return;
  INT64 val = 0;
  UINT64 uval = 0;
  switch( CONVOPT.V )
  {
  case CONVOPT.int32:
    val = IN.Int32;
    break;
  case CONVOPT.float32:
    val = IN.FLOAT32;
    break;
  case CONVOPT.uint64:
    uval = IN.Uint64;
    break;
  case CONVOPT.float64x32:
    val = IN.FLOAT64x32;
    break;
  default:
    FLOAT64STS = FLOAT64STS.Bad;
    return;
  }
  BYTE *pO = &OUT[1];
  INT64 mask = 1;
  for( int i=0; i<64; ++i, pO++ )
  {
    if( CONVOPT.uint64==CONVOPT.V)
    {
      if( mask & uval )
        *pO = 0;
      else
        *pO = 1;
    }
    else
    {
      if( mask & val )
        *pO = 0;
      else
        *pO = 1;
    }
    mask = mask << 1;
  }
  FLOAT64STS = FLOAT64STS.Normal;
}

#include <rsuErr.h>
#include "H_Class.h"

static SBlockCreate ROLLAVG( "ROLLAVG", SH_ROLLAVG::Create );

#include <HPARM_INIT.h> 
#include "ParmVarInfo.h"
LIST_PARM(SH_ROLLAVG,W_ROLLAVG,15)

void SH_ROLLAVG::InitParm()
{
#include "Blocks/ROLLAVG.h" 
  s_defFlag = SVarInfo::efParam;
#include "Blocks/ROLLAVG_P.h"
  qsort ( VarInfo, kVarInfo, sizeof ( SVarInfo ), CompVarInfo );
}

class ROLLAVG_IMPL : public W_ROLLAVG
{
public:
  void StepT( SStepCalcParams &dt, SH_ROLLAVG::SInternal &data );
};

void SH_ROLLAVG::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  ROLLAVG_IMPL *impl = reinterpret_cast<ROLLAVG_IMPL*>(W);
  impl->StepT( dt, m_Data );
  OutputConnectionsTransfer();
}
//////////////////////////////////////////////////////////////////////////
void ROLLAVG_IMPL::StepT( SStepCalcParams &dt, SH_ROLLAVG::SInternal &data )
{
  if( !data.pBuf )
  {
    data.counts = (ROLLBUF * (int)ROLLBFBASE)/(ROLLFREQ * (int)ROLLFRBASE);
    data.pBuf = (double*)KMemoryServiceH5xx::Instance().NewMem(sizeof(double)*data.counts+2);
    data.waitS = ROLLFREQ * (int)ROLLFRBASE;
    data.cwS = 0;
    ROLLINDEX = 0;
    ROLLCOUNTER = 0;
  }
  data.cwS += dt;
  if( data.cwS < data.waitS )
    return;

  if( IsNaN(IN) )
  {
    if( ROLLAVGBAD < data.counts )
      ROLLAVGBAD++;
    if( ROLLAVGOK )
      --ROLLAVGOK;
    return;
  }
  double *ROLLSAMPLE = data.pBuf;
  if( ROLLAVGRST )
  {
    OUT = 0;
    ROLLAVGBAD = 0;
    ROLLAVGOK = 0;
    ROLLCOUNTER = 0;
    ROLLINDEX = 0;
    ROLLACCUM = 0;
    ROLLAVGRST = 0;
  }

  if( ROLLAVGOK < data.counts )
    ROLLAVGOK++;
  if( ROLLAVGBAD )
    --ROLLAVGBAD;

  data.cwS -= data.waitS;

  data.pBuf[ROLLINDEX] = IN;
  ROLLCOUNTER++;
  if( ROLLINDEX==data.counts )
    ROLLINDEX = 0;
  if( ROLLCOUNTER==data.counts )
    ROLLCOUNTER = data.counts;
  ROLLACCUM = 0;
  for( int i=0; i<ROLLCOUNTER; ++i )
    ROLLACCUM += data.pBuf[i];
  ROLLACCUM /= ROLLCOUNTER;
  OUT = ROLLACCUM;
}

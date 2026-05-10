#include "SR.h"

namespace ns_HIMA
{
  KSR::KSR()
  {
  }

  void KSR::Calc( KFlexBlockBase::SCallParams &params )
  {
    if( R )
      Q1 = FALSE;
    if( S1 )
      Q1 = TRUE;
  }
}

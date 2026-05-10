#include "TP.h"

namespace ns_HIMA
{
  KTP::KTP()
  {
  }

  void KTP::Calc( KFlexBlockBase::SCallParams &params )
  {
    if( REDGE )
    {
      PDATE += params.dtS;
      if( PDATE < PT )
        Q = TRUE;
      else
        Q = FALSE, PDATE = PT, REDGE = in;
    }
    else
    {
      if( in && !REDGE )
        Q = TRUE, REDGE = TRUE, PDATE = 0;
      else
        Q = FALSE, REDGE = FALSE;
    }
    ET = PDATE;
  }
}

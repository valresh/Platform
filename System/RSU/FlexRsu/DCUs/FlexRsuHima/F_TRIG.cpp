#include "F_TRIG.h"

namespace ns_HIMA
{
  KF_TRIG::KF_TRIG()
  {
  }

  void KF_TRIG::Calc( KFlexBlockBase::SCallParams &params )
  {
    if( CLK )
    {
      FEDGE = TRUE;
      Q = FALSE;
    }
    else
    {
      if( FEDGE )
        Q = TRUE, FEDGE = FALSE;
      else
        Q = FALSE ,FEDGE = FALSE;
    }
  }
}

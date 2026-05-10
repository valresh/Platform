#include "TON.h"

namespace ns_HIMA
{
  KTON::KTON()
  {
  }

  void KTON::Calc( KFlexBlockBase::SCallParams &params )
  {
    if ( FEDGE && in )
    {
      PDATE += params.dtS;
      if ( PDATE < PT )
        Q = false;
      else
        Q = true, PDATE = PT;
    }
    else
    {
      Q = false;
      if ( in )
        FEDGE = true, PDATE = 0;
      else
        FEDGE = false;
    }
    ET = PDATE;
  }
}

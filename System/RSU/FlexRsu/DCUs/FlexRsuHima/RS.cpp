#include "RS.h"

namespace ns_HIMA
{
  KRS::KRS()
  {
  }

  void KRS::Calc( KFlexBlockBase::SCallParams &params )
  {
    if( S )
      Q1 = TRUE;
    if( R1 )
      Q1 = FALSE;
  }
}

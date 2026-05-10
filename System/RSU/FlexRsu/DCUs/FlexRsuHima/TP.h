#pragma once
#include <FlexVarTypes.h>
#include <FlexBlockBase.h>

namespace ns_HIMA
{
  class KTP
  {
  public:
    tDigital32 in;
    tAnalog32/*time*/ PT;
    tDigital32 Q;
    tAnalog32/*time*/ ET;
    tAnalog32/*time*/ PDATE;
    tDigital32 REDGE;
  public:
    KTP();
    void Calc( KFlexBlockBase::SCallParams &params );
  };
}

#pragma once
#include <FlexVarTypes.h>
#include <FlexBlockBase.h>

namespace ns_HIMA
{
  class KSR
  {
  public:
    tDigital32 S1;
    tDigital32 Q1;
    tDigital32 R;
  public:
    KSR();
    void Calc( KFlexBlockBase::SCallParams &params );
  };
}

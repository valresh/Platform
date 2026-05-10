#pragma once
#include <FlexVarTypes.h>
#include <FlexBlockBase.h>

namespace ns_HIMA
{
  class KF_TRIG
  {
  public:
    tDigital32 CLK;
    tDigital32 Q;
    tDigital32 FEDGE;
  public:
    KF_TRIG();
    void Calc( KFlexBlockBase::SCallParams &params );
  };
}

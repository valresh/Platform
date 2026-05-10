#pragma once
#include <FlexVarTypes.h>
#include <FlexBlockBase.h>

namespace ns_HIMA
{
  class KRS
  {
  public:
    tDigital32 S;
    tDigital32 Q1;
    tDigital32 R1;
  public:
    KRS();
    void Calc( KFlexBlockBase::SCallParams &params );
  };
}

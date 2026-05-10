#include <crossplatform.h>
#include <SharedFlexBlocks.h>

#include "SharedListValue.h"
void S_SimDO::BuildList()
{
  typedef S_SimDO _S;
#include "SharedListValue_Build.h"
#include <FlexBlocks/SimDIO.h>
}

void S_SimDI::BuildList()
{
  typedef S_SimDI _S;
#include "SharedListValue_Build.h"
#include <FlexBlocks/SimDIO.h>
}
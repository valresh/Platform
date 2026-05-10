#include <crossplatform.h>
#include <SharedFlexBlocks.h>

#include "SharedListValue.h"
void S_OPCDO::BuildList()
{
  typedef S_OPCDO _S;
#include "SharedListValue_Build.h"
#include <FlexBlocks/OpcDIO.h>
}

void S_OPCDI::BuildList()
{
  typedef S_OPCDI _S;
#include "SharedListValue_Build.h"
#include <FlexBlocks/OpcDIO.h>
}

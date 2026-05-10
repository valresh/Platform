#include <crossplatform.h>
#include <SharedFlexBlocks.h>

#include "SharedListValue.h"
void S_OPCAO::BuildList()
{
  typedef S_OPCAO _S;
#include "SharedListValue_Build.h"
#include <FlexBlocks/OpcAIO.h>
}

void S_OPCAI::BuildList()
{
  typedef S_OPCAI _S;
#include "SharedListValue_Build.h"
#include <FlexBlocks/OpcAIO.h>
}

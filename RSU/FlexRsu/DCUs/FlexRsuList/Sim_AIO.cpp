#include <crossplatform.h>
#include <SharedFlexBlocks.h>

#include "SharedListValue.h"

void S_SimAI::BuildList()
{
  typedef S_SimAI _S;
#include "SharedListValue_Build.h"
#include <FlexBlocks/SimAIO.h>
}

void S_SimAO::BuildList()
{
  typedef S_SimAO _S;
#include "SharedListValue_Build.h"
#include <FlexBlocks/SimAIO.h>
}

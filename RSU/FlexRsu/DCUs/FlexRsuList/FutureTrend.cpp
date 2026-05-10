#include <crossplatform.h>
#include <DataTypes_.h>

#include "SharedListValue.h"
#include <stdio.h>
void SFlexFutureTrend::BuildList()
{
  typedef SFlexFutureTrend _S;
#include "SharedListValue_Build.h"
/*  {
    char szName[64];
    for( int i=0; i<_countof( ((_S*)0)->trendPoints); ++i )
    {
      sprintf_s( szName, "trendPoints[%d]", i );
    _theList.AddRaw( szName, evtAnalog32, _S::TypeID, offsetof( _S, trendPoints[i]), sizeof(tAnalog32) );
    }
  }*/
#include <FlexBlocks/FutureTrend.h>
}

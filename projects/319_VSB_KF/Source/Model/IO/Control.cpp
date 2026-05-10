#include "stdafx.h"
#include "IO.h"
#include "Err.h"
#include "SetData.h"
#include "Work.h"
#include "TestNodes.h"
#include "HydroStruct.h"


int CIO::Work( double dt )
  {
  return 0;
  }

int CIO::TestCtrlPoints( int kNodes, struct CObjectPoint ** ppNodes )
  {
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[0];
    if ( pOP->Point == "Управление" )
      {
      pOP->PntGroup = 2;
      pOP->NumbInGroup = 0;
      pOP->IO = '?';
      continue;
      }
    if ( pOP->Point == "Выход" )
      {
      pOP->PntGroup = 3;
      pOP->NumbInGroup = 0;
      pOP->IO = '?';
      continue;
      }
    ModMsg ( "У источника '%s' неравильный аналоговый вход '%s'", ObjName.Str, pOP->Point.Str );
    }
  return 0;
  }


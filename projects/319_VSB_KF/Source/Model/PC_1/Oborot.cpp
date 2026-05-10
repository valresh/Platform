#include "stdafx.h"
#include "PC_1.h"
#include "Err.h"
#include "Work.h"
#include "TestNodes.h"
#include "HydroStruct.h"

/*
int CPC_1::Work( double dt )
  {
  if ( pMotorData == NULL )
    {
//    ModMsg ( "У компрессора '%s' нет мотора", ObjName );
    return 1;
    }
  On = pMotorData->On;
  Oborot = pMotorData->Oborot;
  return 0;
  }

int CPC_1::TestCtrlPoints( int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( kNodes < 1 )
    {
    ModMsg ( "У компрессора '%s' нет входов", ObjName.Str );
    return 1;
    }
  int kIn = 0;
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    pOP->Type = eГидравлика;
    if ( pOP->Point == "Обороты" )
      {
    //  pAnalogPipe = pOP->pPipe;
    //  pOP->PntGroup = n+1;
    //  pOP->NumbInGroup = 0;
    //  pOP->IO = '+';
    //  kIn++;
    //  continue;
    //  }
    // Все остальные выходы
    //pAnalogPipe = pOP->pPipe;
    //Char<128>Source;
    //Source.Prt ( "%s.Обороты", ObjName );
    //pAnalogPipe->pControl->SetAddr( &ExtOborot, this, Source );
    //pOP->PntGroup = n+1;
    //pOP->NumbInGroup = 0;
    //pOP->IO = '-';
      }
    }
  if ( kIn != 1 )
    {
    ModMsg ( "У компрессора '%s' неправильно задан вход 'Обороты'", ObjName.Str );
    return 1;
    }
  return 0;
  }

int CPC_1::PutAddr( char * Point, void * pData, char * Name )
  {
  ASS(lstrcmp(Point,"Обороты")==0)
  pMotorData = (COborot*)pData;
  return 0;
  }
*/
#include "stdafx.h"
#include "MultiSens.h"
#include "Err.h"
#include "Work.h"

/*
int CMultiSens::TestCtrlPoints( int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( kNodes < 1 )
    {
    ModMsg ( "У измерителя '%s' нет входов", ObjName );
    return 1;
    }
  int kIn = 0;
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    if ( pOP->Point == "Обороты" )
      {
      pAnalogPipe = pOP->pPipe;
      pOP->PntGroup = n+1;
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      kIn++;
      continue;
      }
// Все остальные выходы
    pAnalogPipe = pOP->pPipe;
    Char<128>Source;
    Source.Prt ( "%s.Обороты", ObjName );
    pAnalogPipe->pControl->SetAddr( &ExtOborot, this, Source );
    pOP->PntGroup = n+1;
    pOP->NumbInGroup = 0;
    pOP->IO = '-';
    }
  if ( kIn != 1 )
    {
    ModMsg ( "У измерителя '%s' неправильно задан вход 'Обороты'", ObjName );
    return 1;
    }
  return 0;
  }
int CMultiSens::PutAddr( char * Point, void * pData, char * Name )
  {
  if ( lstrcmp( Point, "Обороты" ) == 0 )
    {
    pMotor = (COborot*)pData;
    return 0;
    }
  return 0;
  }
*/
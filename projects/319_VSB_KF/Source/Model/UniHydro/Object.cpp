#include "stdafx.h"
#include "UniHydro.h"
#include "Err.h"


void CGroupObject::Init()
  {
  //    pObject = NULL;
  pFirstGroupPoint = NULL; 
  pLastGroupPoint = NULL;
  pFirstPoolPoint = NULL; 
  pLastPoolPoint = NULL;
  kPoints = 0;
  Order = -1;
  PntGroup = -1;
  A = NULL;
  l_A = 0;
  Ind_A = NULL;
  B = NULL;
  IsMatrInd = false;
  MainPool = false;
  }
void CGroupObject::Add( CObjectPoint * pP )
  {            
  if ( pFirstGroupPoint == NULL )
    pFirstGroupPoint = pP;
  if ( pLastGroupPoint )
    pLastGroupPoint -> pNextGroupPoint = pP;
  pLastGroupPoint = pP;
  pP -> pNextGroupPoint = NULL;
  kPoints++;
  if ( PntGroup < 0 )
    PntGroup = pP->PntGroup;
  else
    {
    ASS( PntGroup == pP->PntGroup );
    }
  }
void CGroupObject::AddPntToPool( CObjectPoint * pP )
  {
  if ( pFirstPoolPoint == NULL )
    pFirstPoolPoint = pP;
  if ( pLastPoolPoint )
    pLastPoolPoint -> pNext = pP;
  pLastPoolPoint = pP;
  pP -> pNext = NULL;
  kPoints++;
  }

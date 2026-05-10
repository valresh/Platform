#include "stdafx.h"
#include "UniHydro.h"
#include "Err.h"
#include "Data.h"


void CVarPool::Init()
  {
  kPoints = 0;
  kScalarVar = 0;
  pFirstXY = NULL;
  pLastXY = NULL;
  pFirstPoint = NULL;
  pLastPoint = NULL;
  pFirstPoolPnt = NULL;
  pLastPoolPnt = NULL;
  pFirstProp = NULL;
  pLastProp = NULL;
  pNextPool = NULL;
  pPipe = NULL;
  }

void CVarPool::Add( CPoolPoint * pP )
  {
  if ( pFirstPoolPnt == NULL )
    pFirstPoolPnt = pP;
  if ( pLastPoolPnt )
    pLastPoolPnt -> pNext = pP;
  pLastPoolPnt = pP;
  pP->pNext = NULL;
  if ( strstr ( pP->ObjName, "11293"))
    KKK();
  kPoints++;
  }

void CVarPool::Add( CObjectPoint * pP )
  {
  if ( pFirstPoint == NULL )
    pFirstPoint = pP;
  if ( pLastPoint )
    pLastPoint->pNextPoolPoint = pP;
  pLastPoint = pP;
  pP->pNextPoolPoint = NULL;
  pP->pPool = this;
  kPoints++;
  }

void CVarPool::Add( CConnectionXY * pXY )
  {
  if ( pFirstXY == NULL )
    pFirstXY = pXY;
  if ( pLastXY )
    pLastXY -> pNext = pXY;
  pLastXY = pXY;
  pXY->pNext = NULL;
  }

void CVarPool::Add( struct CObjProps * pP )
  {
  if ( pFirstProp == NULL )
    pFirstProp = pP;
  if ( pLastProp )
    pLastProp -> pNext = pP;
  pLastProp = pP;
  pP->pNext = NULL;
  }

void CVarPool::SetThisObject( )
  {
  switch ( Type )
    {
    case eГидравлика:
      {
      pThisObject = Load_Obj ( (const char*)Name, "Pipe" );
      }
    break;
    case eTransfer:
      {
      pThisObject = Load_Obj ( (const char*)Name, "move" );
      }
    break;
    case eConnection:
      {
      pThisObject = NULL;
      }
    break;
    case eMotor:
      {
      pThisObject = Load_Obj ( (const char*)Name, "Driver" );
      }
    break;
    case eSignal:
      {
      pThisObject  = Load_Obj ( (const char*)Name, "Signal" );
      }
    break;
    case ePipeFlow:
      {
      pThisObject  = Load_Obj ( (const char*)Name, "PipeFlow" );
      }
    break;
    case eContact:
      {
      pThisObject  = Load_Obj ( Name.Str, "VolPourPool" );
      }
    break;
    default:
      ASS(0)
    break;
    }
  }

bool CVarPool::HaveMatr( )
  {
  switch ( Type )
    {
    case eГидравлика:
    case ePipeFlow:
    return true;
    default:
    return false;
    }
  return false;
  }

//void CPipeRef::Init()
//  {
//  kSeg = 0;
//  pFirstSeg = NULL;
//  pLastSeg = NULL;
//  pPool = NULL;
//  pObject = NULL;
//  Selected = false;
//  Break = false;
//  }

CSegment * CPipeRef::AddSegment()
  {
  CSegment * pSeg = NewArr ( CSegment, 1 );
  if ( pFirstSeg == NULL )
    pFirstSeg = pSeg;
  if ( pLastSeg )
    pLastSeg->pNext = pSeg;
  pLastSeg = pSeg;  
  pSeg->pNext = NULL;
  return pSeg;
  }


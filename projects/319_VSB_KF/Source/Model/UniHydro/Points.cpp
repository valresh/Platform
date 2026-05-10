#include "stdafx.h"
#include "UniHydro.h"
#include "Err.h"
//#include "Spider.h"

#define MAX_POINTS 256


int CUniHydro::ObrUniModelPoints( CObjectRef & Obj )
  {
  CObjectPoint * pPoints[MAX_POINTS];
  CObjectPoint * pSignalPoints[MAX_POINTS];
  CObjectPoint * pPnt = Obj.pFirstPoint;
  int kPoints = 0;
  int kSignalPoints = 0;
  while ( pPnt )
    {
    ASS(kPoints<MAX_POINTS)
    char * Name = pPnt->PntName;
    if ( strncmp ( Name, "ai_", 3 ) == 0 ||
         strncmp ( Name, "ao_", 3 ) == 0 ||
         strncmp ( Name, "pi_", 3 ) == 0 ||
         strncmp ( Name, "po_", 3 ) == 0 )
      pSignalPoints[kSignalPoints++] = pPnt;
    else
      pPoints[kPoints++] = pPnt;
    pPnt = pPnt->pNext;
    }
//  if ( )
  int Res = Obj.pObject->pUniModel->TestNodes( kPoints, pPoints );
  if ( Res )
    return Res;
  // if ( Obj.pObject->pSpider )
  //   Obj.pObject->pSpider->TestNodes( kPoints, pPoints );
// Разбираемся с pSignalPoints
  for ( int n = 0; n < kSignalPoints; n++ )
    {
    CObjectPoint * pPnt = pSignalPoints[n];
    char * Name = pPnt->PntName;
    if ( strncmp ( Name, "ai_", 3 ) == 0 ||
         strncmp ( Name, "pi_", 3 ) == 0 )
      pPnt->IO = '+';
    else
      pPnt->IO = '-';
    pPnt->PntGroup = 40000 + n;
    pPnt->NumbInGroup = 0;
    }
  //  
  //pPnt = Obj.pFirstPoint;
  //int nPoints = 0;
  //while ( pPnt )
  //  {
  //  pPnt->PntGroup = Points[nPoints].GrpPoint;
  //  pPnt->IO = Points[nPoints].TypeF;
  //  pPnt->Type = eГидравлика;
  //  nPoints++;
  //  pPnt = pPnt->pNext;
  //  }
  //
  return 0;
  }

/*
struct CObjPoint
  {
  int N_Pipe;
  CStr Pipe;
  CStr Point; ++
  int GrpPoint;
  int IntNumb;  ++
  char TypeF;   ++
  int nGroup;
  struct IBaseModel * pPipe;
  CObjPoint * pNext;
  int Label; 
  bool UseLabel;
  void Init()
    {
    N_Pipe = -1;
    GrpPoint = -1;
    IntNumb = -1;
    TypeF = ' ';
    nGroup = -1;
    pPipe = NULL;
    pNext = NULL;
    Point.Str = NULL;
    Pipe.Str = NULL;
    Label = -1;
    UseLabel = false;
    }
  char * Pnt()
    {
    static char L[128];
    strcpy_s( L, 128, (char*)Point );
    strlwr( L );
    return L;
    }
  };
*/


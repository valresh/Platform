#include "stdafx.h"
#include "Driver.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "HydroStruct.h"

#undef IF
#define IF(Node) if ( lstrcmp ( Node, Name ) == 0 )

int CDriver::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  return 0;
  }

int CDriver::SetMatr( void * pExternals, int PntGroup,  double dt, 
                     double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  ASS(0)
  return 0;
  }

int CDriver::SetVar( void * pExternals, int PntGroup, double dt, 
                    // Переменные на входах
                    double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  SET_BP BreakPoint;
  return 0;
  }


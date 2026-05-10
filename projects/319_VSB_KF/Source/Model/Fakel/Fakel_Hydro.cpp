#include "stdafx.h"
#include "Fakel.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "HydroStruct.h"
#include "TestNodes.h"

/*
NODE_IN(1,"gas","сжигаемый газ")
NODE_IN(2,"topl_gas","топливный газ")
NODE_IN(3,"steam","пар на горелку")

*/
#undef IF
#define IF(Node) if ( lstrcmp ( Node, Name ) == 0 )

struct CNodeInfo
  {
  char * Name;
  char * Desc;
  int Group;
  int NumbInGroup;
  char Type;
  //
  int Set;
  };

NODE_LIST
	NODE_IN  ( 1, 0, "gas","сжигаемый газ")
	NODE_IN ( 2, 0, "topl_gas","топливный газ")
	NODE_IN  ( 3, 0, "steam","пар на горелку")
END_LIST

STD_TEST

extern "C"
EXP int TestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
{
  return StdTestNodes(ObjName, kNodes, ppNodes);
  
}

int CFakel::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
    return ::TestNodes(ObjName, kNodes, ppNodes);
  }

#include "SetMatr.h"

int CFakel::SetMatr( void * pExternals, int PntGroup,  double dt, 
                    double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  SET_BP BreakPoint;
  switch ( PntGroup )
    {
    case 1:
      {
//      "gas","сжигаемый газ", 1, 0, '+', 0,
      MATR_IN( 1., OmegaGas );
      return 0;
      }
    case 2:
      {
//      "topl_gas","топливный газ", 2, 0, '+', 0,
      MATR_IN( 1., OmegaTopl );
      return 0;
      }
    case 3:
      {
//      "steam","пар на горелку", 3, 0, '+', 0,
      MATR_IN( 1., OmegaSteam );
      return 0;
      }
    default:
      ASS(0)
    break;
    }
  return 0;
  }

#include "SetVar.h"

int CFakel::SetVar( void * pExternals, int PntGroup, double dt, 
                   double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  SET_BP BreakPoint;
  switch ( PntGroup )
    {
    case 1:
      {
      IO_1(pGas_in)
      return 0;
      }
    case 2:
      {
      IO_1(pFuel_in)
      return 0;
      }
    case 3:
      {
      IO_1(pSteam_in)
      return 0;
      }
    default:
      ASS(0)
    break;
    }
  return 0;
  }

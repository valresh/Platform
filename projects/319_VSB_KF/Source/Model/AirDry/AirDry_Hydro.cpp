#include "stdafx.h"
#include "AirDry.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "TestNodes.h"
#include "HydroStruct.h"

#undef IF  ///create
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
  };///create

//NODE_IN(1, "i",	"Воздух воздуха")
//NODE_OUT(1, "o",	"Выход воздуха"
CNodeInfo NodeInfo[] = {///create
		"i",	"Воздух воздуха",	1,	0,	'+',	0,
		"o",	"Выход воздуха",	1,	1,	'-',	0,
	};///create

///////////////////////////////////////////////////

int kNodeInfo = sizeof ( NodeInfo )/sizeof ( CNodeInfo );

extern "C"
EXP int TestNodes(  char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
  {
  int kErr = 0;
  for ( int i = 0; i < kNodeInfo; i++)
    NodeInfo[i].Set = 0;
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    pOP->Type = eГидравлика;
    char * Name = NodeNoNumb ( pOP->Point );
    bool OK = false;
    for ( int i = 0; i < kNodeInfo; i++)
      {
      if ( lstrcmp( NodeInfo[i].Name, Name ) == 0 )
        {
        OK = true;
        NodeInfo[i].Set++;
        break;
        }
      }
    if ( OK )
      continue;
    ModMsg ( "У осушителя '%s' ошибочный вход '%s'", ObjName, pOP->Point ), kErr++; //факеоа
    }
  int kMsg = 0;
  for ( int i = 0; i < kNodeInfo; i++)
    {
    if ( NodeInfo[i].Set == 0 )
      ModMsg ( "#У осушителя '%s' не задан '%s'", ObjName, NodeInfo[i].Desc ), kMsg++;
    if ( NodeInfo[i].Set > 1 )
      ModMsg ( "У осушителя '%s' ошибочно задан '%s'", ObjName, NodeInfo[i].Desc ), kErr++, kMsg++;
    }
  return kErr;
  }

int CAirDry::TestNodes( int kNodes, struct CObjectPoint ** ppNodes )
  {
  int Res = ::TestNodes( (char*)ObjName, kNodes, ppNodes );
  if ( Res )
    return Res;
  int kErr = 0;
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    pOP->Type = eГидравлика;
    char * Name = pOP->PntName;
    bool OK = false;
    for ( int i = 0; i < kNodeInfo; i++)
      {
      if ( lstrcmp( NodeInfo[i].Name, Name ) == 0 )
        {
        OK = true;
        pOP->PntGroup = NodeInfo[i].Group;  
        pOP->NumbInGroup = NodeInfo[i].NumbInGroup;
        pOP->IO = NodeInfo[i].Type;
        break;
        }
      }
    if ( OK )
      continue;
    ModMsg ( "У осушителя '%s' ошибочный вход '%s'", ObjName, pOP->Point ), kErr++;
    }
  return kErr;
  }

#include "SetMatr.h"

int CAirDry::SetMatr( void * pExternals, int PntGroup,  double dt, 
    double A[/* kExkV */], double B[ /*kE*/ ] )
  {
  SET_BP BreakPoint;
  MATR_OMEGA(Omega)
  return 0;
  }

#include "SetVar.h"

int CAirDry::SetVar( void * pExternals, int PntGroup,  double dt, 
		double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  SET_BP BreakPoint;
  IO_2(pFlow_in,pFlow_out)
  return 0;
  }


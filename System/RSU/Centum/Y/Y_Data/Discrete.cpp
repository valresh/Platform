#include "stdafx.h"
#include "Y_Data.h"

#include "StdDef.h"
#include "ReadParms.h"



CZ_OBJ::CZ_OBJ()
	{
  memset ( Name, 0, sizeof ( Name ));
  Out[0].Type = CSignal::Z_OBJ;
  Out[1].Type = CSignal::Z_OBJ;
  Out[2].Type = CSignal::Z_OBJ;
  Out[0].Name = "PV/OUT";
  Out[1].Name = "PV.0";
  Out[2].Name = "PV.1";
  IN = NULL;
	Type = 0;
	Numb = 0;
	pZ = NULL;
	PV_old = PV_0 = PV_1 = 0;
  CLEAR(Out)
	Used = false;
	pMain = NULL;
  }

CSignal * CZ_OBJ::GetOUT( char * Name ) 
	{ 
	if ( strcmp ( Name, "OUT" ) == 0 ||
			 strcmp ( Name, "PV" ) == 0 )
		{
	  Out[0].Name = "PV/OUT";
		return &Out[0]; 
		}
	if ( strcmp ( Name, "PV.0" ) == 0 )
		{
	  Out[1].Name = "PV.0";
		Out[1] << PV_0;
		return &Out[1]; 
		}
	if ( strcmp ( Name, "PV.1" ) == 0 )
		{
	  Out[2].Name = "PV.1";
		Out[2] << PV_1;
		return &Out[2]; 
		}
	return NULL;
	};

CSignal ** CZ_OBJ::GetIN ( ) 
	{ 
	return &IN; 
	};

int * CZ_OBJ::pPV()
	{
	int * P = NULL;
	switch ( Type )
		{
		case id__Z:	
			P = &pZ->PV;
		break;
    case id__Y:	
      P = &pY->PV;
      break;
		case id__AN:
			P = &pAN->PV;
		break;
		case id__WB:
			P = &pWB->PV;
		break;
		case id__GS:
			P = &pGS->PV;
		break;
		case id__SW:
			P = &pSW->PV;
		break;
		default:
			ASS(FALSE);
		break;
		}
	return P;
	}

int CZ_OBJ::PV()
	{
	int P = 0;
	switch ( Type )
		{
		case id__Z:	
			P = pZ->PV;
		break;
    case id__Y:	
      P = pY->PV;
      break;
		case id__AN:
			P = pAN->PV;
		break;
		case id__WB:
			P = pWB->PV;
		break;
		case id__GS:
			P = pGS->PV;
		break;
		case id__SW:
			P = pSW->PV;
		break;
		default:
			ASS(FALSE);
		break;
		}
	return P;
	}

char * CZ_OBJ::Struct()
	{
	char * Name = "";
	switch ( Type )
		{
		case id__Z:	
			Name = "%Z";
		break;
    case id__Y:	
      Name = "%Y";
      break;
		case id__AN:
			Name = "%AN";
		break;
		case id__WB:
			Name = "%WB";
		break;
		case id__GS:
			Name = "%GS";
		break;
		case id__SW:
			Name = "%SW";
		break;
		default:
			ASS(FALSE);
		break;
		}
	return Name;
	}

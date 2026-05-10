#include "stdafx.h"
#include "VEP.h"
#include "Err.h"
//#include "CommProc.h"
#include "Work.h"
#include "SetData.h"

static LPCSTR g_szHelpFileName = "Базовый вентиль.pdf";

CVEP::CVEP( char * _ObjName, char* _Type ) :	CValve_M_b ( _ObjName, _Type ) 
#include "ACS_Constr.h"
#include "VEP_ACS.h"
{
#include "IO_Clear.h"
#include "Vep_Pnt.h"
#include "VEP_Defects.h"
}

CVEP::~CVEP()
{

}


int CVEP::SaveState( )
{
	S_CLASS("_W_VEP", _W_VEP);
	return CValve_M_b::SaveState();
}

int CVEP::RestoreState( char * StrName )
{
	COPY_FIRST;
	R_CLASS("_W_VEP", _W_VEP);
	return CValve_M_b::RestoreState(StrName);	
}












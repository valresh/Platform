#include "Y_Data.h"
#include "math.h"
#include <rsuErr.h>

#include "StdDef.h"

//////////////////////////////////////////////////////////////////////////////
CY_Create * CY_Create::pFirst = NULL;
CY_Create * CY_Create::pLast = NULL;

CY_Create::CY_Create( char * _ModelName,	tYCreateNP _pCreate )
	{
	if ( pFirst == NULL )
		pFirst = this;
	if ( pLast )
		pLast -> pNext = this;
	pLast = this;
	pNext = NULL;
	ModelName = _ModelName;
	pCreateNP = _pCreate;
	pCreateCP = NULL;
	pCreateIP = NULL;
	Parm = "";
	}

CY_Create::CY_Create( char * _ModelName,	tYCreateCP _pCreate, char * _Parm )
	{
	if ( pFirst == NULL )
		pFirst = this;
	if ( pLast )
		pLast -> pNext = this;
	pLast = this;
	pNext = NULL;
	ModelName = _ModelName;
	pCreateCP = _pCreate;
	pCreateNP = NULL;
	pCreateIP = NULL;
	Parm = _Parm;
	}

CY_Create::CY_Create( char * _ModelName,	tYCreateIP _pCreate, int _Parm )
	{
	if ( pFirst == NULL )
		pFirst = this;
	if ( pLast )
		pLast -> pNext = this;
	pLast = this;
	pNext = NULL;
	ModelName = _ModelName;
	pCreateIP = _pCreate;
	pCreateNP = NULL;
	pCreateCP = NULL;
	iParm = _Parm;
	}

CY_Base * CY_Create::Create ( char * ModelName, char * _ObjName, int number, int domain )
{
	CY_Create * pC = pFirst;
	while ( pC )
		{
		if ( strcmp ( ModelName, pC->ModelName ) == 0 )
			{
      CY_Base * pB = NULL;
      if ( pC->pCreateCP )
        pB = (*(pC->pCreateCP))( _ObjName, pC->Parm, number, domain );
      if ( pC->pCreateIP )
			  pB = (*(pC->pCreateIP))( _ObjName, pC->iParm, number, domain );
      if ( pC->pCreateNP )
			  pB = (*(pC->pCreateNP))( _ObjName, number, domain );
      ASS(pB!=NULL)
      strcpy_s ( pB->Model, ModelName );
      strcpy_s ( pB->CY_Base::Name, _ObjName );
      return pB;
			}
		pC = pC ->pNext;
		}
	return NULL;
}
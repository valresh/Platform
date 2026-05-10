#include "stdafx.h"
#define DLL_ACS
#include "ACS_Def.h"
#include "Connect.h"

//CSavePnt * CExtern_Pnt::pSave;
//int CExtern_Pnt::kPnt;

int FindSave(const void * p1, const void * p2)
{
	char * s1 = (char*)p1;
	CSavePnt * s2 = (CSavePnt*)p2;
	return lstrcmp ( s1, s2->Name );
}


CACS::CACS( char * _ObjName ) :	IBaseModel ( _ObjName ) 
  {
  ObjName = _ObjName;
  Model = "ACS";
  }

CACS::~CACS()
  {

  }

_W_CACS::_W_CACS()
  {
  memset( this, 0, sizeof ( *this ));
  }

int CACS::Init( int Regim )
  {
  return 0;
  }

int CACS::Step0( )
  {
  return 0;
  }

int CACS::Step1()
{
	if(CExtern_Pnt::kPnt <= 0) return 0;
	CExtern_Pnt * pPnt = pFirst_Extern_Pnt;
	char PntName[64];
	while(pPnt)
	{
		if(!pPnt->IsConnection)
		{
			pPnt = pPnt->pNext_Extern_Pnt;
			continue;
		}
		sprintf_s(PntName, 64, "%s.%s", pPnt->pModel->ObjName.Str, pPnt->Point.Str);
		CSavePnt * pS = (CSavePnt*)bsearch(PntName, CExtern_Pnt::pSave, CExtern_Pnt::kPnt, sizeof(CSavePnt), FindSave);
		if(pS)
		{
			if(pPnt->Type[0] == 'A')
			{
				CAnalog * pA = (CAnalog*)pPnt;
				pA->Value = pS->Dbl;
				pPnt = pPnt->pNext_Extern_Pnt;
				continue;
			}
			if(pPnt->Type[0] == 'D')
			{
				CDiscrete * pD = (CDiscrete*)pPnt;
				pD->Value = pS->Int;
				pPnt = pPnt->pNext_Extern_Pnt;
				continue;
			}
		}
		pPnt = pPnt->pNext_Extern_Pnt;
	}
	return 0;
}

int CACS::StepT(double dt)
  {
  return 0;
  }

int CACS::GetParams( char * )
  {
  return 0;
  }

int CACS::UpdateParam( struct CParams & Param )
  {
  return 0;
  }

int CACS::SetData( int TypeData, void * pData )
  {
  return 0;
  }

int CACS::SaveState ( )
  {
  pSRFile->WStruct( "Точки", 0, NULL );
  CExtern_Pnt::SaveAll( );
  return 0;
  }

int CACS::RestoreState ( char * StrName )
  {
  ASS ( lstrcmp ( StrName, "Точки" ) == 0 )
  CExtern_Pnt::RestoreAll( );
  return 0;
  }


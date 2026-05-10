#include "Y_MainLogic.h"

CY_Base* CY_MainLogic::CreateYobj( char * Name, char * Model, int number, int domain )
{
  if( !Name[0] )
    return NULL;

  CY_Base * pY = CY_Create::Create( Model, Name, number, domain );
  if( !pY )
    return pY;
  strcpy_s( pY->Name, Name );
  return pY;
}

void CY_MainLogic::OnYobjFullNameReady( CY_Base* pObj )
{
}

CY_DI* CY_MainLogic::CreateDobj( char * Name, int Y_Type, int NumbFCS, int domain, int numb )
{
  CY_DI * pY = CY_DI::Create( Name, Y_Type, NumbFCS, domain, numb );
  if( !pY )
    return pY;
  pY->domain = domain;
  pY->NumbFCS = NumbFCS;
  strcpy_s( pY->Name, Name );
  pY->Y_Type = Y_Type;
  pY->Numb = numb;
  return pY;
}

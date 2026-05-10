#include "stdafx.h"
#include "VBR.h"
#include "Err.h"
#include "CommProc.h"


_W_VBR::_W_VBR()
	{
  memset ( this, 0, sizeof ( _W_VBR ));
  k_Flow_IO = 1.;
	}


int CVBR::GetParams( char * )
{
#include "IO_Parms.h"
#include "Points.h"
	bool No_React[K_GAS];
	Char<128>Name; 
	TAB("Не реагируют", 1 )
	for ( int c = 0; c < LAST_GAS; c++ )
	{
		Name.Prt( "%s-пассивна", pGas[c]->Name );
		PARM( No_React[c], Name );
	}
	ETAB
	TAB("Поправки на выход", 1 )
	PARM( kCompMax, "Максимум поправки" )
	for ( int c = 0; c < LAST_GAS; c++ )
	{
		Name.Prt( "%s-k выхода", pGas[c]->Name );
		PARM( kComp[c], Name );
	}
	ETAB
	TAB("Состав на выходе", 1 )
	for ( int c = 0; c < LAST_GAS; c++ )
	{
		Name.Prt( "#%s-моль %%", pGas[c]->Name );
		PARM( CmolOpt[c], Name );
	}
	ETAB
	TAB("Эталон", 1 )
	PARM( Etalon, "Состав" )
	PARM( SetOptKoef, "#Задать эталонные к-ты" )
	for ( int c = 0; c < LAST_GAS; c++ )
	{
		Name.Prt( "#%s-вых/эталон", pGas[c]->Name );
		PARM( OutToEtalon[c], Name );
	}
	ETAB
	ACS_Params( this );
	return 0;
}

int CVBR::UpdateParam( CParams & Param )
  {
  if ( Param.Addr == Etalon )
    {
    CompEtalon.Read ( Etalon );
    return 0;
    }
  bool Change_k = false;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( Param.Addr == &kComp[c] )
      {
      Change_k = true;
      break;
      }
    }
  if ( Change_k )
    return 0;
  bool Change_C = false;
  for ( int c = 0; c < LAST_GAS; c++ )
    {
    if ( Param.Addr == &CmolOpt[c] )
      {
      Change_C = true;
      break;
      }
    }
  if ( Change_C )
    {

    }
  return 0;
  }

//#include "ShowParams.h"

int CVBR::ShowParams( DWORD DataType, struct CShowData * pSD )
  {
/*
	switch ( DataType )
		{
		case sp_Params:
		  GetParams( pSD->N, pSD->Params );
		return 1;
		}
*/
  return 0;
  }

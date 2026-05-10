#include "stdafx.h"
#define DLL_VolBase
#include "VolBase_b.h"
#include "math.h"
#include "Err.h"

int CVolBase::UpdateParam( struct CParams & Param )
  {
  // if ( Param.Addr == &Залить && Залить )
  //   {
  //   CSrcCompData * pData = (CSrcCompData*)Залить;
  //   Залить = 0;
  //   if ( pData->ID != 0xAF42314E )
  //     return 1;
  //   SetComp( &pData->Comp, pData->T, pData->Lev );
  //   }
  return 0;
  }

int CVolBase::GetParams(  char * )
  {
  KKK();
#include "IO_Parms.h"
#include "VolBasePoints_b.h"
  char Name[256];
  //TAB("Henry",5)
  //for ( int n = 0; n < LAST_GAS; n++ )
  //	{
  //	Sprintf ( Name, "H-%s", pGas[n]->Name );
  //	PARM_D( StateVol.Henry[n], Name, 1. );
  //	}
  TAB("Очистка",5)
    PARM_B( Delete, "Очищать", false )
    PARM_D( k_Delete, "Скорость очистки", 0.1 )
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      Sprintf ( Name, "Очищать %s", pGas[n]->Name );
      PARM( IsDelete[n], Name );
      }
    for ( int n = 0; n < LAST_GAS; n++ )
      {
      Sprintf ( Name, "#-Поток %s", pGas[n]->Name );
      PARM( F_Del[n], Name );
      }
    //  TAB("Модули",5)
    ETAB
      return 0;
  }

CVolBase_P::CVolBase_P()
  {
  memset ( this, 0, sizeof ( CVolBase_P ));
  Max_dM = 0.05;
  D_trub = 0.025;
  NoFunnel = false;
  M_korp_t = 0.5;
  k_dt = 1.;
  Ro_liq = 1000.;
  Vol = 10.;
  Add_1F = Add_2F = Add_Comp = 0.;
  lstrcpy ( Add_Comp_Name, "Вода" );
  Add_Comp_T = 15.;
  Gas_Vol = 1.;
  Liq_Vol = 1.;
  }


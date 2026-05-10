#include "stdafx.h"
#define DLL_BaseCol
#include "BaseCol_NE.h"
#include "math.h"
#include "Err.h"
#include "SetData.h"

#if 0
struct IN_DLL I_ColAdditionList : public I_AdditionList
  {
  I_Addition * pFirst;
  I_Addition * pLast;
  void Init( );
  void InitAdditions( void * pData );
  void Add( I_Addition * pAdd );
  void Gamma(  void * pData, double P, double T, bool IsComp[K_GAS], double Cmol[K_GAS], double P_sat_id[K_GAS], double Gamma[K_GAS], double & dH_liq );
  void GetParams( IBaseModel * pMain );
  void UpdateParams( );
  };

void I_ColAdditionList::Init( )
  {
  pFirst = NULL;
  pLast = NULL;
  }

void I_ColAdditionList::Add( I_Addition * pAdd )
  {
  if ( pFirst == NULL )
    pFirst = pAdd;
  if ( pLast )
    pLast->pNext = pAdd;
  pLast = pAdd;
  pAdd->pNext = NULL;
  }

void I_ColAdditionList::Gamma( void * pData, double P, double T, bool IsComp[K_GAS], double Cmol[K_GAS], double P_sat_id[K_GAS], double Gamma[K_GAS], double & dH_liq )
  {
  I_Addition * pAdd = pFirst;
  while ( pAdd )
    {
    pAdd->Gamma(  pData, P, T, IsComp, Cmol, P_sat_id, Gamma, dH_liq );
    pAdd = pAdd->pNext;
    }
  }

void I_ColAdditionList::InitAdditions( void * pData )
  {
  I_Addition * pAdd = pFirst;
  while ( pAdd )
    {
    pAdd->Init( pData );
    pAdd = pAdd->pNext;
    }
  }

void I_ColAdditionList::GetParams( IBaseModel * pMain )
  {
  I_Addition * pAdd = pFirst;
  while ( pAdd )
    {
    pAdd->GetParams( pMain );
    pAdd = pAdd->pNext;
    }
  }


void I_ColAdditionList::UpdateParams( )
  {
  I_Addition * pAdd = pFirst;
  while ( pAdd )
    {
    pAdd->UpdateParams( );
    pAdd = pAdd->pNext;
    }
  }
#endif

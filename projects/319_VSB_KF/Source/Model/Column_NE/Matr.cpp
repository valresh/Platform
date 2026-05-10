#include "stdafx.h"
#define DLL_COLUMN
#include "Column_NE.h"



void CColumn_NE::GetGasFlowSrc ( double * Vs )
  {
  for ( int n = 0; n < kIO; n++ )
    {
    CExtIO * pS = Src[n];
    if ( pS == NULL )
      continue;
    if ( !pS->StaticFlow )
      continue;
    int nS = nST[n];
    if ( pS->pFlow == NULL )
      continue;
    Vs[nS] += pS->pFlow->Flow_mol * pS->Eps;
    }
  }


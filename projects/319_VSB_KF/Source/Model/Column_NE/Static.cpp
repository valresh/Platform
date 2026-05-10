#include "stdafx.h"
#define DLL_COLUMN
#include "Column_NE.h"
#include "2x2.h"


#if 0
void CColumn_NE::GetStaticFlowSrc ( )
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
    double F_gas = pS->pFlow->Flow_mol * pS->Eps;
    double F_liq = pS->pFlow->Flow_mol  - F_gas;
//    Column.pStage[nS]->Static.F_source_gas += F_gas;
//    Column.pStage[nS]->Static.F_source_liq += F_liq;
    }
  }

#endif

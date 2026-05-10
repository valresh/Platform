#pragma once
#include "../FlexRsuCore/FlexDR.h"

class KOpenInGraph
{
  HMODULE m_hGui;
public:
  KOpenInGraph();
  void Show( LPCSTR pszPrjName, LPCSTR pszFileName, KFlexDR *pDR );
};

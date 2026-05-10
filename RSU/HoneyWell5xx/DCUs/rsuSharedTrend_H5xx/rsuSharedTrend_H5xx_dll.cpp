#include <crossplatform.h>
#include <rsuNoNames.h>
#include <TrendsSup.h>
#include <BaseType_.h>
#include <stdlib.h>
#include <rsuErr.h>
#include <Names.h>
#include <UtilsH.h>

#ifdef _MANAGED
#pragma managed(push, off)
#endif

KNoName *g_Objs, *g_IOs;

#ifndef _WIN32
void OnLoadLibrarySTH5() __attribute__((constructor));
#else
void OnLoadLibrarySTH5();
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
        OnLoadLibrarySTH5();
    break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
  return TRUE;
}
#endif

#ifdef _MANAGED
#pragma managed(pop)
#endif
void OnLoadLibrarySTH5()
{
    g_Objs = KNoName::Create("H5xx_Objs", false);
    g_IOs = KNoName::Create("H5xx_IOs", false);
}

extern "C" _EXP bool EnumObjs(DWORD& ID, const DWORD ClassID, struct CBase** base, LPCSTR* pszName, int *pFcsNumber )
{
  return g_Objs->WhileBase( ID, ClassID, base, pszName, pFcsNumber );
}

extern "C" _EXP bool EnumIOs(DWORD& ID, const DWORD ClassID, struct CBase** base, LPCSTR* pszName, int *pFcsNumber )
{
  return g_IOs->WhileBase( ID, ClassID, base, pszName, pFcsNumber );
}

struct SAddTrend 
{
  char name[16*4];
  SValueDef* def[id_AcyMax-id_AcyMin+1];
};

SAddTrend list[] =
{
  { "PV"  },
  { "SP"  },
  { "OP"  },
  { "PVP" },
  { "SPP" },
  { "C(1)" },
  { "C(2)" },
  { "C(3)" },
  { "C(4)" },
  { "C(5)" },
  { "C(6)" },
  { "C(7)" },
  { "C(8)" },
  { "C[1]" },
  { "C[2]" },
  { "C[3]" },
  { "C[4]" },
  { "C[5]" },
  { "C[6]" },
  { "C[7]" },
  { "C[8]" },
};

extern "C" _EXP int TrendInfoPoint(DWORD type, LPCSTR pObjName, CBase* pBase, int fcsNumber, int cInfs, STrendPointInfo *pInfs )
{
  DWORD N = pBase->ID_CLASS;
  if( !IsAcy(N) )
    return 0;

  static bool bInited = false;
  if( !bInited )
  {
    bInited = true;
    for ( UINT n = 0; n < _countof(list); n++ )
    {
      for ( UINT m = id_AcyMin; m < id_AcyMax; m++ )
      {
        list[n].def[m-id_AcyMin] = NameToValueH( m, list[n].name );
      }
    }
  }

  int c = 0;
  for ( UINT n = 0; n < _countof(list); n++ )
  {
    if( list[n].def[N-id_AcyMin] == NULL )
      continue;
    if( c>= cInfs )
      break;
    STrendPointInfo &obj = pInfs[c];
    obj.eValType = list[n].def[N-id_AcyMin]->eVal;
    obj.dwID = GetID( pObjName, list[n].name, fcsNumber );
    ASS( obj.dwID );
    obj.pAddr = (BYTE*)(((BYTE*)pBase)+list[n].def[N-id_AcyMin]->dwShift);
    obj.pszSufName = list[n].name;
    c++;
  }

  return c;
}

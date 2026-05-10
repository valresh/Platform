#include "SafetyManager.h"
#include <rsuNoNames.h>
#include <SafetyManagerStruct.h>
#include <rsuFormats.h>

extern KNoName *g_IOs;

int KSafetyManager::StepTL( double dtS )
{
  DWORD ID = 0;
  CBase* pBase = NULL;
  int nFcs = 0;
  const char* pszName;
  int added = 0;

  while( g_IOs->WhileBase( ID, -1, &pBase, &pszName, &nFcs ) )
  {
    switch( pBase->ID_CLASS )
    {
    case W_SMDI::TypeID:
      {
        W_SMDI *W = (W_SMDI*)pBase;
        W->PV = W->PVFL;
      }
      break;
    case W_SMDICOM::TypeID:
      {
        W_SMDICOM *W = (W_SMDICOM*)pBase;
        W->PV = W->PVFL;
      }
      break;
    default:
      continue;
    }
  }

  mList.Timer( this, dtS );


#if 0
  static int nCall = 1;

  std::string logName = "log/sm" + std::to_string(nCall) + ".log";
  std::fstream stream(logName, std::ios_base::app);
  nCall++;
  char szTmpName[64] = {};
  for( UINT n = 0, nCount(mList.Count()); n < nCount; n++ )
  {
    IFscStorage::SVarInfo vars[10] = { 0 };
    LPCSTR pPntName = NULL;
    SFscBase* obj = mList.Item(n);
    const int C = mList.GetVars( this, obj, vars, _countof(vars), &pPntName );
    if( !C )
      continue;
    if( !pPntName || !*pPntName )
    {
      sprintf_s( szTmpName, "%d", obj->ID );
      pPntName = szTmpName;
    }

    stream << "ID=" << obj->ID << " PntName='" << pPntName << "'" << std::endl;

    for( int i=0; i<C; ++i )
    {
        char szVal[32] = {};
        if (vars[i].val)
        {
            switch (vars[i].val->eType)
            {
            default:
                ASSD(0);
                break;
            case enumValueBol:
                strcpy_s(szVal, vars[i].val->bVal ? "1" : "0");
                break;
            case enumValueDbl:
            {
                std::string s;
                DblToStr(s, vars[i].val->dVal);
                strcpy(szVal, s.c_str());
                break;
            }
            case enumValueInt:
                sprintf_s(szVal, "%d", vars[i].val->nVal);
                break;
            case enumValueChr:
                sprintf_s(szVal, "%d", vars[i].val->cVal);
                break;
            }

            stream << "\t" << vars[i].szName << "='" << szVal << "'" << " type(" << (int)vars[i].val->eType << ")" << std::endl;
        }
        else
        {
            stream << "\t" << vars[i].szName << " (val==null)" << std::endl;
        }
    }
  }

  stream.flush();
  stream.close();
#endif

  ID = 0;
  while( g_IOs->WhileBase( ID, -1, &pBase, &pszName, &nFcs ) )
  {
    switch( pBase->ID_CLASS )
    {
    case W_SMDO::TypeID:
      {
        W_SMDO *W = (W_SMDO*)pBase;
        W->OP = W->OPFL;
      }
      break;
    case W_SMDOCOM::TypeID:
      {
        W_SMDOCOM *W = (W_SMDOCOM*)pBase;
        W->PV = W->PVFL;
      }
      break;
    default:
      continue;
    }
  }
  return 0;
}

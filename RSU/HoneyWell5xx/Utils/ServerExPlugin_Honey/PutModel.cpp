#include "ServerExPlugin_Honey.h"
#include <DataTypes_.h>
#include <ShData.h>
#include <Names.h>
#include <eventstring.h>
#include <Common.h>
#include "EventStringH.h"
#include <RsuX.h>

extern "C" SERVEREXPLUGIN_HONEY5XX_API int SizeShRSU(DWORD type, KRsuX *pPnt)
{
  switch (type)
  {
  case CShHoneywellSystem::TypeID:
    return sizeof(CShHoneywellSystem);
  case CShOneMnemoHoney::TypeID:
    return sizeof(CShOneMnemoHoney);
  case CShOneAlmGrHoney::TypeID:
    return sizeof(CShOneAlmGrHoney);
#undef  HONEY_TYPE
#define HONEY_TYPE( a, b, c ) \
    case id_##b       : return sizeof(CSh##b);
    #include <HoneywellType.hpp>
  }
  return 0;
}

extern "C" SERVEREXPLUGIN_HONEY5XX_API int ExtractumRSU( DWORD type, CShBase* pSB, CBase *pB )
{
  pSB->m_nSize = SizeShRSU(type,NULL);
  if( !pSB->m_nSize )
    return 0;
  switch (type)
  {
  default:
    ASS(0);
    return 0;
  case CShHoneywellSystem::TypeID:
    {
      CShHoneywellSystem* sh = (CShHoneywellSystem*)pSB;
      CCBHSystem* hs = static_cast<CCBHSystem*>(sh);
      memcpy( hs, pB, pSB->m_nSize );
      pSB->Busy = 0;
    }
    break;
  case CShOneMnemoHoney::TypeID:
    {
      CShOneMnemoHoney* sh = (CShOneMnemoHoney*)pSB;
      COneMnemoHoney* hs = static_cast<COneMnemoHoney*>(sh);
      memcpy( hs, pB, pSB->m_nSize );
      pSB->Busy = 0;
    }
    break;
  case CShOneAlmGrHoney::TypeID:
    {
      CShOneAlmGrHoney* sh = (CShOneAlmGrHoney*)pSB;
      COneAlmGrHoney* hs = static_cast<COneAlmGrHoney*>(sh);
      memcpy( hs, pB, pSB->m_nSize );
      pSB->Busy = 0;
    }
    break;
#undef  HONEY_TYPE
#define HONEY_TYPE( a, b, c ) \
  case id_##b: \
    { \
    CSh##b* sh = (CSh##b*)pSB;\
    W_##b  * hs = static_cast<W_##b*>(sh);\
    memcpy( hs, pB, pSB->m_nSize );\
    pSB->Busy = 0;\
    }break;
    #include <HoneywellType.hpp>
  }
  pSB->RetCode = CShBase::rcOK;
  return pSB->m_nSize;
}
/*
class KRsuInt : public KRsuX
{
public:
  operator LPCSTR()
  {
    return (LPCSTR)m_PointInfo.pMem;
  }
  int GetType()
  {
    return m_PointInfo.type;
  }
};

extern "C" SERVEREXPLUGIN_HONEY5XX_API int GetRsuData( DWORD type, CShBase* pSB, KRsuX &pnt )
{
  if( !pnt.IsConnected() || type!=id_Intouch )
    return CShBase::rcNO;

  CShIntouchV* This = (CShIntouchV*)pSB;
  ASS( This->m_nSize == sizeof(*This) );

  KRsuInt* rr = (KRsuInt*)&pnt;

  nRSUx::EParamType typeMem = (nRSUx::EParamType)rr->GetType();
  This->valType = typeMem;
  switch( typeMem )
  {
  case e_BOOL:
  case e_int:
    This->integer = pnt;
    break;
  case e_double:
    This->real = pnt;
    break;
  case e_char:
    _snprintf_s( This->text, _countof(This->text), _TRUNCATE, "%s", (LPCSTR)*rr);
    break;
  default:
    ASS(0);
    This->valType = nRSUx::e_unknown;
    return CShBase::rcNO;
  }

  This->RetCode =  CShBase::rcOK;

  return This->RetCode;
}
*/
extern "C" SERVEREXPLUGIN_HONEY5XX_API DWORD PutModelRSU( LPCSTR szName, SSendToModel &send, CEventString &str )
{
  DWORD dwMess = CEventLogType::etUnknown;

  if ( !::IsAcy(send.eData) && send.eData!=(EDataTypes)id_OneMnemoHoney && send.eData!=(EDataTypes)id_OneAlmGrHoney)
    return dwMess;

  KEventString *strY = (KEventString*)&str;
  CBase* pBase = FindConnPoint( send.szName, send.eData, -1 );

  if( pBase != NULL )
  {
    if( strY->Put( send, pBase ) )
    {
      CAlarmBase *pH = (CAlarmBase*)pBase;
      pH->cfa = true;
      dwMess = send.eData;
    }
  }
  else
  {
    static char* szFiltr = "Не найден объект '%s'.'%s'";
    str.Event( szFiltr, send.szName, send.szValue );
  }

  return dwMess;
}

#include "ServerExSharedPlugin_Honey.h"
#include <DataTypes_.h>
#include <ShData.h>
#include <eventstring.h>
#include <LocalUtils.h>
#include "EventStringH.h"
#include <RsuX.h>

#include <SmDataTypes_.h>
#include <SmShData.h>

#include <QbDataTypes_.h>
#include <QbShData.h>

#include <eventstring.h>
#include <Common.h>
#include "EventStringH.h"

extern "C" SERVEREXSHAREDPLUGINSHARED_HONEY5XX_API int SizeShRSU(DWORD type, KRsuX *pPnt)
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
#undef  SM_TYPE
#define SM_TYPE( a, b, c ) \
  case id_##b       : return sizeof(CSh##b);
#include <SafetyManagerType.hpp>
#undef  QB_TYPE
#define QB_TYPE( a, b, c ) \
  case id_##b       : return sizeof(CSh##b);
#include <QuickBuilderType.hpp>
  }
  return 0;
}

extern "C" SERVEREXSHAREDPLUGINSHARED_HONEY5XX_API int ExtractumRSU( DWORD type, CShBase* pSB, CBase *pB )
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
#undef  SM_TYPE
#define SM_TYPE( a, b, c ) \
  case id_##b: \
    { \
    CSh##b* sh = (CSh##b*)pSB;\
    W_##b  * hs = static_cast<W_##b*>(sh);\
    memcpy( hs, pB, pSB->m_nSize );\
    pSB->Busy = 0;\
    }break;
#include <SafetyManagerType.hpp>
#undef  QB_TYPE
#define QB_TYPE( a, b, c ) \
  case id_##b: \
    { \
    CSh##b* sh = (CSh##b*)pSB;\
    W_##b  * hs = static_cast<W_##b*>(sh);\
    memcpy( hs, pB, pSB->m_nSize );\
    pSB->Busy = 0;\
    }break;
#include <QuickBuilderType.hpp>
  }
  pSB->RetCode = CShBase::rcOK;
  return pSB->m_nSize;
}

extern "C" SERVEREXSHAREDPLUGINSHARED_HONEY5XX_API DWORD PutModelRSU( LPCSTR szName, SSendToModel &send, CEventString &str )
{  
  DWORD dwMess = CEventLogType::etUnknown;

  if( ::IsAcy(send.eData) || send.eData==(EDataTypes)id_OneMnemoHoney || send.eData==(EDataTypes)id_OneAlmGrHoney )
  {
    KEventString *strY = (KEventString*)&str;
    CBase* pBase = _FindConnPoint( send.szName, send.eData, -1 );
    if( pBase != NULL )
    {
      switch( strY->Put( send, pBase ) )
      {
      case 1:
        {
          CAlarmBase *pH = (CAlarmBase*)pBase;
          pH->changedFromArm[0] = true;
          if( W_DEVCTL::TypeID==pBase->ID_CLASS )
          {
            if( !strcmp(send.szValue,"GOP") )
            {
              strcpy_s( send.szValue, "OP");
              send.bMessage = false;
              strY->Put( send, pBase );
            }
            if( !strcmp(send.szValue,"PV") )
            {
              strcpy_s( send.szValue, "OP");
              send.bMessage = false;
              strY->Put( send, pBase );
            }
          }
          else if( W_FLAG::TypeID==pBase->ID_CLASS )
          {
            if( !strcmp(send.szValue,"PV") && enumValueStr==send.eType )
            {
              W_FLAG *pFlag = (W_FLAG*)pBase;
              for( BYTE i=0; i<_countof(pFlag->STATETEXT); ++i )
              {
                if( !pFlag->STATETEXT[i][0] )
                  continue;
                if( _strcmpi( send.sNew, pFlag->STATETEXT[i] ) )
                  continue;
                strcpy_s( send.szValue, "PVFL");
                send.bMessage = false;
                send.eType = enumValueChr;
                send.cNew = i;
                strY->Put( send, pBase );
                //pFlag->PVFL = i;
                break;
              }
            }
          }
        }
      case -1:
        dwMess = send.eData;
        break;
      }
    }
    else
    {
      static char* szFiltr = "Не найден объект '%s'.'%s'";
      str.Event( szFiltr, send.szName, send.szValue );
    }
  }
  else if( ::IsSMAcy(send.eData) )
  {
    KEventString *strY = (KEventString*)&str;
    CBase* pBase = _FindConnPoint( send.szName, send.eData, -1 );
    if( pBase != NULL )
    {
      switch( strY->Put( send, pBase ) )
      {
      case 1:
        {
          if( W_SMDI::TypeID==pBase->ID_CLASS || W_SMDICOM::TypeID==pBase->ID_CLASS )
          {
            if( !strcmp(send.szValue,"PV") )
            {
              strcpy_s( send.szValue, "PVFL");
              send.bMessage = false;
              send.eType = enumValueBol;
              strY->Put( send, pBase );
            }
          }
        }
      case -1:
        dwMess = send.eData;
        break;
      }
    }
    else
    {
      static char* szFiltr = "Не найден объект '%s'.'%s'";
      str.Event( szFiltr, send.szName, send.szValue );
    }
  }
  else if( ::IsQBAcy(send.eData) )
  {
    KEventString *strY = (KEventString*)&str;
    CBase* pBase = _FindConnPoint( send.szName, send.eData, -1 );
    if( pBase != NULL )
    {
      switch( strY->Put( send, pBase ) )
      {
      case 1:
        {
          CAlarmBase *pH = (CAlarmBase*)pBase;
          pH->changedFromArm[0] = send.szValue[0];
          pH->changedFromArm[1] = send.szValue[1];
        }
      case -1:
        dwMess = send.eData;
        break;
      }
    }
    else
    {
      static char* szFiltr = "Не найден объект '%s'.'%s'";
      str.Event( szFiltr, send.szName, send.szValue );
    }
  }

  return dwMess;
}

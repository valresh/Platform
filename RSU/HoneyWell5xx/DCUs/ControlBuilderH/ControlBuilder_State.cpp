#include <rsuErr.h>
#include "ControlBuilder.h"
#include "stateKeys.h"
#include "../../HoneyWell/Include/StateSer.h"
#include <chrono>
#include <crossstring.h>

static const DWORD s_CurVersion = 0x00000011;

DWORD KControlBuilder::GetLastVersion()
{
  return s_CurVersion;
}

int KControlBuilder::SaveState( IStateSer *psaver )
{
  OutputDebugString( "KControlBuilder::SaveState...\n" );
  auto t = std::chrono::high_resolution_clock::now();

  if( m_entryModule )
  {
    SaveState( psaver, m_entryModule );
    psaver->Write( kKeyEnd );
  }


  auto durationms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t).count();
  char szDebug[64*4];
  sprintf_s( szDebug, "время сохранения состояния ControlBuilder %lld ms\n", durationms);
  OutputDebugString( szDebug );

  return 0;
}

void KControlBuilder::SaveState( IStateSer *psaver, SH_Module *pM )
{
  SaveStateVars( psaver, pM );
  for( size_t i=0; i < pM->nSubModules; ++i )
  {
    SaveState( psaver, pM->ppSubModules[i] );
  }

  for( size_t i=0; i < pM->nBlocks; ++i )
  {
    SaveStateVars( psaver, pM->ppBlocks[i] );
  }
}

void KControlBuilder::SaveStateVars( IStateSer *psaver, KBmBase *pB )
{
  psaver->Write( kKeyBlockBegin );
  psaver->Write( pB->BlockName );
  psaver->Write( pB->TypeName );
  psaver->Write( pB->H_Type );
  LPCSTR pField = NULL;
  LPBYTE pVarAdd = NULL;
  eVarType type = evtHZ;
  USHORT stringVarSize = 0;
  int i = pB->GetFirstVar( SVarInfo::efVar ); 
  while( (pField = pB->GetVar( i, pVarAdd, type, &stringVarSize )) && pField )
  {
    psaver->Write( kKeyField );
    psaver->Write( pField );
    psaver->Write( type );
    switch( type )
    {
    case evtBool:
    case evtByte:
      psaver->Write( *(BYTE*)pVarAdd );
      break;
    case evtInt:
      psaver->Write( *(int*)pVarAdd );
      break;
    case evtUShort:
      psaver->Write( *(USHORT*)pVarAdd );
      break;
    case evtDouble:
      psaver->Write( *(double*)pVarAdd );
      break;
    case evtTime:
      psaver->Write( *(time_t*)pVarAdd );
      break;
    case evtString:
      psaver->Write( (LPCSTR)pVarAdd );
      break;
    case evtFloat:
      psaver->Write( *(float*)pVarAdd );
      break;
    case evtUInt64:
      psaver->Write( *(UINT64*)pVarAdd );
      break;
    default:
      ASS(0);
    }
  }
}

int KControlBuilder::RestoreState( IStateSer *psaver )
{
  OutputDebugString( "KControlBuilder::RestoreState...\n" );
  auto t = std::chrono::high_resolution_clock::now();


  if( !m_entryModule )
    return 3;

  char Name[64*4];
  char TypeName[64*4];
  DWORD H_Type;

  BYTE skiparray[1024*4] = { 0 };

  KBmBase* pBlk = NULL;
  for( int i=0; 1; ++i )
  {
    EKeys key = eKeyNULL;
    psaver->Read( key );
    if( kKeyBlockBegin==key )
    {
      pBlk = NULL;
      psaver->Read( Name );
      psaver->Read( TypeName );
      if (psaver->NeedConvertUTF())
      {
          cross::string::convert_to_utf8(1251, Name, Name, sizeof(Name));
          cross::string::convert_to_utf8(1251, TypeName, TypeName, sizeof(TypeName));
      }
      psaver->Read( H_Type );
      pBlk = m_entryModule->QuickFindObj( Name );
      if( !pBlk )
        continue;
      if( strcmp( TypeName, pBlk->TypeName) )
      {
        pBlk = NULL;
        continue;
      }
      ASS( H_Type==pBlk->H_Type );
    }
    else if( kKeyField==key )
    {
      eVarType type = evtHZ, realType = evtHZ;
      USHORT stringSize = 0;
      psaver->Read( Name );
      if (psaver->NeedConvertUTF())
        cross::string::convert_to_utf8(1251, Name, Name, sizeof(Name));
      psaver->Read( type );
      LPBYTE pVarAdd = NULL;
      if( pBlk )
      {
        BYTE flags = 0;
        bool b = pBlk->KBmBase::GetVar( Name, &pVarAdd, &realType, &stringSize, NULL, &flags );
        ASS( b );
        if( !b )
          return -4;
        ASS( realType==type );
        if( flags&SVarInfo::efParam)
          pVarAdd = skiparray;
      }
      else
        pVarAdd = skiparray;
      switch( type )
      {
      case evtBool:
      case evtByte:
        psaver->Read( *(BYTE*)pVarAdd );
        break;
      case evtInt:
        psaver->Read( *(int*)pVarAdd );
        break;
      case evtUShort:
        psaver->Read( *(USHORT*)pVarAdd );
        break;
      case evtDouble:
        psaver->Read( *(double*)pVarAdd );
        break;
      case evtTime:
        psaver->Read( *(time_t*)pVarAdd );
        break;
      case evtString:
      {
          USHORT rdSize = sizeof(skiparray);
          //USHORT rdSizeNew = rdSize;
          if (pBlk)
          {
              rdSize = stringSize + 1;
              //rdSizeNew = rdSize;
              //if (psaver->NeedConvertUTF() && (!strcmp(Name, "PV") || !strcmp(Name, "STATETEXT")))
              //    rdSize = stringSize/4 + 1;;
          }
          psaver->Read((LPSTR)pVarAdd, rdSize);
          if (psaver->NeedConvertUTF())
              cross::string::convert_to_utf8(1251, (LPSTR)pVarAdd, (LPSTR)pVarAdd, std::min<int>(rdSize,countof(skiparray)));
      }
        break;
      case evtFloat:
        psaver->Read( *(float*)pVarAdd );
        break;
      case evtUInt64:
        psaver->Read( *(UINT64*)pVarAdd );
        break;
      default:
        ASS(0);
      }
    }
    else if( kKeyEnd==key )
      break;
    else
    {
      ASS(0);
      return -5;
    }
  }

  auto durationms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t).count();
  char szDebug[64*4];
  sprintf_s( szDebug, "время восстановления состояния ControlBuilder %lld ms\n", durationms);
  OutputDebugString( szDebug );

  return 0;
}

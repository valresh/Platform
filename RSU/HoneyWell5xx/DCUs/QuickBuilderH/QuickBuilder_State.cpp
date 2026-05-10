#include "QuickBuilder.h"
#include "stateKeys.h"
//#include <HabConnect.h>
#include <rsuErr.h>
#include <crossstring.h>

const _W::verType_ _W::s_CurVersion = 0x00000011;

DWORD KQuickBuilder::GetLastVersion()
{
  return s_CurVersion;
}

int KQuickBuilder::SaveState( IStateSer *pSaver )
{
  for( DWORD n=0; n<m_nPoints; ++n )
  {
    KQbBase *pnt = m_ppPoints[n];
    pSaver->Write( kKeyPointBegin );
    pSaver->Write( pnt->PointName.c_str() );
    int nEnum = 0;
    eVarType type = evtHZ;
    BYTE *pVarAdd = NULL;
    LPCSTR pszName = NULL;
    tVarSizeType_ size = 0;
    while( pVarAdd = pnt->EnumSavebleVars( nEnum, &pszName, type, size ) )
    {
      pSaver->Write( kKeyField );
      pSaver->Write( pszName );
      pSaver->Write( type );
      pSaver->Write( size );
      switch( type )
      {
      case evtByte:
        pSaver->Write( *(BYTE*)pVarAdd );
        break;
      case evtInt4:
      case evtInt:
        pSaver->Write( *(int*)pVarAdd );
        break;
      case evtInt2:
        pSaver->Write( *(short*)pVarAdd );
        break;
      case evtUShort:
        pSaver->Write( *(USHORT*)pVarAdd );
        break;
      case evtDouble:
        pSaver->Write( *(double*)pVarAdd );
        break;
      case evtString:
        pSaver->Write( (LPCSTR)pVarAdd );
        break;
      case evtFloat:
          pSaver->Write(*(float*)pVarAdd );
          break;
      default:
        ASS(0);
      }
    }
    pSaver->Write( kKeyPointEnd );
  }

  pSaver->Write( kKeyEnd );
  return 0;
}

int KQuickBuilder::RestoreState( IStateSer *prestorer )
{
  EKeys key = eKeyNULL;
  bool bContinue = true;
  KQbBase *pnt = NULL;
  char buffer[128*4];
  eVarType type = evtHZ;
  tVarSizeType_ size = 0;

  BYTE *pMem = NULL;
  eVarType typeM = evtHZ;
  tVarSizeType_ sizeM = 0;

  while( bContinue && prestorer->Read( key ) )
  {
    switch( key )
    {
    case kKeyPointBegin:
      prestorer->Read( buffer );
      if (prestorer->NeedConvertUTF())
          cross::string::convert_to_utf8(1251, buffer, buffer, sizeof(buffer));
      //if( IsReadState(buffer) )
      {
        pnt = Find( buffer );
        ASSD( pnt );
      }
      break;
    case kKeyField:
      prestorer->Read( buffer );
      if (prestorer->NeedConvertUTF())
          cross::string::convert_to_utf8(1251, buffer, buffer, sizeof(buffer));
      prestorer->Read( type );
      prestorer->Read( size );
      pMem = NULL;
      if( pnt )
      {
        if( pnt->GetVar( buffer, &pMem, &typeM, &sizeM ) )
        {
            if (prestorer->NeedConvertUTF() && type == evtFloat && size == 0)
                continue;
          ASSD( typeM==type );
          ASSD(sizeM == size || evtString == type);
          if( typeM!=type || (evtString != type && sizeM!=size) )
            pMem = NULL;
        }
      }
      if( evtString==typeM || evtString==type )
      {
        prestorer->Read( buffer );
        if (prestorer->NeedConvertUTF())
            cross::string::convert_to_utf8(1251, buffer, buffer, sizeof(buffer));
        if( pMem )
          strcpy_s( (LPSTR)pMem, sizeM, buffer );
      }
      else
      {
        if( pMem )
          prestorer->Read( size, pMem );
        else
        {
		  ASS( size <= _countof(buffer) );
          prestorer->Read( size, buffer );
		}
      }
      break;
    case kKeyPointEnd:
      pnt = NULL;
      break;
    case kKeyEnd:
      bContinue = false;
      break;
    default:
      ASS(0);
      return -1;
    }
  }

  return 0;
}

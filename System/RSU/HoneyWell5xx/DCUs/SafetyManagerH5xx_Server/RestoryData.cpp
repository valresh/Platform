#include "SafetyManager.h"
#include <macros/AutoCloser.h>
#include "Get2Buffer.h"
#include <MemoryServiceSM5xx.h>
#include <SafetyManagerStruct.h>

extern int g_chVer;

KSafetyManager::e_RestResult KSafetyManager::RestoryData( LPCSTR pszPrj )
{
  char Path[_MAX_PATH]={}, szBin[_MAX_PATH]={};

  {
    GetDatFilePath( Path, pszPrj );
    std::ifstream hFile(Path, std::ios::binary);
    if (!hFile.is_open() )
      return err_CantOpen;
    DWORD Lr;
    int testVer = 0;
    hFile.read((char*) & testVer, sizeof(testVer));
    if( testVer!=g_chVer )
      return err_WrongVer;
    hFile.read( (char*)&m_kFSC_Info, sizeof(m_kFSC_Info));
    hFile.read( (char*)&m_Ready, sizeof(m_Ready) );
    //  ASS(Ready)
    ASS(hFile.read((char*)m_FSC_Info, m_kFSC_Info * sizeof( CFSC_Info )));
  }

  GetPathToSmFile( Path, pszPrj, pszPrj, ".*" );
  GetInMemFilePath( szBin, pszPrj, "fsc", "Bin" );

  bool bLoad = true;
  DWORD dwSize = 0;
  //KKKK
#ifndef _DEBUG1
  KAutoCloser<char*> szBuffer( (char*)Get2Buffer( szBin, dwSize ), ClearBuffer );
  if( szBuffer )
  {
    char* ptr = szBuffer;
    if( *(int*)ptr == g_chVer )//Версия
    {
      ptr += sizeof(g_chVer);
      FILETIME* F = (FILETIME*)ptr;
      ptr += sizeof(FILETIME);
      mFile.Read( ptr );
      mList.Read( ptr );
      mBuff.Read( ptr );
      mNext.Read( ptr );
      mDots.Read( ptr );
      bLoad = false;
    }
  }
#endif

  if( bLoad )
  {
    e_RestResult r = Load( pszPrj );
    ASSD( err_Ok==r );

    char szBin[_MAX_PATH];
    GetInMemFilePath( szBin, pszPrj, "fsc", "Bin" );
    std::ofstream hFile(szBin, std::ios::out | std::ios::binary | std::ios::trunc);
    //HFILE hFile = _lcreat( szBin, 0 );
    if ( hFile.is_open() )
    {
      hFile.write((char*)&g_chVer, sizeof(g_chVer) );
      FILETIME tmp;
      ZeroMemory( &tmp, sizeof(tmp) );
      hFile.write( (char*)&tmp, sizeof(tmp) );
      mFile.Write( hFile );
      mList.Write( hFile );
      mBuff.Write( hFile );
      mNext.Write( hFile );
      mDots.Write( hFile );
      hFile.close();
    }
  }
  {
    for( UINT n=0, e(mFile.Count()); n<e; ++n )
    {
      LFscFile &obj = mFile.Item(n);
      if( !obj.szTagName[0] )
        continue;
      W_SMFLD *pObj = (W_SMFLD*)KMemoryServiceSM5xx::Instance().CreateIOsStruct( obj.szTagName, NULL, W_SMFLD::TypeID, 0 );
      if( !pObj )
        continue;
      strcpy_s( pObj->DESC, obj.szTitle );
      for( int i=0; i<m_kFSC_Info; ++i )
      {
        CFSC_Info &info = m_FSC_Info[i];
        if( info.nFLDNumber!=obj.nNumber )
          continue;
        strcpy_s( pObj->POINTDETAILDISPLAY, info.szPointDetailDisplay );
        strcpy_s( pObj->GROUPDETAILDISPLAY, info.szGroupDetailDisplay );
        strcpy_s( pObj->ASSOCIATEDDISPLAY, info.szAssociatedDisplay );
        break;
      }
    }
  }

  bool bCreatedShared[_countof(m_FSC_Info)] = {};
  const UINT nCount = mList.Count();
  for ( UINT n = 0; n < nCount; n++ )
  {
    SFscBase* obj = mList.Item(n);
    if ( obj->mT == 0x0B ) //"Digital Input"
    {
      SPointIn* dat = PointIn(obj->nBuff);
      ASSD( dat->nDbf < _countof(bCreatedShared) && dat->nDbf >=0 );
      if( !(dat->nDbf < _countof(bCreatedShared) && dat->nDbf >=0) )
        continue;
      bCreatedShared[dat->nDbf] = true;
      if( LFscBase::COM==obj->mB )
        dat->In = CreateSharedIO( dat->nDbf, enumValueBol, obj->mT, obj->mB );
      else if( LFscBase::FLD==obj->mB )
      {
        dat->In = CreateSharedIO( dat->nDbf, enumValueBol, obj->mT, obj->mB );
      }
      else if( LFscBase::unknown==obj->mB )
      {}
      else
      {
        ASSD(0);
      }
    }
    else if ( obj->mT == 0x0C ) //"Analog Input (AI)"
    {
      SPointIn* dat = PointIn(obj->nBuff);
      ASSD( dat->nDbf < _countof(bCreatedShared) && dat->nDbf >=0 );
      bCreatedShared[dat->nDbf] = true;
      if( LFscBase::COM==obj->mB )
        dat->In = CreateSharedIO( dat->nDbf, enumValueDbl, obj->mT, obj->mB );
      else if( LFscBase::FLD==obj->mB )
        dat->In = CreateSharedIO( dat->nDbf, enumValueDbl, obj->mT, obj->mB );
      else if( LFscBase::SYS==obj->mB )
        dat->In = CreateSharedIO( dat->nDbf, enumValueDbl, obj->mT, obj->mB );
      else
      {
        ASSD(0);
        KKK();
      }
    }
    else if ( obj->mT == 0x1A ) //"Digital Output"
    {
      SPointOut* dat = PointOut(obj->nBuff);
      ASSD( dat->nDbf < _countof(bCreatedShared) && dat->nDbf >=0 );
      bCreatedShared[dat->nDbf] = true;
      dat->Out = CreateSharedIO( dat->nDbf, enumValueBol, obj->mT, obj->mB );
    }
    else if ( obj->mT == 0x19 ) //"Analog Output"
    {
      SPointOut* dat = PointOut(obj->nBuff);
      ASSD( dat->nDbf < _countof(bCreatedShared) && dat->nDbf >=0 );
      bCreatedShared[dat->nDbf] = true;
      dat->Out = CreateSharedIO( dat->nDbf, enumValueDbl, obj->mT, obj->mB );
    }
    else if( 0x47==obj->mT ) //"Multiple Output (BO)"
    {
      SPointOut* dat = PointOut(obj->nBuff);
      ASSD( dat->nDbf < _countof(bCreatedShared) && dat->nDbf >=0 );
      bCreatedShared[dat->nDbf] = true;
      SDotValue* V = DotV(dat->nIn);
      dat->Out = CreateSharedIO( dat->nDbf, V->eType, obj->mT, obj->mB );
    }
    else if( 0x46==obj->mT ) //"Multiple Input (BI)"
    {
      SPointIn* dat = PointIn(obj->nBuff);
      ASSD( dat->nDbf < _countof(bCreatedShared) && dat->nDbf >=0 );
      bCreatedShared[dat->nDbf] = true;
      SDotValue* V = DotV(dat->nOut);
      dat->In = CreateSharedIO( dat->nDbf, V->eType, obj->mT, obj->mB );
    }
  }

  for( int i=0; i<m_kFSC_Info; ++i )
  {
    if( bCreatedShared[i] )
      continue;
    CFSC_Info &info = m_FSC_Info[i];
    if( info.nFLDNumber )
      continue;
    if( 'F'!=info.szTypeLocation[0])
      continue;
    switch( info.szPointType[0] )
    {
    default:
      continue;
    case 'D':
      CreateSharedIO( i, enumValueBol, 0, LFscBase::unknown );
      break;
    case 'A':
      CreateSharedIO( i, enumValueDbl, 0, LFscBase::unknown );
      break;
    }
    KKK();
  }

  return err_Ok;
}

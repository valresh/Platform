#include "FscFile.h"
#include <macros/AutoCloser.h>
#include "Get2Buffer.h"
#include <rsuProjectVersion.h>
#include <CommProc.h>

UINT SFscFile::AddFile( UINT nNumb, UINT nPar, int nFromFldCall )
{
  if( !Realloc(1) )
  {
    ASS(0);
    return -1;
  }
  //
  UINT n = AddCurr();
  LFscFile& obj = Item(n);
  obj.nNumber = nNumb;
  obj.nFromFldCall = nFromFldCall;
  // Иерархия файлов в списке SFscFile
  obj.nParent = nPar;
  obj.nChild = -1;
  obj.nNextC = -1;
  return n;
}

UINT SFscFile::LoadFile( IFscStorage* fsc, LPCSTR szFile, int nNumber, UINT nParent, SUniTemp& temp, int nFromFldCall )
{
  UINT nItem = AddFile( nNumber, nParent, nFromFldCall );
  if( nItem == -1 )
    return nItem;

  DWORD dwSize;
  KAutoCloser<SfscAbstruct*> szBuff( (SfscAbstruct*)Get2Buffer( szFile, dwSize ), ClearBuffer );
  if( NULL==szBuff )
    return -1;
  DWORD count = 0, size = 0;
  if( 0==(dwSize % sizeof(ns_ver137::Sfsc)) && 0==(dwSize % sizeof(ns_ver153::Sfsc)) )
  {
    if( 0==SfscAbstruct::sizeData )
    {
      if( ns_rsu::eVersion::VSB_KF==rsuProjectVersion() )
      {
        SfscAbstruct::sizeData = sizeof( ((ns_ver153::Sfsc*)(SfscAbstruct*)szBuff)->data );
      }
      else
      {
        ASSD(0)
      }
    }
  }
  if( 0==SfscAbstruct::sizeData )
  {
    if( 0==(dwSize % sizeof(ns_ver137::Sfsc)) )
    {
      size = sizeof(ns_ver137::Sfsc);
      count = dwSize / size;
      SfscAbstruct::sizeData = sizeof( ((ns_ver137::Sfsc*)(SfscAbstruct*)szBuff)->data );
      ASSD( dwSize % sizeof(ns_ver153::Sfsc) );
    }
    else if( 0==(dwSize % sizeof(ns_ver153::Sfsc)) )
    {
      size = sizeof(ns_ver153::Sfsc);
      count = dwSize / size;
      SfscAbstruct::sizeData = sizeof( ((ns_ver153::Sfsc*)(SfscAbstruct*)szBuff)->data );
      ASSD( dwSize % sizeof(ns_ver137::Sfsc) );
    }
    else
    {
      ASS(0);
      return -1;
    }
  }
  else
  {
    if( SfscAbstruct::sizeData == sizeof( ((ns_ver137::Sfsc*)(SfscAbstruct*)szBuff)->data ) )
    {
      size = sizeof(ns_ver137::Sfsc);
      count = dwSize / size;
      ASS( 0==(dwSize % sizeof(ns_ver137::Sfsc)) );
    }
    else if( SfscAbstruct::sizeData == sizeof( ((ns_ver153::Sfsc*)(SfscAbstruct*)szBuff)->data ) )
    {
      size = sizeof(ns_ver153::Sfsc);
      count = dwSize / size;
      ASS( 0==(dwSize % sizeof(ns_ver153::Sfsc)) );
    }
    else
    {
      ASS(0);
    }
  }
  
  LFscFile& item = Item(nItem);
  item.nPlace = mList.Count();
  int nError = mList.Load( fsc, szBuff, size, count, item.szDoc,temp, NULL,0, item.nNumber );
  item.nCount = mList.Count()-item.nPlace;

  if ( nError != 0 )
    return -1;

  // Проверяем наличие процедур
  UINT nNext  = -1;
  const UINT nCount = mList.Count();
  for ( UINT n = item.nPlace; n < nCount; n++ )
  {
    SFscBase* base = mList.Item(n);
    base->nFromFldCall = nFromFldCall;
    if ( base->mT == 0x41 )
    {
      SProcedure* dat = (SProcedure*)fsc->Data(base->nBuff);
      char ext[16];
      fs::path path(szFile);
      sprintf_s( ext, "%04d", dat->nProc );
      path.replace_extension(fs::path(ext));
      //_makepath_s( szNfile, drive, dir, prjName, ext );
      UINT nReturn = LoadFile( fsc, path.string().c_str(), dat->nProc, nItem, temp, nNumber);
      if ( nReturn == -1 ) 
        return -1;
      // Связываем объект и файл
      base = mList.Item(n);
      dat = (SProcedure*)fsc->Data(base->nBuff);
      dat->nPlace = nReturn;
      //
      LFscFile& item = Item(nItem);
      if ( item.nChild == -1 )
      {
        item.nChild = nReturn;
      }
      else
      {
        LFscFile& next = Item(nNext);
        next.nNextC = nReturn;
      }
      nNext = nReturn;
    }
    else if( 0x1C==base->mT )
    {
      SEquationBlock* dat = (SEquationBlock*)fsc->Data(base->nBuff);
      char ext[16];
      fs::path path(szFile);
      sprintf_s(ext, "%04d", dat->nProc);
      path.replace_extension(fs::path(ext));
      UINT nReturn = LoadFile( fsc, path.string().c_str(), dat->nProc, nItem, temp, nNumber);
      if ( nReturn == -1 ) 
        return -1;
      // Связываем объект и файл
      base = mList.Item(n);
      dat = (SEquationBlock*)fsc->Data(base->nBuff);
      dat->nPlace = nReturn;
      //
      LFscFile& item = Item(nItem);
      if ( item.nChild == -1 )
      {
        item.nChild = nReturn;
      }
      else
      {
        LFscFile& next = Item(nNext);
        next.nNextC = nReturn;
      }
      nNext = nReturn;
    }
  }

  return nItem;
}

#ifdef _WIN32
void SFscFile::GetACSBounds (int nItem, RECT *pRect)
{
	LFscFile& item = Item(nItem);
	UINT nLast = item.nPlace+item.nCount;
	for (UINT n = item.nPlace; n < nLast; ++n)
	{
		SFscBase *obj = mList.Item(n);
		pRect->left = min (pRect->left, obj->rect.left);
		pRect->top = min (pRect->top, obj->rect.top);
		pRect->right = std::max<LONG> (pRect->right, obj->rect.right);
		pRect->bottom = std::max<LONG> (pRect->bottom, obj->rect.bottom);
	}
}

void SFscFile::Draw( IFscStorage* fsc, UINT nItem, CMyFont& font, UINT nSelect, bool bNumber )
{
  if( nItem>= Count() )
    return;

  LFscFile& item = Item(nItem);
  UINT nLast = item.nPlace+item.nCount;
  // Из-за линий рисуем в два захода
  for( UINT n = item.nPlace; n < nLast; ++n )
  {
    mList.Draw1( fsc, n, font, nSelect );
  }
  for( UINT n = item.nPlace; n < nLast; ++n )
  {
    mList.Draw2( fsc, n, font, nSelect );
  }

  if( !bNumber )
    return;

  static HFONT hFont = NULL;
  if( hFont == NULL )
  {
    LOGFONT lf;
    memset( &lf, 0, sizeof(lf) );
    lf.lfCharSet = DEFAULT_CHARSET;
    lf.lfHeight  = -18;
    lf.lfWeight  = 900;
    lstrcpy( lf.lfFaceName, "Courier New" );
    hFont = ::CreateFontIndirect(&lf);
  }
  CMyFont fnt( font, hFont, 0x0080ff );
  char txt[64];
  for ( UINT n = item.nPlace; n < nLast; ++n)
  {
    SFscBase* obj = mList.Item(n);
    if( 0x3A <= obj->mT && obj->mT <= 0x3D )
      continue;
    sprintf_s( txt, "%d", n-item.nPlace );
    RECT rc = obj->rect;
    ::InflateRect( &rc, 20, 20 );
    font.Draw( rc, txt );
  }
}

SFscBase* SFscFile::PtInRect( UINT nItem, POINT pt )
{
  if( nItem>= Count() )
    return NULL;

  LFscFile& item = Item(nItem);
  UINT nCount = item.nPlace+item.nCount;
  for( UINT n = item.nPlace; n < nCount; n++)
  {
    SFscBase* obj = mList.Item(n);
    if ( ::PtInRect( obj->rect.ToRect(), pt))
      return obj;
  }

  return NULL;
}
#endif
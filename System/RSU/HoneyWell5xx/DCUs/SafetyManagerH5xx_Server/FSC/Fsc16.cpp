#include "FscList.h"
static char* sid = "Аналоговый переход";
// 
bool SFscList::FuncParser16( VAR_PARSER )
{
  FuncParserdo(fsc,p,dat,tmp);
  //
  SGoto io;
  FillGoto( io, dat );
  p.nBuff = fsc->AddFscBuff( &io, sizeof(io) );
  return true;
}
//
//void FuncDebugs16( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints16( VAR_PAINTS )
{
  SGoto* dat = (SGoto*)fsc->Data(p.nBuff);
  //
  RECT rcl = p.rect; rcl.right = rcl.left+(rcl.bottom-rcl.top);
  RECT rcr = p.rect; rcr.left  = rcl.right+10;
  //
  COLORREF color = dat->nData == -1 ? BADCOLOR:((dat->nData == nSelect) ? SELCOLOR :BOXCOLOR);
  CMyPen pen( font, color );
  CMyBrush brush( font, 0x010203, true );
  ::InflateRect( &rcl, 1, 1 );
  pen.Ellipse( rcl );
  p.DrawL( font, rcr, "Переход", 0, 2 );
  //
  char txt[64*4];
  //
  sprintf_s( txt, 16, "%d", dat->nSrc );
  p.DrawC( font, rcl, txt, 0, 2 );
  //
  sprintf_s( txt, 16, "%d", dat->nDst );
  p.DrawC( font, rcl, txt, 1, 2 );
  //
  rcl.right -= 2;
  sprintf_s( txt, 16, "%d", dat->nIndex );
  p.DrawL( font, txt, 2, &rcl );
}
#endif
//
void SFscList::FuncSetLnk16( VAR_SETLNK )
{
  // Вход только справа
  if ( x == p.rect.right ) return;
  SGoto* dat = (SGoto*)fsc->Data(p.nBuff);
  if ( dat->nData == -1 )
    dat->nData = nLink;
}
////-----------------------------------------------------------------------------
//UINT FuncCursor16( VAR_CURSOR )
//  {
//  SGoto* dat = (SGoto*)fsc->Data(p.nBuff);
//  return dat->nData;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput16( VAR_OUTPUT )
{
  n = 0;
  return NULL;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers16( VAR_TIMERS )
{
}
////-----------------------------------------------------------------------------
int SFscList::FuncQuickWatch16( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  return 0;
}

int SFscList::SaveState16( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}

int SFscList::RestoreState16( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}

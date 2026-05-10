#include "FscList.h"
static char* sid = "Дискретный переход (Мульти)";
//
bool SFscList::FuncParser18( VAR_PARSER )
{
  FuncParserdo(fsc,p,dat,tmp);
  //
  SGotoN io;
  FillGoto( io, dat );
  p.nBuff = fsc->AddFscBuff( &io, sizeof(io) );
  return true;
}
//
//void FuncDebugs18( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints18( VAR_PAINTS )
{
  SGotoN* dat = (SGotoN*)fsc->Data(p.nBuff);
  //
  RECT rcl = p.rect; rcl.right = rcl.left+(rcl.bottom-rcl.top);
  RECT rcr = p.rect; rcr.left  = rcl.right+10;
  //
  COLORREF color = dat->nData == -1 ? BADCOLOR:((dat->nData == nSelect) ? SELCOLOR :BOXCOLOR);
  CMyPen pen( font, color );
  CMyBrush brush( font, 0x010203, true );
  ::InflateRect( &rcl, 1, 1 );
  pen.Ellipse( rcl );
  //p.DrawL( font, rcr, "Комментарий"   , 0 );
  //p.DrawL( font, rcr, "Берёт от родителя", 1 );
  char txt[64*4] = "to ";
  for ( int n = 0; n < 5; n++ )
    {
    if ( !dat->nDst[n] )
      break;
    char a[16];
    sprintf_s( a, 16, "%d", dat->nDst[n] );
    strcat_s( txt, a );
    if ( n != 4 && dat->nDst[n+1] )
    strcat_s( txt, "," );
    }
  p.DrawL( font, rcr, txt, 2 );
  sprintf_s( txt, 16, "%d", dat->nSrc );
  font.Draw( rcl, txt );
  rcl.right -= 2;
  sprintf_s( txt, 16, "%d", dat->nIndex );
  p.DrawL( font, txt, 2, &rcl );
}
#endif
//
void SFscList::FuncSetLnk18( VAR_SETLNK )
{
  // Вход только справа
  if ( x == p.rect.right ) return;
  SGoto* dat = (SGoto*)fsc->Data(p.nBuff);
  if ( dat->nData == -1 )
    dat->nData = nLink;
}
////-----------------------------------------------------------------------------
//UINT FuncCursor18( VAR_CURSOR )
//  {
//  SGotoN* dat = (SGotoN*)fsc->Data(p.nBuff);
//  return dat->nData;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput18( VAR_OUTPUT )
{
  n = 0;
  return NULL;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers18( VAR_TIMERS )
{
}
////-----------------------------------------------------------------------------
int SFscList::FuncQuickWatch18( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  return 0;
}

int SFscList::SaveState18( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}

int SFscList::RestoreState18( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}

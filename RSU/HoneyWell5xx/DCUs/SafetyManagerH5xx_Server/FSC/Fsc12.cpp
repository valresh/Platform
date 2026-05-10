#include "FscList.h"
static char* sid = "R-S FLIP FLOP";
//
struct SFlipFlopRS
{
  UINT S;//
  UINT R;//
  UINT Q;// Выход
  UINT N;// NOT(Q)
};
//
bool SFscList::FuncParser12( VAR_PARSER )
{
  FuncParserdo(fsc,p,dat,tmp);
  //
  SFlipFlopRS io;
  // Выходные точки
  SDotValue Q(enumValueBol);
  SDotValue N(enumValueBol);
  io.Q = fsc->AddFscDots(&Q);
  io.N = fsc->AddFscDots(&N);
  // Входные точки
  io.S = -1;
  io.R = -1;
  // Заказываем память
  p.nBuff = fsc->AddFscBuff( &io, sizeof(io) );
  //
  ns_UT::SLocalTmp* item = tmp.Item();
  item->nCount = 2;
  item->dim[0] = io.Q;
  item->dim[1] = io.N;
  return true;
}
//
//void FuncDebugs12( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints12( VAR_PAINTS )
{
  SFlipFlopRS* dat = (SFlipFlopRS*)fsc->Data(p.nBuff);
  //
  CMyPen pen( font, (nSelect == dat->Q) ? SELCOLOR : BOXCOLOR );
  pen.Rect( p.rect );
  int h = (p.rect.top+p.rect.bottom)/2;
  pen.Horz( h, p.rect.left, p.rect.right );
  pen.Vert( p.rect.right-1, p.rect.bottom, h );
  pen.Vert( p.rect.right-2, p.rect.bottom, h );
  pen.Vert( p.rect.right-3, p.rect.bottom, h );
  //
  p.DrawL( font, p.rect, "S", 0 );
  p.DrawL( font, p.rect, "R", 2 );
  //
  if ( dat->S == -1 || dat->R == -1 )
  p.BadRect( font, -1 );
  if ( nSelect != -1 )
  {
    if ( dat->S == nSelect )
      p.Select( font, 0, 2, true );
    else if ( dat->R == nSelect )
      p.Select( font, 1, 2, true );
  }
}
#endif
//
void SFscList::FuncSetLnk12( VAR_SETLNK )
{
  // Вход только справа
  if ( x == p.rect.right ) 
    return;
  SFlipFlopRS* dat = (SFlipFlopRS*)fsc->Data(p.nBuff);
  int yy = (p.rect.top+p.rect.bottom)/2;
  if ( y < yy )
    dat->S = nLink;
  else
    dat->R = nLink;
}
////-----------------------------------------------------------------------------
//UINT FuncCursor12( VAR_CURSOR )
//  {
//  SFlipFlopRS* dat = (SFlipFlopRS*)fsc->Data(p.nBuff);
//  int x = (p.rect.left+p.rect.right)/2;
//  int y = (p.rect.top+p.rect.bottom)/2;
//  return ( pt.x < x ) ? ((pt.y < y) ? dat->S : dat->R) : dat->Q;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput12( VAR_OUTPUT )
{
  SFlipFlopRS* dat = (SFlipFlopRS*)fsc->Data(p.nBuff);
  n = 1;
  return &dat->Q;
}
////-----------------------------------------------------------------------------
//// S R | Q
//// 0 1 | 0
//// 1 0 | 1
//// 0 0 | Qt-1
//// 1 1 | 0
void SFscList::FuncTimers12( VAR_TIMERS )
{
  SFlipFlopRS* dat = (SFlipFlopRS*)fsc->Data(p.nBuff);
  //
  SDotValue* S = fsc->DotV(dat->S);
  SDotValue* R = fsc->DotV(dat->R);
  //
  bool s = S->Bol();
  bool r = R->Bol();
  if ( !s && !r )
    return;
  //
  SDotValue* Q = fsc->DotV(dat->Q);
  if ( !(Q->dwFlags & 1) )
    Q->Set( !r );
  //
  SDotValue* N = fsc->DotV(dat->N);
  if ( !(N->dwFlags & 1) )
    N->Set( r );
}
////-----------------------------------------------------------------------------
static LPCSTR s_pName[] = { "S", "R", "Q", "N" };
int SFscList::FuncQuickWatch12( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  SFlipFlopRS* dat = (SFlipFlopRS*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->S), fsc->DotV(dat->R), fsc->DotV(dat->Q), fsc->DotV(dat->N) };

  int n = 0;
  for( n; n<_countof(vals) && n<cVI; ++n )
  {
    pvi[n].val = vals[n];
    strcpy_s( pvi[n].szName, s_pName[n] );
  }
  return n;
}

int SFscList::SaveState12( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  SFlipFlopRS* dat = (SFlipFlopRS*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->S), fsc->DotV(dat->R), fsc->DotV(dat->Q), fsc->DotV(dat->N) };
  for( int i=0; i<_countof(vals); ++i )
  {
    psaver->WriteDotV( vals[i], s_pName[i] );
  }
  return 0;
}

int SFscList::RestoreState12( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  SFlipFlopRS* dat = (SFlipFlopRS*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->S), fsc->DotV(dat->R), fsc->DotV(dat->Q), fsc->DotV(dat->N) };
  for( int i=0; i<_countof(vals); ++i )
  {
    prest->ReadDotV( vals[i], s_pName[i] );
  }
  return 0;
}

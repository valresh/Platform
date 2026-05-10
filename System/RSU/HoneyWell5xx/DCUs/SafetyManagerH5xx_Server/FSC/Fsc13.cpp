#include "FscList.h"
static char* sid = "S-R FLIP FLOP";
//
struct SFlipFlopSR
{
  UINT S;//
  UINT R;//
  UINT Q;// Выход
  UINT N;// NOT(Q)
};
//
bool SFscList::FuncParser13( VAR_PARSER )
{
  FuncParserdo(fsc,p,dat,tmp);
  //
  SFlipFlopSR io;
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
//void FuncDebugs13( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints13( VAR_PAINTS )
{
  SFlipFlopSR* dat = (SFlipFlopSR*)fsc->Data(p.nBuff);
  //
  CMyPen pen( font, (nSelect == dat->Q) ? SELCOLOR : BOXCOLOR );
  pen.Rect( p.rect );
  int h = (p.rect.top+p.rect.bottom)/2;
  pen.Horz( h, p.rect.left, p.rect.right );
  pen.Vert( p.rect.right-1, p.rect.top, h );
  pen.Vert( p.rect.right-2, p.rect.top, h );
  pen.Vert( p.rect.right-3, p.rect.top, h );
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
void SFscList::FuncSetLnk13( VAR_SETLNK )
{
  // Вход только справа
  if ( x == p.rect.right )
    return;
  SFlipFlopSR* dat = (SFlipFlopSR*)fsc->Data(p.nBuff);
  int yy = (p.rect.top+p.rect.bottom)/2;
  if ( y < yy )
    dat->S = nLink;
  else
    dat->R = nLink;
}
////-----------------------------------------------------------------------------
//UINT FuncCursor13( VAR_CURSOR )
//  {
//  SFlipFlopSR* dat = (SFlipFlopSR*)fsc->Data(p.nBuff);
//  int x = (p.rect.left+p.rect.right)/2;
//  int y = (p.rect.top+p.rect.bottom)/2;
//  return ( pt.x < x ) ? ((pt.y < y) ? dat->S : dat->R) : dat->Q;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput13( VAR_OUTPUT )
{
  SFlipFlopSR* dat = (SFlipFlopSR*)fsc->Data(p.nBuff);
  n = 1;
  return &dat->Q;
}
////-----------------------------------------------------------------------------
//// S R | Q
//// 0 1 | 0
//// 1 0 | 1
//// 0 0 | Qt-1
//// 1 1 | 1
void SFscList::FuncTimers13( VAR_TIMERS )
{
  SFlipFlopSR* dat = (SFlipFlopSR*)fsc->Data(p.nBuff);
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
    Q->Set( s );
  //
  SDotValue* N = fsc->DotV(dat->N);
  if ( !(N->dwFlags & 1) )
    N->Set( !s );
}
////-----------------------------------------------------------------------------
static LPCSTR s_pName[] = { "S", "R", "Q", "N" };
int SFscList::FuncQuickWatch13( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  SFlipFlopSR* dat = (SFlipFlopSR*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->S), fsc->DotV(dat->R), fsc->DotV(dat->Q), fsc->DotV(dat->N) };

  int n = 0;
  for( n; n<_countof(vals) && n<cVI; ++n )
  {
    pvi[n].val = vals[n];
    strcpy_s( pvi[n].szName, s_pName[n] );
  }
  return n;
}

int SFscList::SaveState13( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  SFlipFlopSR* dat = (SFlipFlopSR*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->S), fsc->DotV(dat->R), fsc->DotV(dat->Q), fsc->DotV(dat->N) };
  for( int i=0; i<_countof(vals); ++i )
  {
    psaver->WriteDotV( vals[i], s_pName[i] );
  }
  return 0;
}

int SFscList::RestoreState13( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  SFlipFlopSR* dat = (SFlipFlopSR*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->S), fsc->DotV(dat->R), fsc->DotV(dat->Q), fsc->DotV(dat->N) };
  for( int i=0; i<_countof(vals); ++i )
  {
    prest->ReadDotV( vals[i], s_pName[i] );
  }
  return 0;
}

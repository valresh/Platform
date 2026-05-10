#include "FscList.h"
static char* sid = "PULSE";
//
bool SFscList::FuncParser01( VAR_PARSER )
{
  return Trigger( fsc, p, dat, tmp );
}
//
//void FuncDebugs01( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints01( VAR_PAINTS )
{
  STrigger* dat = (STrigger*)fsc->Data(p.nBuff);
  //
  p.DrawR( font, p.rect, "N", 0 );
  //
  RECT a = p.rect;
  POINT pt[6] =
    {
    { a.left +3,a.bottom-10},{ a.left +9,a.bottom-10},{ a.left +9,a.top   + 5},
    { a.right-9,a.top   + 5},{ a.right-9,a.bottom-10},{ a.right-3,a.bottom-10},
    };
  p.DrawTime( fsc, font, nSelect, pt );
}
#endif

void SFscList::FuncSetLnk01( VAR_SETLNK )
{
  // Вход только слева
  if ( x == p.rect.right ) 
    return;
  STrigger* dat = (STrigger*)fsc->Data(p.nBuff);
  int yy = (p.rect.top+p.rect.bottom)/2;
  if ( y <= yy )
    dat->S = nLink;
  else
    dat->R = nLink;
}
////-----------------------------------------------------------------------------
//UINT FuncCursor01( VAR_CURSOR )
//  {
//  STrigger* dat = (STrigger*)fsc->Data(p.nBuff);
//  int x = (p.rect.left+p.rect.right)/2;
//  return ( pt.x < x ) ? dat->S : dat->Q;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput01( VAR_OUTPUT )
  {
  STrigger* dat = (STrigger*)fsc->Data(p.nBuff);
  n = 2;
  return &dat->Q;
  }
////-----------------------------------------------------------------------------
void SFscList::FuncTimers01( VAR_TIMERS )
{
  if ( dt == 0  ) return;
  //
  STrigger* dat = (STrigger*)fsc->Data(p.nBuff);
  //
  SDotValue* S = fsc->DotV(dat->S);
  SDotValue* R = fsc->DotV(dat->R);
  SDotValue* Q = fsc->DotV(dat->Q);
  SDotValue* U = fsc->DotV(dat->u);
  //
  bool s = S->Bol();
  bool r = ( R != NULL ) ? R->Bol() : false;
  //
  bool   q = false;
  double u = dat->t;
  if( 4==dat->base )
    dt /= 60.;
  if ( !r && s )
    {
    q = true, u = U->Dbl()-dt;
    if ( u < 0 )
      q = false, u = 0;
    }
  //
  if ( (Q->dwFlags & 1) == 0 )
    Q->Set(q);
  //
  if ( (U->dwFlags & 1) == 0 )
    U->Set(u);
  //
}
////-----------------------------------------------------------------------------
static LPCSTR s_pName[] = { "S", "R", "Q", "U" };
int SFscList::FuncQuickWatch01( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  STrigger* dat = (STrigger*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->S), fsc->DotV(dat->R), fsc->DotV(dat->Q), fsc->DotV(dat->u) };

  int n = 0;
  for( n; n<_countof(vals) && n<cVI; ++n )
  {
    pvi[n].val = vals[n];
    strcpy_s( pvi[n].szName, s_pName[n] );
  }
  return n;
}

int SFscList::SaveState01( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  STrigger* dat = (STrigger*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->S), fsc->DotV(dat->R), fsc->DotV(dat->Q), fsc->DotV(dat->u) };
  for( int i=0; i<_countof(vals); ++i )
  {
    psaver->WriteDotV( vals[i], s_pName[i] );
  }
  return 0;
}

int SFscList::RestoreState01( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  STrigger* dat = (STrigger*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->S), fsc->DotV(dat->R), fsc->DotV(dat->Q), fsc->DotV(dat->u) };
  for( int i=0; i<_countof(vals); ++i )
  {
    prest->ReadDotV( vals[i], s_pName[i] );
  }
  return 0;
}

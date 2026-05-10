#include "FscList.h"
static char* sid = "Delayed ON";
//
bool SFscList::FuncParser03( VAR_PARSER )
{
    return Trigger( fsc, p, dat, tmp );
}
//
//void FuncDebugs03( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints03( VAR_PAINTS )
{
  STrigger* dat = (STrigger*)fsc->Data(p.nBuff);
  //
  p.DrawL( font, p.rect, " t", 0 );
  p.DrawR( font, p.rect, "0 ", 0 );
  //
  RECT a = p.rect;
  POINT pt[6] =
    {
    { a.left +5,a.top+ 6},{ a.left +5,a.top+12},{ a.left +5,a.top+ 9},
    { a.right-5,a.top+ 9},{ a.right-5,a.top+12},{ a.right-5,a.top+ 6},
    };
  p.DrawTime( fsc, font, nSelect, pt );
}
#endif
//
void SFscList::FuncSetLnk03( VAR_SETLNK )
{
  // Вход только справа
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
//UINT FuncCursor03( VAR_CURSOR )
//  {
//  STrigger* dat = (STrigger*)fsc->Data(p.nBuff);
//  int x = (p.rect.left+p.rect.right)/2;
//  return ( pt.x < x ) ? dat->S : dat->Q;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput03( VAR_OUTPUT )
{
  STrigger* dat = (STrigger*)fsc->Data(p.nBuff);
  n = 2;
  return &dat->Q;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers03( VAR_TIMERS )
{
  if ( dt == 0  ) return;
  //
  STrigger* dat = (STrigger*)fsc->Data(p.nBuff);
  //
  SDotValue* S = fsc->DotV(dat->S);
  SDotValue* R = fsc->DotV(dat->R);
  SDotValue* Q = fsc->DotV(dat->Q);
  SDotValue* U = fsc->DotV(dat->u);
  SDotValue* X = fsc->DotV(dat->X);
  //
  bool   s = S->Bol();
  bool   r = ( R != NULL ) ? R->Bol() : false;
  bool   q = Q->Bol();
  double u = U->Dbl();
  // S - изменил состояние
  bool cngS = s != X->Bol();
  X->Set(s);
  //
  // Изменение S 0->1 запускает изменение u сначала
  //
  if( 4==dat->base )
    dt /= 60.;
  if ( r ) //Reset
    q = false, u = dat->t;
  else
    {
    q = ( u == 0 );
    if ( cngS )
      {
      u = dat->t;
      if ( s ) u -= dt;
      else     q = false;
      }
    else
      {
      if ( s && u < dat->t ) u -= dt;
      }
    }
  //
  if ( u <= 0 ) u = 0;
  //
  if ( (Q->dwFlags & 1) == 0 )
    Q->Set(q);
  //
  if ( (U->dwFlags & 1) == 0 )
    U->Set(u);
  //
}
////-----------------------------------------------------------------------------
static LPCSTR s_pName[] = { "S", "R", "Q", "U", "X" };
int SFscList::FuncQuickWatch03( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  STrigger* dat = (STrigger*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->S), fsc->DotV(dat->R), fsc->DotV(dat->Q), fsc->DotV(dat->u), fsc->DotV(dat->X) };

  int n = 0;
  for( n; n<_countof(vals) && n<cVI; ++n )
  {
    pvi[n].val = vals[n];
    strcpy_s( pvi[n].szName, s_pName[n] );
  }
  return n;
}

int SFscList::SaveState03( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  STrigger* dat = (STrigger*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->S), fsc->DotV(dat->R), fsc->DotV(dat->Q), fsc->DotV(dat->u), fsc->DotV(dat->X) };
  for( int i=0; i<_countof(vals); ++i )
  {
    psaver->WriteDotV( vals[i], s_pName[i] );
  }
  return 0;
}

int SFscList::RestoreState03( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  STrigger* dat = (STrigger*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->S), fsc->DotV(dat->R), fsc->DotV(dat->Q), fsc->DotV(dat->u), fsc->DotV(dat->X) };
  for( int i=0; i<_countof(vals); ++i )
  {
    prest->ReadDotV( vals[i], s_pName[i] );
  }
  return 0;
}

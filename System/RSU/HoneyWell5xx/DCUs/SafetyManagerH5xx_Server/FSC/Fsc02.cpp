#include "FscList.h"
static char* sid = "PULSE Retriggerable";
//
bool SFscList::FuncParser02( VAR_PARSER )
{
  return Trigger( fsc, p, dat, tmp );
}
//
//void FuncDebugs02( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints02( VAR_PAINTS )
{
  STrigger* dat = (STrigger*)fsc->Data(p.nBuff);
  //
  p.DrawR( font, p.rect, "R", 0 );
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
//
void SFscList::FuncSetLnk02( VAR_SETLNK )
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
//UINT FuncCursor02( VAR_CURSOR )
//  {
//  STrigger* dat = (STrigger*)fsc->Data(p.nBuff);
//  int x = (p.rect.left+p.rect.right)/2;
//  return ( pt.x < x ) ? dat->S : dat->Q;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput02( VAR_OUTPUT )
{
  STrigger* dat = (STrigger*)fsc->Data(p.nBuff);
  n = 2;
  return &dat->Q;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers02( VAR_TIMERS )
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
  bool s = S->Bol();
  bool r = ( R != NULL ) ? R->Bol() : false;
  bool   q = Q->Bol();
  double u = U->Dbl();
  // S - изменил состояние
  bool cngS = s != X->Bol();
  X->Set(s);
  //
  // Изменение S 0->1 запускает изменение u сначала, устанавливая Q = 1
  //
  if( 4==dat->base )
    dt /= 60.;
  if ( r ) //Reset
    q = false, u = dat->t;
  else
    {
    if ( cngS )
      {
      if ( s )
        q = true, u = dat->t;
      else
        {
        if ( u == 0 )
          q = false, u = dat->t;
        }
      }
    //
    if ( s || q )
      {
      u -= dt;
      if ( u < 0 )
        q = false, u = 0;
      //if ( u == 0 && !q )
      //  u = dat->t;
      }
    //
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
static LPCSTR s_pName[] = { "S", "R", "Q", "U", "X" };
int SFscList::FuncQuickWatch02( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
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

int SFscList::SaveState02( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  STrigger* dat = (STrigger*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->S), fsc->DotV(dat->R), fsc->DotV(dat->Q), fsc->DotV(dat->u), fsc->DotV(dat->X) };
  for( int i=0; i<_countof(vals); ++i )
  {
    psaver->WriteDotV( vals[i], s_pName[i] );
  }
  return 0;
}

int SFscList::RestoreState02( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  STrigger* dat = (STrigger*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->S), fsc->DotV(dat->R), fsc->DotV(dat->Q), fsc->DotV(dat->u), fsc->DotV(dat->X) };
  for( int i=0; i<_countof(vals); ++i )
  {
    prest->ReadDotV( vals[i], s_pName[i] );
  }
  return 0;
}

#include "FscList.h"
static char* sid = "Counter";

struct SCounter
{
  UINT CUp;
  UINT CDown;
  UINT Set;
  UINT DpresetValue;
  UINT Reset;
  UINT FoutIsAtmaximum;
  UINT GoutputValue;
  UINT HoutputIsZero;
  UINT maxValue;
  UINT prevCup;
  UINT prevCdown;
};

//
bool SFscList::FuncParser2A( VAR_PARSER )
{
  int t = 8000;
  ASS( dat.data[0]==1);
  //ASS( dat.data[1]==0x36);
  //ASSD( dat.data[2]==1 );
  FuncParserdo(fsc,p,dat,tmp);

  SCounter io;
  memset( &io, -1, sizeof(io) );


  SDotValue FoutIsAtmaximum(enumValueBol);
  SDotValue GoutputValue(enumValueInt);
  SDotValue HoutputIsZero(enumValueBol);
  SDotValue prevCup(enumValueBol);
  SDotValue prevCdown(enumValueBol);

  SDotValue maxV(enumValueInt);
  maxV.nVal = *(short*)&dat.data[3];

  ASS( maxV.nVal>0 && maxV.nVal<8192 );

  io.GoutputValue = fsc->AddFscDots(&GoutputValue);
  io.FoutIsAtmaximum = fsc->AddFscDots(&FoutIsAtmaximum);
  io.HoutputIsZero = fsc->AddFscDots(&HoutputIsZero);
  io.maxValue = fsc->AddFscDots(&maxV);
  io.prevCup = fsc->AddFscDots(&prevCup);
  io.prevCdown = fsc->AddFscDots(&prevCdown);

  p.nBuff = fsc->AddFscBuff( &io, sizeof(io) );

  ns_UT::SLocalTmp* item = tmp.Item();
  item->nCount = 3;
  item->dim[2] = io.HoutputIsZero;
  item->dim[1] = io.GoutputValue;
  item->dim[0] = io.FoutIsAtmaximum;

  return true;
}
//
//void FuncDebugs2A( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints2A( VAR_PAINTS )
{
  COLORREF color = 0x808000;
  SCounter* dat = NULL;
  if ( p.nBuff != -1 )
  {
    dat = (SCounter*)fsc->Data(p.nBuff);
    if ( p.nBuff == nSelect )
      color = SELCOLOR;
  }
  RECT tmp = p.rect;
  ::InflateRect( &tmp, -1, 2 );
  CMyBrush brush( font, BACKGROUND );
  brush.Rect( tmp );
  CMyPen pen( font, color );
  pen.Rect( p.rect );
  RECT rc = p.rect;
  rc.left += 2;
  LPCSTR pInputs[] = {"CU", "CD","S", "Counter", "R"};

  int dy = (rc.bottom-rc.top)/_countof(pInputs);
  rc.bottom = rc.top + dy;
  for( int i=0; i<_countof(pInputs); ++i, rc.top+=dy, rc.bottom+=dy )
  {
    font.DrawS( rc, pInputs[i], DT_LEFT );
  }
  rc.top-=dy, rc.bottom-=dy;
  font.DrawS( rc, "O", DT_RIGHT );

  char txt[64] = "=";
  if( dat )
  {
    SDotValue* maxValue = fsc->DotV(dat->maxValue);
    sprintf_s( txt, "=%d", maxValue->nVal );
  }
  rc = p.rect;
  rc.left += (rc.right-rc.left) / 2;
  p.DrawL( font, rc, txt, 0 );
}
#endif
//
void SFscList::FuncSetLnk2A( IFscStorage* fsc, SFscBase& p, UINT nLink, int x, int y )
{
  if ( nLink == -1 ) 
    return;
  if ( x == p.rect.right ) 
    return;
  //
  SDotValue* dot = fsc->DotV(nLink);

  SCounter* dat = (SCounter*)fsc->Data(p.nBuff);
  UINT *ins[] = { &dat->CUp, &dat->CDown, &dat->Set, &dat->DpresetValue, &dat->Reset };
  int dy = (p.rect.bottom-p.rect.top)/_countof(ins);
  int ym = p.rect.top;
  for( int i=0; i<_countof(ins); ++i, ym+=dy )
  {
    if( y>ym && y<(ym+dy) )
    {
      ASS( -1==*ins[i] || *ins[i] == nLink );
      *ins[i] = nLink;
      break;
    }
  }
}
////-----------------------------------------------------------------------------
//UINT FuncCursor2A( VAR_CURSOR )
//  {
//  return p.nBuff;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput2A( VAR_OUTPUT )
{
  n = 3;
  SCounter* dat = (SCounter*)fsc->Data(p.nBuff);
  return &dat->FoutIsAtmaximum;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers2A( VAR_TIMERS )
{
  if( dt == 0 )
    return;
  SCounter* dat = (SCounter*)fsc->Data(p.nBuff);
  SDotValue* A = fsc->DotV(dat->CUp);
  SDotValue* B = fsc->DotV(dat->CDown);
  SDotValue* C = fsc->DotV(dat->Set);
  SDotValue* D = fsc->DotV(dat->DpresetValue);
  SDotValue* E = fsc->DotV(dat->Reset);
  
  SDotValue* GoutputValue = fsc->DotV(dat->GoutputValue);
  SDotValue* FoutIsAtmaximum = fsc->DotV(dat->FoutIsAtmaximum);
  SDotValue* HoutputIsZero = fsc->DotV(dat->HoutputIsZero);
  SDotValue* maxValue = fsc->DotV(dat->maxValue);
  SDotValue* prevCup = fsc->DotV(dat->prevCup);
  SDotValue* prevCdown = fsc->DotV(dat->prevCdown);

  if( E && E->Bol() )//Reset
    GoutputValue->nVal = 0;

  if( C && C->Bol() && D )//Set
  {
    ASSD( enumValueInt==D->eType );
    GoutputValue->nVal = D->nVal;
  }

  if( B )
  {
    if( B->Bol() && !prevCdown->Bol() )
      --GoutputValue->nVal;
    prevCdown->bVal = B->bVal;
  }

  if( A )
  {
    if( A->Bol() && !prevCup->Bol() )
      ++GoutputValue->nVal;
    prevCup->bVal = A->bVal;
  }

  if( GoutputValue->nVal > maxValue->nVal )
    GoutputValue->nVal = maxValue->nVal;

  if( GoutputValue->nVal == maxValue->nVal )
    FoutIsAtmaximum->bVal = 1;
  else
    FoutIsAtmaximum->bVal = 0;

  if( !GoutputValue->nVal )
    HoutputIsZero->bVal = true;
  else
    HoutputIsZero->bVal = false;
}
////-----------------------------------------------------------------------------
static LPCSTR s_pName[] = { "GoutputValue", "FoutIsAtmaximum", "HoutputIsZero", "maxValue", "prevCup", "prevCdown" };

static LPCSTR s_pQWNames[] = { "Cu", "Cd", "Set", "Dcounter", "Reset", "Goutput", "F IsMax", "H IsZero", "maxValue", "prevCup", "prevCdown" };
int SFscList::FuncQuickWatch2A( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  SCounter* dat = (SCounter*)fsc->Data(obj->nBuff);

  SDotValue* vals[] = { fsc->DotV(dat->CUp), fsc->DotV(dat->CDown), fsc->DotV(dat->Set), fsc->DotV(dat->DpresetValue), fsc->DotV(dat->Reset),
                        fsc->DotV(dat->GoutputValue), fsc->DotV(dat->FoutIsAtmaximum), fsc->DotV(dat->HoutputIsZero), fsc->DotV(dat->maxValue), fsc->DotV(dat->prevCup), fsc->DotV(dat->prevCdown) };

  int J = 0;
  for( int n=0; n<_countof(vals) && n<cVI; ++n )
  {
    if( !vals[n] )
      continue;
    pvi[J].val = vals[n];
    strcpy_s( pvi[J].szName, s_pQWNames[n] );
    ++J;
  }
  return J;
}

int SFscList::SaveState2A( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  SCounter *dat = (SCounter*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->GoutputValue), fsc->DotV(dat->FoutIsAtmaximum), fsc->DotV(dat->HoutputIsZero), 
                        fsc->DotV(dat->maxValue), fsc->DotV(dat->prevCup), fsc->DotV(dat->prevCdown) };
  for( int i=0; i<_countof(vals); ++i )
  {
    psaver->WriteDotV( vals[i], s_pName[i] );
  }
  return 0;
}

int SFscList::RestoreState2A( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  SCounter *dat = (SCounter*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->GoutputValue), fsc->DotV(dat->FoutIsAtmaximum), fsc->DotV(dat->HoutputIsZero), 
                        fsc->DotV(dat->maxValue), fsc->DotV(dat->prevCup), fsc->DotV(dat->prevCdown) };
  for( int i=0; i<_countof(vals); ++i )
  {
    prest->ReadDotV( vals[i], s_pName[i] );
  }
  return 0;
}

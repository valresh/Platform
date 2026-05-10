#include "FscList.h"

static char* sid = "Equation block";
//
bool SFscList::FuncParser1C( VAR_PARSER )
{
  FuncParserdo(fsc,p,dat,tmp);
  LPWORD w = (LPWORD)(dat.data);
  SEquationBlock io;
  io.nProc = w[0];
  io.w2 = w[1];
  io.nPlace = -1;
  // Выходные точки
  SDotValue O(enumValueDbl);
  io.Y = fsc->AddFscDots(&O);
  // Входные точки
  io.X = -1;
  // Заказываем память
  p.nBuff = fsc->AddFscBuff( &io, sizeof(io) );
  //
  ns_UT::SLocalTmp* item = tmp.Item();
  item->nCount = 1;
  item->dim[0] = io.Y;
  return true;
}

#ifdef _WIN32
void SFscList::FuncPaints1C( VAR_PAINTS )
{
  SEquationBlock* dat = (SEquationBlock*)fsc->Data(p.nBuff);
  char txt[32*4];
  //
  CMyPen pen( font, 0x808000 );
  pen.Rect( p.rect );
  font.Draw( p.rect, "EB" );
  RECT rc = p.rect;
  ::OffsetRect( &rc, 0, 10 );
  sprintf_s( txt, "%d", dat->nProc );
  font.Draw( rc, txt );
  txt[1] = 0;
  //
  rc = p.rect; rc.left += 2;
}
#endif

void SFscList::FuncSetLnk1C( IFscStorage* fsc, SFscBase& p, UINT nLink, int x, int y )
{
  if ( x == p.rect.right ) 
    return;
  SEquationBlock* dat = (SEquationBlock*)fsc->Data(p.nBuff);
  dat->X = nLink;
}

UINT* SFscList::FuncOutput1C( VAR_OUTPUT )
{
  SEquationBlock* dat = (SEquationBlock*)fsc->Data(p.nBuff);
  n = 1;
  return &dat->Y;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers1C( IFscStorage* fsc, SFscBase& p, double dt )
{
  SEquationBlock* dat = (SEquationBlock*)fsc->Data(p.nBuff);
  SDotValue* X = fsc->DotV(dat->X);
  SDotValue* Y = fsc->DotV(dat->Y);

  if( !X || !Y )
    return;

  double inX = X->Dbl();

  if( !(dat->nFirstPlaceRow < dat->nLastPlaceRow) )
    return;

  for( UINT n=dat->nFirstPlaceRow, E(dat->nLastPlaceRow-1); n<E; ++n )
  {
    LFscBase* item1 = fsc->GetItem( n );
    LFscBase* item2 = fsc->GetItem( n+1 );
    if( !item1 || !item2 )
      break;
    SEquationBlockRow* row1 = (SEquationBlockRow*)fsc->FscGiveData(item1->nBuff);
    SEquationBlockRow* row2 = (SEquationBlockRow*)fsc->FscGiveData(item2->nBuff);
    if( inX < row1->X )
    {
      Y->Set( row1->Y );
      return;
    }
    if( inX >= row1->X && inX< row2->X )
    {
      double lx = (inX-row1->X) / (row2->X - row1->X);
      double ly = (row2->Y - row1->Y);
      double s = ly * lx + row1->Y;
      Y->Set( s );
      return;
    }
  }
  LFscBase* item2 = fsc->GetItem( dat->nLastPlaceRow );
  if( !item2 )
    return;
  SEquationBlockRow* row2 = (SEquationBlockRow*)fsc->FscGiveData(item2->nBuff);
  Y->Set( row2->Y );
}
////-----------------------------------------------------------------------------
int SFscList::FuncQuickWatch1C( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  return 0;
}

int SFscList::SaveState1C( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}

int SFscList::RestoreState1C( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}

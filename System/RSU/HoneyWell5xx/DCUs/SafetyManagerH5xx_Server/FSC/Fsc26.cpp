#include "FscList.h"
static char* sid = "PULSE";
//
struct SPulse26
{
  UINT I;// Вход
  UINT O;// Выход
  UINT u;// Remaining time value
  double t;// Time value
};
//
bool SFscList::FuncParser26( VAR_PARSER )
{
  FuncParserdo(fsc,p,dat,tmp);
  //
  SPulse26 io;
  // Входные точки
  io.I = -1;
  //io.t = 0.5;// Заказываем пол-секунды
  io.t = 1;// Заказываем секунду
  // Выходные точки
  SDotValue O(enumValueBol);
  SDotValue u(enumValueDbl); u.dVal = io.t;
  io.O = fsc->AddFscDots(&O);
  io.u = fsc->AddFscDots(&u);
  // Заказываем память
  p.nBuff = fsc->AddFscBuff( &io, sizeof(io) );
  //
  ns_UT::SLocalTmp* item = tmp.Item();
  item->nCount = 2;
  item->dim[0] = io.O;
  item->dim[1] = io.u;
  return true;
}
//
//void FuncDebugs26( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints26( VAR_PAINTS )
{
  SPulse26* dat = (SPulse26*)fsc->Data(p.nBuff);
  //
  COLORREF color = (nSelect == dat->O) ? SELCOLOR : BOXCOLOR;
  if ( nSelect != -1 && dat->I == nSelect )
    color = SELCOLOR;
  CMyPen pen( font, color );
  pen.Rect( p.rect );
  RECT a = p.rect;
  POINT pt[6] =
    {
    { a.left +3,a.bottom-2},
    { a.left +7,a.bottom-2},
    { a.left +7,a.top   +2},
    { a.right-7,a.top   +2},
    { a.right-7,a.bottom-2},
    { a.right-3,a.bottom-2},
    };
  pen.Points( pt, 6, 0 );
  //
  {
  SDotValue* u = fsc->DotV(dat->u);
  double val = dat->t-u->dVal;
  val = std::max<double>( val, 0 );
  val = std::min<double>( val, dat->t  );
  int x = int((pt[0].x*(dat->t-val)+pt[5].x*val)/dat->t);
  CMyPen pen( font, 0x00ffff );
  pen.Horz( pt[0].y, pt[0].x, x );
  }
  p.BadRect( font, dat->I );
}
#endif

void SFscList::FuncSetLnk26( VAR_SETLNK )
{
  // Вход только справа
  if ( x == p.rect.right ) 
    return;
  SPulse26* dat = (SPulse26*)fsc->Data(p.nBuff);
  dat->I = nLink;
}
////-----------------------------------------------------------------------------
//UINT FuncCursor26( VAR_CURSOR )
//  {
//  SPulse26* dat = (SPulse26*)fsc->Data(p.nBuff);
//  int x = (p.rect.left+p.rect.right)/2;
//  return ( pt.x < x ) ? dat->I : dat->O;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput26( VAR_OUTPUT )
{
  SPulse26* dat = (SPulse26*)fsc->Data(p.nBuff);
  n = 1;
  return &dat->O;
}
////-----------------------------------------------------------------------------
//// Function:
//// N   a | b
//// __________
//// 1   1 | 1
//// >1  1 | 0
void SFscList::FuncTimers26( VAR_TIMERS )
{
  if ( dt == 0 ) return;
  //
  SPulse26* dat = (SPulse26*)fsc->Data(p.nBuff);
  if ( dat->I == -1 ) return;
  //
  SDotValue* I = fsc->DotV(dat->I);
  SDotValue* O = fsc->DotV(dat->O);
  SDotValue* u = fsc->DotV(dat->u);
  bool o;
  bool i = I->Bol();
  if ( i )
    {
    u->dVal -= dt;
    if ( u->dVal <= 0 )
      {
      u->dVal = 0;
      o = false;
      }
    else
      o = true;
    }
  else
    {
    o = false;
    u->dVal = dat->t;
    }
  if ( (O->dwFlags & 1) == 0 )
    O->Set(o);
}
////-----------------------------------------------------------------------------
static LPCSTR s_pName[] = { "O", "u" };
int SFscList::FuncQuickWatch26( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  return 0;
}

int SFscList::SaveState26( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  SPulse26* dat = (SPulse26*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->O), fsc->DotV(dat->u) };
  for( int i=0; i<_countof(vals); ++i )
  {
    psaver->WriteDotV( vals[i], s_pName[i] );
  }
  return 0;
}

int SFscList::RestoreState26( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  SPulse26* dat = (SPulse26*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->O), fsc->DotV(dat->u) };
  for( int i=0; i<_countof(vals); ++i )
  {
    prest->ReadDotV( vals[i], s_pName[i] );
  }
  return 0;
}

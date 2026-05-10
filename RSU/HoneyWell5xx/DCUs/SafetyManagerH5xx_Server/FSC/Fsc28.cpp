#include "FscList.h"
static char* sid = "Операция NOT";
//
struct SLogic28
  {
  UINT I;// Вход
  UINT O;// Выход
  };
//
bool SFscList::FuncParser28( VAR_PARSER )
{
  FuncParserdo(fsc,p,dat,tmp);
  int h = (p.rect.bottom-p.rect.top);
  int w = (p.rect.right-p.rect.left);
  ASS( (h % 2) == 0 );
  ASS( (w % 2) == 0 );
  p.rect.OffsetRect(-w / 2, -h / 2);
  //
  SLogic28 io;
  // Выходные точки
  SDotValue O(enumValueBol);
  io.O = fsc->AddFscDots(&O);
  // Входные точки
  io.I = -1;
  // Заказываем память
  p.nBuff = fsc->AddFscBuff( &io, sizeof(io) );
  //
  ns_UT::SLocalTmp* item = tmp.Item();
  item->nCount = 1;
  item->dim[0] = io.O;
  return true;
}
#ifdef _WIN32
//
//void FuncDebugs28( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
void SFscList::FuncPaints28( VAR_PAINTS )
{
  SLogic28* dat = (SLogic28*)fsc->Data(p.nBuff);
  //
  CMyPen pen( font, p.Color( nSelect, dat->I, dat->O ) );
  CMyBrush brush( font, 0x101010, true );
  pen.Ellipse( p.rect );
}
#endif
//
void SFscList::FuncSetLnk28( VAR_SETLNK )
{
  // Вход только справа
  if ( x == p.rect.right || y == p.rect.bottom ) 
    return;
  SLogic28* dat = (SLogic28*)fsc->Data(p.nBuff);
  dat->I = nLink;
}
////-----------------------------------------------------------------------------
//UINT FuncCursor28( VAR_CURSOR )
//  {
//  SLogic28* dat = (SLogic28*)fsc->Data(p.nBuff);
//  int x = (p.rect.left+p.rect.right)/2;
//  return ( pt.x < x ) ? dat->I : dat->O;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput28( VAR_OUTPUT )
{
  SLogic28* dat = (SLogic28*)fsc->Data(p.nBuff);
  n = 1;
  return &dat->O;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers28( VAR_TIMERS )
{
  SLogic28* dat = (SLogic28*)fsc->Data(p.nBuff);
  SDotValue* O = fsc->DotV(dat->O);
  if ( O->dwFlags & 1 )
    return;
  SDotValue* I = fsc->DotV(dat->I);
	if ( I == NULL )
		return ;
  bool i = I->Bol();
  O->Set( !i );
}
////-----------------------------------------------------------------------------
static LPCSTR s_pName[] = { "O" };
int SFscList::FuncQuickWatch28( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  return 0;
}

int SFscList::SaveState28( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  SLogic28* dat = (SLogic28*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->O) };
  for( int i=0; i<_countof(vals); ++i )
  {
    psaver->WriteDotV( vals[i], s_pName[i] );
  }
  return 0;
}

int SFscList::RestoreState28( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  SLogic28* dat = (SLogic28*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->O) };
  for( int i=0; i<_countof(vals); ++i )
  {
    prest->ReadDotV( vals[i], s_pName[i] );
  }
  return 0;
}

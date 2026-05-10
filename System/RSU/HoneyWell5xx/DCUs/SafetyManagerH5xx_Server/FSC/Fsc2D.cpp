#include "FscList.h"
static char* sid = "Операция <=";//AT-6
//
bool SFscList::FuncParser2D( VAR_PARSER )
{
  return LogicDB(fsc,p,dat,tmp);
}
//
//void FuncDebugs2D( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints2D( VAR_PAINTS )
{
  p.LogicOut( fsc, font, nSelect, "<=" );
}
#endif
//
void SFscList::FuncSetLnk2D( VAR_SETLNK )
{
  // Вход только справа
  if ( x == p.rect.right ) 
    return;
  p.LogicLnk( fsc, nLink, y );
}
////-----------------------------------------------------------------------------
//UINT FuncCursor2D( VAR_CURSOR )
//  {
//  SLogicDB* dat = (SLogicDB*)fsc->Data(p.nBuff);
//  int x = (p.rect.left+p.rect.right)/2;
//  int y = (p.rect.top+p.rect.bottom)/2;
//  return ( pt.x < x ) ? ((pt.y < y) ? dat->In1 : dat->In2) : dat->O;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput2D( VAR_OUTPUT )
{
  SLogicDB* dat = (SLogicDB*)fsc->Data(p.nBuff);
  n = 1;
  return &dat->O;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers2D( VAR_TIMERS )
{
  SLogicDB* dat = (SLogicDB*)fsc->Data(p.nBuff);
  SDotValue* O = fsc->DotV(dat->O);
  if ( O->dwFlags & 1 )
    return;
  //
  SDotValue* I1 = fsc->DotV(dat->In1);
  if ( I1 == NULL ) return;
  SDotValue* I2 = fsc->DotV(dat->In2);
  if ( I2 == NULL )
    return;
  //
  double i1 = I1->Dbl();
  double i2 = I2->Dbl();
  //
  bool o = i1 <= i2;
  O->Set(o);
}
//-----------------------------------------------------------------------------
static LPCSTR s_pName[] = { "O" };
int SFscList::FuncQuickWatch2D( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  return 0;
}

int SFscList::SaveState2D( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  SLogicDB* dat = (SLogicDB*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->O) };
  for( int i=0; i<_countof(vals); ++i )
  {
    psaver->WriteDotV( vals[i], s_pName[i] );
  }
  return 0;
}

int SFscList::RestoreState2D( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  SLogicDB* dat = (SLogicDB*)fsc->Data(obj->nBuff);
  SDotValue* vals[] = { fsc->DotV(dat->O) };
  for( int i=0; i<_countof(vals); ++i )
  {
    prest->ReadDotV( vals[i], s_pName[i] );
  }
  return 0;
}

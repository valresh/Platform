#include "FscList.h"
static char* sid = "Заголовочная надпись";
//
bool SFscList::FuncParser3E( VAR_PARSER )
{
  FuncParserdo(fsc,p,dat,tmp);
  p.rect.SetRect(0, 0, 0, 0);
  p.rect.OffsetRect( - 100, -100 );
  return true;
}
#ifdef _WIN32
//
//void FuncDebugs3E( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
void SFscList::FuncPaints3E( VAR_PAINTS )
{
}
#endif
//
void SFscList::FuncSetLnk3E( VAR_SETLNK )
{
// Не используется
}
////-----------------------------------------------------------------------------
//UINT FuncCursor3E( VAR_CURSOR )
//  {
//  return -1;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput3E( VAR_OUTPUT )
{
  n = 0;
  return NULL;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers3E( VAR_TIMERS )
{
}
////-----------------------------------------------------------------------------
int SFscList::FuncQuickWatch3E( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  return 0;
}

int SFscList::SaveState3E( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}

int SFscList::RestoreState3E( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}

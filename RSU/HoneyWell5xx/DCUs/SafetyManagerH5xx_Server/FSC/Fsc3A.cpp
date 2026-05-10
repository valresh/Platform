#include "FscList.h"
static char* sid = "Линия Вертикальная";
//
bool SFscList::FuncParser3A( VAR_PARSER )
{
  FuncParserdo(fsc,p,dat,tmp);
  //ASSERT( p.mB == 0 );
  //for ( int n = 0; n < 17; n++ )
  //  {
  //  ASSERT( dat.data[n] == 0 );
  //  }
  return true;
}
//
//void FuncDebugs3A( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints3A( VAR_PAINTS )
{
  COLORREF color = BADCOLOR;
  if ( p.nBuff != -1 )
    {
    SDotValue* dot = fsc->DotV(p.nBuff);
    if ( dot->eType == enumValueDbl )
    color = BADCOLOR;
    else
    color = dot->bVal ? COLOR_1 : COLOR_0;
    if ( p.nBuff == nSelect )
      color = SELCOLOR;
    }
  CMyPen pen( font, color );
  pen.Vert( p.rect.left, p.rect.top, p.rect.bottom );
}
#endif
//
void SFscList::FuncSetLnk3A( VAR_SETLNK )
{
  if ( p.nBuff != -1 ) 
    return;
  //
  SDotValue* dot = fsc->DotV(nLink);
  ASS( dot->eType == enumValueBol /*|| dot->eType == enumValueDbl*/ );
  p.nBuff = nLink;
}
////-----------------------------------------------------------------------------
//UINT FuncCursor3A( VAR_CURSOR )
//  {
//  return p.nBuff;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput3A( VAR_OUTPUT )
{
  n = 0;
  return NULL;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers3A( VAR_TIMERS )
{
}
////-----------------------------------------------------------------------------
int SFscList::FuncQuickWatch3A( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  return 0;
}

int SFscList::SaveState3A( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}

int SFscList::RestoreState3A( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}

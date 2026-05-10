#include "FscList.h"
static char* sid = "Линия Горизонтальная Float";
//
bool SFscList::FuncParser3D( VAR_PARSER )
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
//void FuncDebugs3D( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints3D( VAR_PAINTS )
{
  COLORREF color = BADCOLOR;
  if ( p.nBuff != -1 )
    {
    SDotValue* dot = fsc->DotV(p.nBuff);
    if ( dot->eType == enumValueBol )
      {
      CMyPen pen( font, dot->bVal ? COLOR_1 : COLOR_0 );
      pen.Horz( (p.rect.top+p.rect.bottom)/2, p.rect.left, p.rect.right );
      }
    color = 0x808080;
    if ( p.nBuff == nSelect )
      color = SELCOLOR;
    }
  //
  CMyPen pen( font, color );
  pen.Rect( p.rect );
}
#endif
//
void SFscList::FuncSetLnk3D( VAR_SETLNK )
{
  SDotValue* dot = fsc->DotV(nLink);
  if( p.nBuff != -1 )
  {
    if( p.nBuff == nLink )
      return;
    SDotValue* dotC = fsc->DotV(p.nBuff);
    if( enumValueDbl!=dotC->eType && enumValueDbl==dot->eType )
      KKK();
    else
      return;
  }
  //
  /*if( enumValueDbl!=dot->eType )
    return;*/
  p.nBuff = nLink;
}
////-----------------------------------------------------------------------------
//UINT FuncCursor3D( VAR_CURSOR )
//  {
//  return p.nBuff;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput3D( VAR_OUTPUT )
{
  n = 0;
  return NULL;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers3D( VAR_TIMERS )
{
}
////-----------------------------------------------------------------------------
int SFscList::FuncQuickWatch3D( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  return 0;
}

int SFscList::SaveState3D( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}

int SFscList::RestoreState3D( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}

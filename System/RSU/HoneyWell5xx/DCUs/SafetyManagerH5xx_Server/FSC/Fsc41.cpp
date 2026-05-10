#include "FscList.h"
#include <rsuProjectVersion.h>
#include <rsuErr.h>

static char* sid = "Процедура";
//
bool SFscList::FuncParser41( VAR_PARSER )
{
  FuncParserdo(fsc,p,dat,tmp);
  //
  if ( dat.data[0] != 1 )
    {
    ASS( dat.data[0] == 2 || dat.data[0] == 3 );
    return false;
    }
  //
  LPWORD w = (LPWORD)(dat.data+1);
  SProcedure io;
  io.nProc = w[0];
  io.nPlace = -1;
  io.nI = dat.data[4];
  io.nO = dat.data[5];
  // Выходные точки
  memset( io.O, -1, sizeof(io.O) );
  // Входные точки
  memset( io.I, -1, sizeof(io.I) );
  //
  memset( io.pI, 0, sizeof(io.pI) );
  memset( io.pO, 0, sizeof(io.pO) );
  // Заказываем память
  p.nBuff = fsc->AddFscBuff( &io, sizeof(io) );
  return true;
}
//
void SFscList::FuncParser41_2( IFscStorage* fsc, SFscBase& p, SfscAbstruct& dat, SUniTemp& tmp )
{
  SProcedure* io = (SProcedure*)fsc->Data(p.nBuff);
  UINT n = 1;
  //
  UINT Ni = 0; 
  for ( ; Ni < _countof(io->pI) ; Ni++ )
  {
    if  ( io->pI[Ni] == 0 )
      break;
  }
  UINT No = 0; 
  for ( ; No < _countof(io->pO); No++ )
  {
    if  ( io->pO[No] == 0 )
      break;
  }
  //
  if( Ni < io->nI )
  {
    for ( ; n < SfscAbstruct::sizeData; n++ )
    {
      if ( Ni   ==  io->nI ) 
        break;
      if ( dat.data[n] == 0 ) 
        break;
      //io->dataI[Ni] = dat.data[n];
      if( (dat.data[n]%8) != 0)
      {
        if( (dat.data[n]%4) == 0)// Признак аналогового входа?
          dat.data[n]-=4;
        else if( (dat.data[n]%4) == 3)// Признак числового (long) входа?
          dat.data[n]-=3;
        else if( (dat.data[n]%4) == 2)// Признак W входа? 0050 w
          dat.data[n]-=2;
        else if( (dat.data[n]%4) == 1)// Признак B входа
          dat.data[n]-=1;
        else
        {
          ASSD(0);
          KKK();
        }
      }
      ASSD( (dat.data[n]%8) == 0);
      ASS( !io->pI[Ni] );
      ASS( Ni < _countof(io->pI) );
      io->pI[Ni++] = dat.data[n]/8;
    }
  }
  //
  if  ( No < io->nO )
  {
    for ( ; n < SfscAbstruct::sizeData; n++ )
    {
      if ( No   ==  io->nO ) 
        break;
      if ( dat.data[n] == 0 ) 
        break;
      //io->dataO[No] = dat.data[n];
      if( (dat.data[n]%8) != 0)
      {
        if( (dat.data[n]%4) == 0)// Признак аналогового выхода?
          dat.data[n]-=4;
        else if( (dat.data[n]%4) == 3)// Признак числового (long) выхода?
          dat.data[n]-=3;
        else if( (dat.data[n]%4) == 2)// Признак W выхода? 1202 w
          dat.data[n]-=2;
        /*else if( (dat.data[n]%4) == 1)// Признак B входа
          dat.data[n]-=1;*/
        else
        {
          ASS(0);
          KKK();
        }
      }
      ASSD( (dat.data[n]%8) == 0);
      ASS( !io->pO[No] );
      ASS( No < _countof(io->pO) );
      io->pO[No++] = dat.data[n]/8;
    }
  }
  //
}
////
//void FuncDebugs41( VAR_DEBUGS )
//  {
//  strcat_s( str, sid );
//  strcat_s( str, "\r\n------------------------------------------------------------\r\n" );
//  FuncDebugsdo( p, str );
//  }
//
#ifdef _WIN32
void SFscList::FuncPaints41( VAR_PAINTS )
{
  SProcedure* dat = (SProcedure*)fsc->Data(p.nBuff);
  char txt[32*4];
  //
  CMyPen pen( font, 0x808000 );
  pen.Rect( p.rect );
  font.Draw( p.rect, "FB" );
  RECT rc = p.rect;
  ::OffsetRect( &rc, 0, 10 );
  sprintf_s( txt, "%d", dat->nProc );
  font.Draw( rc, txt );
  txt[1] = 0;
  //
  rc = p.rect; rc.left += 2;

  int dY = ( p.rect.bottom - p.rect.top ) / dat->nI;
  rc.bottom = rc.top + dY;
  if( dat->nI < dat->nO )
  {
    if( !(dat->nI%2) && !(dat->nO%2) )
    {
      if( dat->nI>2 )
      {
        dY = ( p.rect.bottom - p.rect.top ) / dat->nO;
        rc.top += dY;
        rc.bottom = rc.top + dY;
      }
    }
  }

  for( UINT n = 0; n < dat->nI; n++ )
  {
    //rc.bottom = ( p.rect.bottom*(n+1)+p.rect.top*(dat->nI-n-1) )/dat->nI;
    txt[0] = '@'+dat->pI[n];
    if ( dat->I[n] == -1 )
      p.Select( font, n, dat->nI, true, BADCOLOR );
    else
    if ( dat->I[n] == nSelect )
      p.Select( font, n, dat->nI, true );
    //
    font.DrawS( rc, txt, DT_LEFT );
    rc.bottom += dY;
    rc.top += dY;
  }
  //
  RECT rect = p.rect;
  rc = rect;rc.right -= 2;
  for ( UINT n = 0; n < dat->nO; n++ )
  {
    rc.bottom = ( rect.bottom*(n+1)+rect.top*(dat->nO-n-1) )/dat->nO;
    txt[0] = '@'+dat->pO[n];
    if ( nSelect != -1 )
    {
      if ( dat->O[n] == nSelect )
      {
#if 0
        p.Select( font, n, dat->nO, false );
#else
        RECT r;
        r.top    = rc.top;
        r.bottom = rc.bottom;
        r.left   = rect.right - 8;
        r.right  = rect.right - 1;
        CMyBrush brush( font, SELCOLOR );
        brush.Rect( r );
#endif
      }
    }
    font.DrawS( rc, txt, DT_RIGHT );
    rc.top = rc.bottom;
  }
}
#endif
//
void SFscList::FuncSetLnk41( IFscStorage* fsc, SFscBase& p, UINT nLink, int x, int y )
{
  if ( x == p.rect.right ) 
    return;
  SProcedure* dat = (SProcedure*)fsc->Data(p.nBuff);

  int y0 = p.rect.top;
  int dY = ( p.rect.bottom - p.rect.top ) / dat->nI;
  if( dat->nI < dat->nO )
  {
    if( !(dat->nI%2) && !(dat->nO%2) )
    {
      if( dat->nI>2 )
      {
        if( ns_rsu::eVersion::VSB_KF==rsuProjectVersion() )
        {
          int shift = ( p.rect.bottom - p.rect.top ) / (dat->nO+1);
          y0 += shift;
        }
        else
        {
          dY = ( p.rect.bottom - p.rect.top ) / dat->nO;
          y0 += dY;
        }
      }
    }
    else if( (dat->nI%2) && (dat->nO%2) )
    {
    }
    else
    {
    }
  }
  else if( dat->nI > dat->nO )
  {
  }

  bool bSetted = false;
  for ( UINT n = 0; n < dat->nI; n++ )
  {
    int y1 = y0 + dY;
    if ( y0 <= y && y <= y1 )
    {
      if( dat->I[n]==nLink || -1==dat->I[n] )
      {
        dat->I[n] = nLink;
        bSetted = true;
      }
      break;
    }
    y0 = y1;
  }

  ASSD( bSetted );
}
////-----------------------------------------------------------------------------
//UINT FuncCursor41( VAR_CURSOR )
//  {
//  SProcedure* dat = (SProcedure*)fsc->Data(p.nBuff);
//  int x = (p.rect.left+p.rect.right)/2;
//  // Входные точки
//  if ( pt.x < x )
//    {
//    RECT rc = p.rect;
//    for ( UINT n = 0; n < dat->nI; n++ )
//      {
//      rc.bottom = ( p.rect.bottom*(n+1)+p.rect.top*(dat->nI-n-1) )/dat->nI;
//      if ( pt.y < rc.bottom )
//        return dat->I[n];
//      rc.top = rc.bottom;
//      }
//    return -1;
//    }
//  // Выходные точки
//  RECT rc = p.rect;
//  if ( dat->nProc == 960 )
//    ::InflateRect(&rc,0,-18);
//  RECT rect = rc;
//  if ( pt.y > rect.top && pt.y < rect.bottom  )
//  for ( UINT n = 0; n < dat->nO; n++ )
//    {
//    rc.bottom = ( rect.bottom*(n+1)+rect.top*(dat->nO-n-1) )/dat->nO;
//    if ( pt.y < rc.bottom )
//      return dat->O[n];
//    rc.top = rc.bottom;
//    }
//  return -1;
//  }
////-----------------------------------------------------------------------------
UINT* SFscList::FuncOutput41( VAR_OUTPUT )
{
  n = 0;
  return NULL;
}
////-----------------------------------------------------------------------------
void SFscList::FuncTimers41( VAR_TIMERS )
{
}
////-----------------------------------------------------------------------------
int SFscList::FuncQuickWatch41( IFscStorage* fsc, LFscBase *obj, IFscStorage::SVarInfo *pvi, int cVI, LPCSTR *ppPntName )
{
  return 0;
}

int SFscList::SaveState41( IStateSer *psaver, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}

int SFscList::RestoreState41( IStateSer *prest, IFscStorage* fsc, LFscBase *obj )
{
  return 0;
}

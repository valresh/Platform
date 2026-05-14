#include "winlist.h"
#include "showsheme.h"
#include "l_trends.h"
#include "minitrend.h"


WinList * WinList::pFirst = NULL;
WinList * WinList::pLast = NULL;
bool WinList::change = false;


WinList::WinList()
  {
  if ( pFirst == NULL )
    pFirst = this;
  if ( pLast )
    pLast->pNext = this;
  pLast = this;
  pNext = NULL;
  }

struct Data
  {
  char FileName[64];
  WinList::Types Type;
  QRect WinRect;
  BYTE Params[WINLIST_PARAMS];
  };

void WinList::Save( QFile * pF )
  {
  Data data;
  strcpy_s ( data.FileName, 64, FileName.Str );
  memcpy ( data.Params, Params, WINLIST_PARAMS );
  data.Type = Type;
  data.WinRect = WinRect;
  PreSave();
  pF->write( (const char *)&data, sizeof (data ));
  }


void WinList::Close()
  {
  WinList * pCurr = pFirst;
  WinList * pPrev = NULL;
  while ( pCurr )
    {
    if ( pCurr == this )
      {
      if ( pPrev )
        pPrev->pNext = pCurr->pNext;
      else
        pFirst = pCurr->pNext;
      if ( pLast == this )
        {
        pLast = pPrev;
        }
      break;
      }
    pPrev = pCurr;
    pCurr = pCurr->pNext;
    }
  SaveAll();
  }

void WinList::SaveAll()
  {
  Char<1024>Path;
  Path.Prt ( "%sWND/Linux.dat", PROJECT_ROOT );
  QFile save( (char*)Path );
  if ( save.open(QIODeviceBase::Truncate|QIODeviceBase::WriteOnly))
  {
    WinList * pCurr = pFirst;
    while ( pCurr )
    {
      pCurr->Save( &save );
      pCurr = pCurr->pNext;
    }
    save.close();
  }
  else
  {
    QString err = save.errorString();
    KKK();
  }
  }

void WinList::UpdateAllTime()
  {
  WinList * pCurr = pFirst;
  while ( pCurr )
    {
    pCurr->updateTime();
    pCurr = pCurr->pNext;
    }
  if ( change )
    {
    SaveAll();
    change = false;
    }
  }

void WinList::Restore( class MainWindow * pMainWnd )
  {
  Data data;
  Char<1024>Path;
  Path.Prt ( "%sWND/Linux.dat", PROJECT_ROOT );
  QFile rest( (char*)Path );
  if ( rest.open(QIODeviceBase::ReadOnly))
    {
    while ( rest.read( (char *)&data, sizeof (data )))
      {
      switch ( data.Type )
        {
        case WinList::Shema:
          {
          ShowSheme * pSheme = new  ShowSheme( NULL, data.FileName, NULL );
          pSheme->pMainWnd = pMainWnd;
          pSheme->setGeometry( data.WinRect );
          memcpy ( pSheme->Params, data.Params, WINLIST_PARAMS );
          pSheme->AfterRestore( pMainWnd );
          pSheme->show();
          }
        break;
        case WinList::Trend:
          {
          // QTrends * pTrends = new  QTrends ( NULL, data.FileName );
          // pTrends->pMainWnd = pMainWnd;
          // pTrends->setGeometry( data.WinRect );
          // memcpy ( pTrends->Params, data.Params, WINLIST_PARAMS );
          // pTrends->AfterRestore( pMainWnd );
          // pTrends->show();
          }
        break;
        case WinList::MiniTrend:
          {
          minitrend * pminitrend = new  minitrend ( NULL );
          pminitrend->pMainWnd = pMainWnd;
          pminitrend->setGeometry( data.WinRect );
          memcpy ( pminitrend->Params, data.Params, WINLIST_PARAMS );
          pminitrend->AfterRestore( pMainWnd );
          pminitrend->show();
          }
          break;
        }
      }
 //   FileName = data.FileName;

//    WinRect = data.WinRect;
    rest.close();
    }
  }


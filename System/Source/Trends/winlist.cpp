#include "winlist.h"
#include "DB.h"
#include "Err.h"
#include "qtrends.h"


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
  Update = true;
  }

struct Data
  {
  char FileName[64];
  WinList::Types Type;
  };

void WinList::Save( QFile * pF )
  {
  Data data;
  strcpy_s ( data.FileName, 64, FileName.Str );
  data.Type = Type;
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
  if ( DB::Changet )
    DB::Write();
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
    if ( pCurr->Update )
      pCurr->updateTime();
    pCurr = pCurr->pNext;
    }
  if ( change )
    {
    SaveAll();
    change = false;
    }
  if ( DB::Changet )
    DB::Write();
  }

void WinList::RestoreWnd( )
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
        case WinList::Trend:
          {
          QTrends * pTrends = new  QTrends ( NULL, data.FileName );
          pTrends->show();
          }
        break;
        }
      }
 //   FileName = data.FileName;

//    WinRect = data.WinRect;
    rest.close();
    }
  }


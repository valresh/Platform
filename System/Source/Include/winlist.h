#ifndef WINLIST_H
#define WINLIST_H

#include <QRect>
#include "CommProc.h"

#define WINLIST_PARAMS 128


class WinList
{
  public:
  static WinList * pFirst;
  static WinList * pLast;
  static bool change;
  WinList * pNext;
  CStr FileName;
  enum Types{Shema, Trend, MiniTrend };
  Types Type;
  bool Update;
//
  WinList();
  virtual void updateTime(){};
  void Close();
  static void UpdateAllTime();
  static void RestoreWnd( );
//
  static void SaveAll();
  static void Save();
  void Save( QFile * pF );
};

#endif // WINLIST_H

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
  QRect WinRect;
  BYTE Params[WINLIST_PARAMS];
//
  WinList();
  virtual void updateTime() = 0;
  virtual void PreSave() {};
  virtual void AfterRestore( class MainWindow * pMainWnd ) {};
  static void UpdateAllTime();
  static void SaveAll();
  static void Save();
  static void Restore(class MainWindow * pMainWnd);
  void Close();
  void Save( QFile * pF );
};

#endif // WINLIST_H

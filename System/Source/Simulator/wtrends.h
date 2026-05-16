#ifndef WTRENDS_H
#define WTRENDS_H

#include <QMainWindow>
#include "winlist.h"
#include "showtrends.h"
#include <QLabel>

namespace Ui {
class WTrends;
}

struct WTrends_W
{
  QRect TrendRect;
  int ScaleT;
  bool ShowSteps;
};

class WTrends : public QMainWindow, public WinList, public WTrends_W

{
  Q_OBJECT

  public:

  ShowTrends * pShow;
  TrendVar Vars[MAX_VAR];
  int kVar;
  TrendVarProp VarProp;
  QPen Pen;
  int SetShift;
  int Shift;
  int StartPos;
  int Steps;
  int nWidth;
  int nHeight;
  double mTime; // x = Time * mTime
  double Mtime;
  bool Pause;
  bool ShowSteps;
  int nSelected;
  int nVar[4000];
  QPoint pntMouse;
  QPoint pntDrug;
  bool PressMouse;
  bool Drug;
  QLabel * imageLabel;
  bool Ready;
  //////////
  //NEW
  void updateTime();
  explicit WTrends(const char * FileTrend, QWidget *parent = nullptr);
  ~WTrends();
  void paintEvent( QPaintEvent *event );

  private:
  Ui::WTrends *ui;
};

#endif // WTRENDS_H

#ifndef QTRENDS_H
#define QTRENDS_H

#include <QMainWindow>
#include "winlist.h"
#include <QSlider>
#include "showtrends.h"
#include "trendsvar.h"



namespace Ui {
class QTrends;
}

struct QTrends_W
{
  QRect WinRect;
  int ScaleT;
  bool ShowSteps;
};

class QTrends : public QMainWindow, public WinList, public QTrends_W
{
    Q_OBJECT

  public:
    explicit QTrends(QWidget *parent, const char * Trend);
    ~QTrends();
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);
    void updateTime();
    void closeEvent(QCloseEvent * event);
    bool AddTrend( const char * Name, char Type, void * pVar );
    TrendsVar ListVar;
    CStr Name;
    bool Started;
    NEW
  private:
    Ui::QTrends *ui;
  public slots:
//    void newFile();
//    void open();
  private slots:
    void on_Back_clicked();
    void on_ShowVars_clicked();
    void on_Forvard_clicked();
    void on_Pause_clicked();
    void on_Continue_clicked();
    void on_Up_clicked();
    void on_Down_clicked();
    void on_Align_clicked();
    void on_ScaleT_sliderMoved(int position);
    void on_TimeStep_pressed();
    void on_ScaleT_sliderPressed();
    void on_TimeStep_clicked();
};

#endif // QTRENDS_H

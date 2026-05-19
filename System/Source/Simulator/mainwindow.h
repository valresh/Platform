#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "Start.h"
#include "Sheme/listshem.h"
#include "Trends/listtrends.h"
#include "Sheme/showsheme.h"
#include "CommProc.h"
#include <QMessageBox>
#include "list.h"
#include "Msg/msglist.h"
#include "Sheme/findobj.h"
#include "showrsu.h"

struct CPU
{
    int N;
    int CoreId;
    double MHz;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

#define MAX_SHEME 50


class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    QTimer * timer;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    virtual void start();
    virtual void stop ();
    void closeEvent(QCloseEvent * event);
    void ProcInfo();

  CPU main_cpu[3];
  bool Set_cpu[3];
  int Max_N_cpu;
  int Step_cpu;
  QThread StartWork;
  QThread ModelsWork;
  QThread HydroWork;
  QThread DCUWork;
  Start * pStart;
  Models * pModels;
  Hydro * pHydro;
  DCU * pDCU;
  bool ObjectLoaded;
//
  ListShem listshem;
  ListTrends listtrends;
  CDrawObjData DrawData;
  FindObj Find;
  ProcessData PD;
  MsgList Msgs;
  ShowRSU showRSU;
  List Select;
  const char * StateRead();
  const char * StateWrite();
  const char * ParamsRead();
  const char * ParamsWrite();
  bool Use_RSU();
  bool TS();
 //
  QTime time;
  void ShowTrend ( const char * Trend );
  void updateTime();
  signals:
    void SetAccel ( double Accel );
  private slots:
    void on_Shema_clicked();

//    void on_Trends_clicked();

    void on_Messages_clicked();

    void on_Settings_clicked();

    void on_exit_triggered();

//    void on_AccelZ_returnPressed();

    void on_save_params_clicked();

    void on_save_state_clicked();

    void on_save_all_clicked();

    void on_Pause_clicked();

    void on_Contunue_clicked();


    void on_FindObj_clicked();

    void on_RSU_clicked();

    void on_Stop_clicked();

public slots:
    void on_Start_clicked();
    void started();
    void stopped();
    void loaded();
    void Show_Sheme ( const char * Sheme,  const char * Selected );
    void ClosedShema( ShowSheme * I );
    void OutTxt( const char * Txt );
//    void OutMsg( const char * Title, const char * Txt, QMessageBox::StandardButtons flags, QMessageBox::StandardButton def, int * Res );
    void OutMsg( const char * Title, const char * Txt, DWORD flags, DWORD def, int * Res );
    void ShowSteps ( int kSteps );
    void ShowAccel ( double Accel );
    void ShowData_1 ( double ProcUser, int Mem );
    void ShowData_2 ( double ProcUser, int Mem );
  private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

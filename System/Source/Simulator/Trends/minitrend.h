#ifndef MINITREND_H
#define MINITREND_H

#include <QWidget>
#include "winlist.h"

namespace Ui {
class minitrend;
}

#define MAX_VAR 8
class minitrend : public QWidget, public WinList
{
    Q_OBJECT

  public:
    explicit minitrend(QWidget *parent = nullptr);
    ~minitrend();
    virtual void updateTime();
    class MainWindow * pMainWnd;
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);
    void closeEvent(QCloseEvent * event);
    bool AddVar( int N );
    void PreSave();
    void AfterRestore( class MainWindow * pMainWnd );
//  private:
    Ui::minitrend *ui;
  private slots:
    void on_ScaleV_sliderMoved(int position);
    void on_Back_clicked();
    void on_Forvard_clicked();
    void on_Continue_clicked();
    void on_Pause_clicked();
    void on_ScaleT_sliderMoved(int position);
};

#endif // MINITREND_H

#ifndef L_TRENDS_H
#define L_TRENDS_H

#include <QMainWindow>
#include "winlist.h"

class QTrends : public QMainWindow, public WinList
{
    Q_OBJECT
  public:
    class MainWindow * pMainWnd;
    explicit QTrends(QWidget *parent, const char * File );
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);
    void updateTime();

  signals:
  public slots:
    void newFile();
    void open();
};

#endif // L_TRENDS_H

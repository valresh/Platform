#ifndef MAINTRENDS_H
#define MAINTRENDS_H

#include <QMainWindow>
#include "winlist.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainTrends;
}
QT_END_NAMESPACE

class MainTrends : public QMainWindow, public WinList
{
  Q_OBJECT

  public:
  QTimer * timer;
  MainTrends(QWidget *parent = nullptr);
  ~MainTrends();
  void updateTime();
  void resizeEvent(QResizeEvent *event);
  void moveEvent(QMoveEvent *event);
  private slots:
  void on_Open_clicked();
      void on_listTrends_doubleClicked(const QModelIndex &index);

  private:
  Ui::MainTrends *ui;
};
#endif // MAINTRENDS_H

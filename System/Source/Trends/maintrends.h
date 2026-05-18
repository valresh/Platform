#ifndef MAINTRENDS_H
#define MAINTRENDS_H

#include <QMainWindow>
#include "winlist.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainTrends;
}
QT_END_NAMESPACE

struct MainTrends_W
{
  QRect WinRect;
};

class MainTrends : public QMainWindow, public WinList, public MainTrends_W
{
  Q_OBJECT

  public:
  QTimer * timer;
  MainTrends(QWidget *parent = nullptr);
  ~MainTrends();
  void updateTime();
  void resizeEvent(QResizeEvent *event);
  void moveEvent(QMoveEvent *event);
  void closeEvent(QCloseEvent * event);
  private slots:
  void on_Open_clicked();
  void on_listTrends_doubleClicked(const QModelIndex &index);

  private:
  Ui::MainTrends *ui;
};
#endif // MAINTRENDS_H

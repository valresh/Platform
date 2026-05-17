#ifndef MAINTRENDS_H
#define MAINTRENDS_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainTrends;
}
QT_END_NAMESPACE

class MainTrends : public QMainWindow
{
  Q_OBJECT

  public:
  QTimer * timer;
  MainTrends(QWidget *parent = nullptr);
  ~MainTrends();
  void updateTime();
  private slots:
  void on_Open_clicked();
      void on_listTrends_doubleClicked(const QModelIndex &index);

  private:
  Ui::MainTrends *ui;
};
#endif // MAINTRENDS_H

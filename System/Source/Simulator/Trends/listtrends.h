#ifndef LISTTRENDS_H
#define LISTTRENDS_H

#include <QDialog>
#include <QListView>



namespace Ui {
class ListTrends;
}

class ListTrends : public QDialog
{
    Q_OBJECT

  public:
    explicit ListTrends(QWidget *parent = nullptr);
    ~ListTrends();
    void SetList( char * Root );
    Ui::ListTrends *ui;
  signals:
    void ShowTrend ( const char * Trend );
  private slots:
    void on_buttonBox_accepted();

    void on_List_doubleClicked(const QModelIndex &index);

    void on_List_activated(const QModelIndex &index);

  private:
//  bool eventFilter(QObject *obj, QEvent *event);
};

#endif // LISTTRENDS_H

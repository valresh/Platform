#ifndef LISTSHEM_H
#define LISTSHEM_H

#include <QDialog>
#include <QListView>



namespace Ui {
class ListShem;
}

class ListShem : public QDialog
{
    Q_OBJECT

  public:
    explicit ListShem(QWidget *parent = nullptr);
    ~ListShem();
    void SetList( char * Root );
    Ui::ListShem *ui;
  signals:
    void ShowSheme ( const char * Sheme );
  private slots:
    void on_buttonBox_accepted();

    void on_List_doubleClicked(const QModelIndex &index);

    void on_List_activated(const QModelIndex &index);

  private:
//  bool eventFilter(QObject *obj, QEvent *event);
};

#endif // LISTSHEM_H

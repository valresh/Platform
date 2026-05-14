#ifndef LISTEMF_H
#define LISTEMF_H
#include <QListView>
#include <QDialog>

class ListEMF : public QListView
{
  public:
  ListEMF(QDialog *ListShem) : QListView ( ListShem )
    {

    }
//    void mousePressEvent(QMouseEvent *event);
    QModelIndexList selectedIndexes() const override;
  signals:
    void ShowSheme ( const char * Sheme );
  public slots:
};


#endif // LISTEMF_H

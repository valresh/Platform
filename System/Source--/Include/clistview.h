#ifndef CLISTVIEW_H
#define CLISTVIEW_H

#include <QListView>

class CListView : public QListView
{
  public:
    CListView(QWidget *parent = nullptr): QListView(parent)
    {

    }
    QModelIndexList selectedIndexes() const override
    {
      return QListView::selectedIndexes();
    }
};


#endif // CLISTVIEW_H

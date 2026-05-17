#ifndef LISTTRENDS_H
#define LISTTRENDS_H

#include <QListView>

class ListTrends : public QListView
{
  public:
  QModelIndexList selectedIndexes() const override;
  ListTrends( QWidget *parent );
  void SetList( char * Root );
  void on_buttonBox_accepted();
  void on_List_doubleClicked(const QModelIndex &index);
};

#endif // LISTTRENDS_H

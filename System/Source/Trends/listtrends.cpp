#include "listtrends.h"
#include <QDir>
#include "Err.h"
#include <QStringListModel>
#include <QMouseEvent>
#include "CommProc.h"


ListTrends::ListTrends( QWidget *parent ) : QListView ( parent )
{

}

QModelIndexList ListTrends::selectedIndexes() const
{
  return QListView::selectedIndexes();
}

void ListTrends::SetList( char * Root )
{
  QString Dir = Root;
  Dir += "INI/Trends/";
  QDir DirList( Dir );
  QStringList Filtr ( "*.csv");
  QFileInfoList L = DirList.entryInfoList( Filtr,
                                          QDir::NoFilter, QDir::Name);
  QStringList List;
  foreach (QFileInfo f, L)
    {
      QString Name = f.fileName();
      Name.truncate( Name.size() - 4 );
      List.append( Name );
    }
  QStringListModel * model = new QStringListModel(this);
  model->setStringList(List);
  model->sort(0);
  setModel(model);
}


#include "listtrends.h"
#include "ui_listtrends.h"
#include <QDir>
#include "Err.h"
#include <QStringListModel>
#include <QMouseEvent>

ListTrends::ListTrends(QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::ListTrends)
{
  ui->setupUi(this);
  //  qApp->installEventFilter(this);
  KKK();
}

ListTrends::~ListTrends()
{
  delete ui;
}

void ListTrends::SetList( char * Root )
{
  QString Dir = Root;
  Dir += "INI/Trends/";
  QDir DirList( Dir );
  QStringList Filtr ( "*.csv");
  QFileInfoList L = DirList.entryInfoList( Filtr,
                    QDir::NoFilter, QDir::Name);
  //  ui->List->setModel();
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
  ui->List->setModel(model);
  KKK();
}

//QModelIndexList ListEMF::selectedIndexes() const
//{
//  return QListView::selectedIndexes();
//}

void ListTrends::on_buttonBox_accepted()
{
  QModelIndexList Sel = ui->List->selectedIndexes();
  if ( !Sel.isEmpty())
  {
    foreach (QModelIndex Ind, Sel)
    {
      QString itemText = Ind.data(Qt::DisplayRole).toString();
      emit ShowTrend ( itemText.toStdString().c_str());
    }
  }
  ui->List->clearSelection();
}


void ListTrends::on_List_doubleClicked(const QModelIndex &index)
{
  on_buttonBox_accepted();
  //   ui->List->clearSelection();
  accept();
}


void ListTrends::on_List_activated(const QModelIndex &index)
{
  ui->List->clearSelection();
}


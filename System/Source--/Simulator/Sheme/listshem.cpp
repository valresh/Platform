#include "listshem.h"
#include "ui_listshem.h"
#include <QDir>
#include "Err.h"
#include <QStringListModel>
#include <QMouseEvent>

ListShem::ListShem(QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::ListShem)
{
  ui->setupUi(this);
//  qApp->installEventFilter(this);
  KKK();
}

ListShem::~ListShem()
{
  delete ui;
}

void ListShem::SetList( char * Root )
  {
  QString Dir = Root;
  Dir += "DATA/SCHEME/";
  QDir DirList( Dir );
  QStringList Filtr ( "*.emf");
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

 // bool ListShem::eventFilter(QObject *obj, QEvent *event)
 //  {
 //   if (obj == ui->List->viewport() && event->type() == QEvent::MouseButtonPress)
 //   {
 //     QMouseEvent *ev =
 //       static_cast<QMouseEvent *>(event);
 //     if (ev->buttons() & Qt::LeftButton)
 //     {
 //       QModelIndex index = ui->List->currentIndex();
 //       QString itemText = index.data(Qt::DisplayRole).toString();
 //       emit ShowSheme ( itemText.toStdString().c_str());
 //       KKK();
 //     }
 //   }
 //   return QObject::eventFilter(obj, event);
 //  }

  QModelIndexList ListEMF::selectedIndexes() const
    {
    return QListView::selectedIndexes();
    }

  void ListShem::on_buttonBox_accepted()
  {
     QModelIndexList Sel = ui->List->selectedIndexes();
     if ( !Sel.isEmpty())
       {
       foreach (QModelIndex Ind, Sel)
       {
         QString itemText = Ind.data(Qt::DisplayRole).toString();
         emit ShowSheme ( itemText.toStdString().c_str(),NULL);
       }
      }
     ui->List->clearSelection();
  }


  void ListShem::on_List_doubleClicked(const QModelIndex &index)
  {
   on_buttonBox_accepted();
//   ui->List->clearSelection();
   accept();
  }


  void ListShem::on_List_activated(const QModelIndex &index)
  {
  ui->List->clearSelection();
  }

